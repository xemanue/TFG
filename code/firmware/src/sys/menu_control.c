/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 * @brief Manages the different menu types
 */

#include "sys/menu_control.h"

#include "common/config.h"
#include "common/util.h"
#include "sys/menu/sys_menu.h"
#include "sys/menu/list_menu.h"
#include "sys/menu/pwm_menu.h"
#include "sys/menu/pass_menu.h"
#include "sys/menu/slow_menu.h"
#include "sys/eeprom_control.h"

#include "sys/io/serial_control.h"

Menu current_menu = INFO_MENU; /**< Menu currently on screen */
Menu previous_menu = LIST_MENU; /**< Previous menu on screen */

/**
 * @see <a href="https://github.com/buxtronix/arduino/tree/master/libraries/Rotary">Buxtronix' rotary encoder library</a>
 */
static const PROGMEM unsigned char cchars[][8] = {
    { 0x00, 0x08, 0x0C, 0x0E, 0x0E, 0x0C, 0x08, 0x00 }, // Right arrow
    { 0x00, 0x02, 0x06, 0x0E, 0x0E, 0x06, 0x02, 0x00 }, // Left arrow
    { 0x00, 0x00, 0x04, 0x0E, 0x1F, 0x00 ,0x00, 0x00 }, // Up arrow
    { 0x00, 0x00, 0x1F, 0x0E, 0x04, 0x00, 0x00, 0x00 }, // Down arrow
    { 0x04, 0x0C, 0x1E, 0x0D, 0x05, 0x01, 0x0E, 0x00 }, // Back arrow
    { 0x00, 0x00, 0x0A, 0x00, 0x0E, 0x11, 0x00, 0x00 }  // Sad face
};

bool locked = true;

void menu_setup(pwm_pin_t *pwm_pins){
    // Set up pins for the LCD
    lcd_init(LCD_DISP_ON);

    // Set CG RAM start address 0
    lcd_command(_BV(LCD_CGRAM));

    // Set up brightness control
    DDRE |= _BV(3);
    TCNT3 = 0x0000;

    OCR3A = eeprom_get_brightness();

    TCCR3A = _BV(COM3A1) | _BV(WGM30); // 8 BIT PWM mode, clear on match
    TCCR3B = _BV(CS30);                // no prescaler (aprox 15 kHz)

    // Initialize menus
    active_pins = pwm_pins;

    for (int i = 0; i < LCD_NUM_CUSTOM_CHARS; i++) {
        for (int j = 0; j < 8; j++) {
            lcd_data(pgm_read_byte_near(&cchars[i][j]));
        }
    }

    list_menu_setup();
    slow_menu_setup();

    reload_screen();
}

Menu get_current_menu() {
    return current_menu;
}

void change_menu(Menu next_menu) {
    // Avoid updating screen if unnecesary
    if (next_menu != current_menu) {
        previous_menu = current_menu;
        current_menu = next_menu;

        // If changing to the slow menu, reinitialize PWMs
        if (current_menu == SLOW_MENU) slow_menu_setup();

        reload_screen();
    }
}

void revert_menu() {
    change_menu(previous_menu);
}

bool get_locked() {
    return locked;
}

void set_locked(bool new_state) {
    locked = new_state;
}

void set_brightness(uint8_t value) {
    OCR3A = value;
}

uint8_t get_brightness() {
    return OCR3A;
}

void reload_screen() {
    switch (current_menu) {
        case INFO_MENU:
            info_menu();
            break;
        case WARN_MENU:
            warn_menu();
            break;
        case LIST_MENU:
            list_reload();
            break;
        case PWM_MENU:
            pwm_reload();
            break;
        case PASS_MENU:
            pass_reload();
            break;
        case SLOW_MENU:
            slow_reload();
            break;
    }
}

void scroll(int dir) {
    switch (current_menu){
        case LIST_MENU:
            list_scroll(dir);
            break;
        case PWM_MENU:
            pwm_scroll(dir);
            break;
        case PASS_MENU:
            pass_scroll(dir);
            break;
        case SLOW_MENU:
            slow_scroll(dir);
            break;
        default:
            break;
    }
}

void button_press(){
    switch (current_menu) {
        case LIST_MENU:
            list_button_press();
            break;
        case PWM_MENU:
            pwm_button_press();
            break;
        case PASS_MENU:
            pass_button_press();
            break;
        case SLOW_MENU:
            slow_button_press();
            break;
        default:
            break;
    }
}