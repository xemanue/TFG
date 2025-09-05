/**
 * @mainpage
 * Firmware for the the PWM Box headlight tester
 * @author Luis Sanchez <luissanv@ugr.es>
 * @author Ruben Sanchez
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 */

#include <Arduino.h>

#include "common/config.h"
#include "common/util.h"
#include "common/queue.h"

#include "sys/event_control.h"
#include "sys/menu_control.h"
#include "sys/eeprom_control.h"
#include "sys/lcd_screen.h"

#include "sys/io/rotary_control.h"
#include "sys/io/serial_control.h"

#include "sys/menu/slow_menu.h"


/*******************************************************************************
 * Globals
 ******************************************************************************/

pwm_pin_t pwms[NUM_PINS];

queue_t events;
int current_event; // C++ doesn't allow casting enum to int, so we cast to enum

bool push_during_startup = false;
uint64_t time_ms = millis();
uint64_t time_ms2 = millis();
uint64_t idle_time = 0;
uint64_t warn_time = 0;

uint64_t slow_time = 0;
uint64_t prev_slow_time = 0;


/*******************************************************************************
 * Basic Arduino functions
 ******************************************************************************/

void setup() {
    serial_setup();
    rotary_setup();
    
    pins_init(pwms);
    eeprom_setup(pwms);
    menu_setup(pwms);

    setup_pwm_interrupt();
}

void loop() {
    delay(1); // Dont know why, it just doesn't work without this
    Menu menu = get_current_menu();

    // Handle timed stuff
    if (millis() - time_ms > 1000) {  // Runs once per second
        time_ms = millis();

        idle_time++;
        if (menu == INFO_MENU || menu == WARN_MENU) warn_time++;
    }

    if (millis() - time_ms2 > 500) {  // Runs once every tenth of a second
        time_ms2 = millis();

        if (slow_running != -1) { slow_time++; }
        else if (slow_time != 0) slow_time = 0;
    }

    // Info menu is active
    if (menu == INFO_MENU) {
        // After boot delay
        if ((time_ms / 1000) == UI_BOOT_DELAY) {
            if (push_during_startup) change_menu(SLOW_MENU);
            else change_menu(LIST_MENU);

            idle_time = 0;
        }
        // After user interaction
        else if ((time_ms / 1000) > UI_BOOT_DELAY && idle_time == 0) {
            revert_menu();
        }
    }
    // Warn menu is active, and the set delay has gone by
    else if (menu == WARN_MENU && (warn_time == UI_BOOT_DELAY)) {
        revert_menu();
        warn_time = 0;
    }
    // UI timeout has gone by with no user interaction, and we are not in the boot sequence or running a slow signal
    else if (idle_time == UI_TIMEOUT && (time_ms / 1000) > UI_BOOT_DELAY && slow_running == -1) {
        change_menu(INFO_MENU);
    }

    if (queue_pop(&events, &current_event)) {
        event_handler((event_t)current_event);
    }
}


/*******************************************************************************
 * Interrupts
 ******************************************************************************/

/* Signals ********************************************************************/

ISR(TIMER2_COMPA_vect) {
    pwm_cycle(pwms);

    if (slow_running != -1 && slow_time != prev_slow_time) {
        prev_slow_time = slow_time;
        slow_signal(slow_time);
    }
}


/* Rotary *********************************************************************/

ISR(INT1_vect) {
    idle_time = 0;
    rotary_result_t rot = rotation_handler();

    if (rot == R_DIR_CW) queue_push(&events, EV_ROT_R);
    else if (rot == R_DIR_CCW) queue_push(&events, EV_ROT_L);
}

ISR(INT2_vect){
    idle_time = 0;
    button_result_t push = push_handler_int();
    
    if (push == B_PUSH) queue_push(&events, EV_ROT_P);
    else if (push == B_HOLD) queue_push(&events, EV_ROT_H);
}

ISR(INT3_vect) {
    idle_time = 0;
    rotary_result_t rot = rotation_handler();

    if (rot == R_DIR_CW) queue_push(&events, EV_ROT_R);
    else if (rot == R_DIR_CCW) queue_push(&events, EV_ROT_L);
}


/* UART ***********************************************************************/

ISR (USART0_RX_vect) {
    if (process_serial()) queue_push(&events, EV_SERIAL);
}

// Unhandled interrupts cause a reset, and we need RXCIE0 enabled for RX
// interrupts to work
ISR (USART0_UDRE_vect) { return; }