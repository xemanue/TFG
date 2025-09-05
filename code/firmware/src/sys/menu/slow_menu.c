/**
 *  @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 *  @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 *  @brief Appearance and logic of the device's slow signals menu
 */

#include "sys/menu/slow_menu.h"

#include "common/util.h"
#include "sys/menu_control.h"
#include "sys/lcd_screen.h"
#include "sys/io/serial_control.h"


static char entries[SLOW_NUM_ENTRIES][LCD_WIDTH] = {
    "Blinkers",
    "Fernlicht",
    "Fernlicht L & R",
    "Fernlicht M",
    "Tagfah. & Abblen.",
};

static uint8_t local_cursor = 0;
static uint8_t global_cursor = 0;


/* Local decalarations */

bool blinkers_on = false;
bool fernlitch_on = false;
bool abblendlicht_on = false;

void update_blinkers(uint64_t half_seconds);
void update_fernlicht(uint64_t half_seconds);
void update_fernlicht_lr(uint64_t half_seconds);
void update_fernlicht_m(uint64_t half_seconds);
void update_tag_abb(uint64_t half_seconds);


/* Definitions */

void slow_menu_setup()
{
    slow_running = -1;

    for (int i = 0; i < NUM_PINS; i++) {
        set_pin_mode(active_pins, i, OFF_MODE);
        set_pin_config(active_pins, i, 0, 50);
        set_pin_phase(active_pins, i, 0);
    }
}

void slow_reload() {
    if (slow_running == -1) {
        lcd_clrscr();

        // Print cursor
        lcd_gotoxy(0, local_cursor);
        lcd_putc(RIGHT_ARROW);
        
        // Print entries
        for (int i = global_cursor; i < (global_cursor + LCD_LINES); i++) {
            lcd_gotoxy(1, i - global_cursor);
            lcd_puts(entries[i]);
        }
    }
    else {
        lcd_clrscr();

        // Print cursor and back arrow
        lcd_gotoxy(18, 3);
        lcd_putc(RIGHT_ARROW);
        lcd_putc(BACK_ARROW);

        // Print info
        lcd_gotoxy(5, 1);
        lcd_puts("Running...");

        lcd_gotoxy((uint8_t)(LCD_WIDTH - strlen(entries[slow_running])) / 2, 2);
        lcd_puts(entries[slow_running]);
    }
}

void slow_scroll(int8_t dir) {
    if (slow_running == -1) {
        bool min, max;
        local_cursor = limit_hit(local_cursor + dir, 0, 3, &min, &max);

        if (min || max) {
            global_cursor = wrap_hit(global_cursor + dir, 0, SLOW_NUM_ENTRIES - LCD_LINES, &min, &max);

            if (max) { // It hit the max value, so it wrapped
                local_cursor = 0;
            }
            else if (min) { // It hit the min value, so it wrapped
                local_cursor = 3;
            }
        }
    }

    reload_screen();
}

void slow_button_press() {
    if (slow_running == -1) {
        slow_running = local_cursor + global_cursor;
    }
    else {
        slow_menu_setup();  // Ensure all signals are off
    }

    reload_screen();
}

void slow_signal(uint64_t half_seconds)
{
    switch (slow_running)
    {
        case SLOW_BL_INDEX: update_blinkers(half_seconds); break;
        case SLOW_FL_INDEX: update_fernlicht(half_seconds); break;
        case SLOW_FLLR_INDEX: update_fernlicht_lr(half_seconds); break;
        case SLOW_FLM_INDEX: update_fernlicht_m(half_seconds); break;
        case SLOW_TA_INDEX: update_tag_abb(half_seconds); break;
    }
}

void update_blinkers(uint64_t half_seconds) {
    switch (half_seconds) {
        case 0:
            set_pin_config(active_pins, SLOW_BL_PIN, 10, 50);
            set_pin_mode(active_pins, SLOW_BL_PIN, PWM_MODE);
            break;
        case 10:
            set_pin_mode(active_pins, SLOW_BL_PIN, OFF_MODE);
            slow_running = -1;
            reload_screen();
            break;
    }
}

void update_fernlicht(uint64_t half_seconds) {
    switch(half_seconds) {
        case 0:
            set_pin_mode(active_pins, SLOW_FL_PIN, ON_MODE);
            break;
        case 40:
            set_pin_mode(active_pins, SLOW_FL_PIN, OFF_MODE);
            break;
        case 64:
            set_pin_mode(active_pins, SLOW_FL_PIN, PWM_MODE);
            set_pin_config(active_pins, SLOW_FL_PIN, 20, 50);
            break;
        case 76:
            set_pin_config(active_pins, SLOW_FL_PIN, 100, 50);
            break;
        case 80:
            set_pin_mode(active_pins, SLOW_FL_PIN, OFF_MODE);
            slow_running = -1;
            reload_screen();
            break;
    }
}

void update_fernlicht_lr(uint64_t half_seconds)
{
    switch (half_seconds)
    {
        case 0:
            set_pin_config(active_pins, SLOW_FLLR_PIN, 100, 50);
            set_pin_mode(active_pins, SLOW_FLLR_PIN, ON_MODE);
            break;
        case 4:
            set_pin_mode(active_pins, SLOW_FLLR_PIN, PWM_MODE);
            break;
        case 12:
            set_pin_mode(active_pins, SLOW_FLLR_PIN, ON_MODE);
            break;
        case 20:
            set_pin_mode(active_pins, SLOW_FLLR_PIN, PWM_MODE);
            break;
        case 28:
            set_pin_mode(active_pins, SLOW_FLLR_PIN, ON_MODE);
            break;
        case 40:
            set_pin_mode(active_pins, SLOW_FLLR_PIN, OFF_MODE);
            slow_running = -1;
            reload_screen();
            break;
    }
}

void update_fernlicht_m(uint64_t half_seconds) {
    switch (half_seconds)
    {
        case 0:
            set_pin_config(active_pins, SLOW_FLM_PIN, 100, 50);
            set_pin_mode(active_pins, SLOW_FLM_PIN, ON_MODE);
            break;
        case 4:
            set_pin_mode(active_pins, SLOW_FLM_PIN, PWM_MODE);
            break;
        case 8:
            set_pin_mode(active_pins, SLOW_FLM_PIN, ON_MODE);
            break;
        case 20:
            set_pin_mode(active_pins, SLOW_FLM_PIN, PWM_MODE);
            break;
        case 24:
            set_pin_mode(active_pins, SLOW_FLM_PIN, ON_MODE);
            break;
        case 40:
            set_pin_mode(active_pins, SLOW_FLM_PIN, OFF_MODE);
            slow_running = -1;
            reload_screen();
            break;
    }
}

void update_tag_abb(uint64_t half_seconds) {
    switch (half_seconds)
    {
        case 0:
            set_pin_mode(active_pins, SLOW_TAG_PIN, OFF_MODE);
            set_pin_mode(active_pins, SLOW_ABB_PIN, ON_MODE);
            break;
        case 36:
            set_pin_mode(active_pins, SLOW_TAG_PIN, ON_MODE);
            set_pin_mode(active_pins, SLOW_ABB_PIN, OFF_MODE);
            break;
        case 40:
            set_pin_mode(active_pins, SLOW_TAG_PIN, OFF_MODE);
            set_pin_mode(active_pins, SLOW_ABB_PIN, ON_MODE);
            break;
        case 80:
            set_pin_mode(active_pins, SLOW_TAG_PIN, ON_MODE);
            set_pin_mode(active_pins, SLOW_ABB_PIN, OFF_MODE);
            break;
        case 86:
            set_pin_mode(active_pins, SLOW_TAG_PIN, OFF_MODE);
            set_pin_mode(active_pins, SLOW_ABB_PIN, ON_MODE);
            break;
        case 92:
            set_pin_mode(active_pins, SLOW_TAG_PIN, ON_MODE);
            set_pin_mode(active_pins, SLOW_ABB_PIN, OFF_MODE);
            break;
        case 120:
            set_pin_mode(active_pins, SLOW_TAG_PIN, OFF_MODE);
            set_pin_mode(active_pins, SLOW_ABB_PIN, OFF_MODE);
            slow_running = -1;
            reload_screen();
            break;
    }
}