/**
 *  @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 *  @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 *  @brief Appearance and logic of the device's PWM settings menu
 */

#include "sys/menu/pwm_menu.h"

#include "common/util.h"
#include "sys/menu_control.h"

static uint8_t local_cursor = 0;
static bool on_item = 0;
static bool on_frq_fine = false;
static int8_t selected_pin = -1;

void pwm_reload() {
    lcd_clrscr();

    // Print cursor
    if (local_cursor < 4) {
        lcd_gotoxy(0, local_cursor);
    }
    else {
        lcd_gotoxy(18, 3);
    }

    lcd_putc(RIGHT_ARROW);

    lcd_gotoxy(1, 0);
    lcd_puts_P("MODE ");
    
    switch (active_pins[selected_pin].mode) {
        case OFF_MODE:
            lcd_puts_P("OFF");
            break;
        case PWM_MODE:
            lcd_puts_P("PWM");
            break;
        case ON_MODE:
            lcd_puts_P(" ON");
            break;
    }

    char buf[5];

    // Print FRQ
    lcd_gotoxy(1, 1);
    lcd_puts("FRQ= ");
    lcd_puts(itos(active_pins[selected_pin].frq / 10, 3, buf));
    lcd_puts(".");
    lcd_puts(itos(active_pins[selected_pin].frq % 10, 1, buf));
    lcd_command(LCD_MOVE_CURSOR_RIGHT);
    lcd_puts("Hz");

    if (on_frq_fine) {
        lcd_puts("(f)");
    }

    // Print DTY
    lcd_gotoxy(1, 2);
    lcd_puts_P("DTY= ");
    lcd_puts(itos(active_pins[selected_pin].dty, 3, buf));
    lcd_command(LCD_MOVE_CURSOR_RIGHT);
    lcd_puts_P("%");

    // Print PHS
    lcd_gotoxy(1, 3);
    lcd_puts_P("PHS= ");
    
    if (active_pins[selected_pin].phs >= 0){
        lcd_puts_P("+");
    }
    else {
        lcd_puts_P("-");
    }
    
    lcd_puts(itos(active_pins[selected_pin].phs, 2, buf));
    lcd_command(LCD_MOVE_CURSOR_RIGHT);
    lcd_puts_P("%");

    // Print BACK_ARROW
    lcd_gotoxy(19, 3);
    lcd_putc(BACK_ARROW);

    // Print ARROWS
    if (on_item) {
        lcd_gotoxy(5, local_cursor);
        lcd_putc(LEFT_ARROW);

        if (local_cursor == 1) { //Freq
            lcd_gotoxy(11, local_cursor);
            lcd_putc(RIGHT_ARROW);
        }
        else {
            lcd_gotoxy(9, local_cursor);
            lcd_putc(RIGHT_ARROW);
        }
    }

    // Print title
    lcd_gotoxy(15, 0);
    lcd_puts("PWM ");
    lcd_puts(itos(selected_pin + 1, 1, buf));
}

void pwm_scroll(int dir) {
    // General menu navigation
    if (!on_item) {
        local_cursor = wrap(local_cursor + dir, 0, LCD_LINES);
    }
    else {
        pin_mode new_en = active_pins[selected_pin].mode;
        uint16_t new_frq = active_pins[selected_pin].frq;
        uint16_t new_dty = active_pins[selected_pin].dty;
        int16_t new_phs = active_pins[selected_pin].phs;

        // Changing mode
        if (local_cursor == 0) {
            new_en = wrap(new_en + dir, 0, 2);
            set_pin_mode(active_pins, selected_pin, new_en);
        }
        // Changing frequency
        else if (local_cursor == 1) {
            if (!on_frq_fine) {
                new_frq = wrap(new_frq + (dir * 10), 0, 4000);
            }
            else {
                new_frq = wrap(new_frq + dir, 0, 4000);
            }
            set_pin_config(active_pins, selected_pin, new_frq, new_dty);
            set_pin_phase(active_pins, selected_pin, new_phs);
        }
        // Changing duty cycle
        else if (local_cursor == 2) {
            new_dty = wrap(new_dty + dir, 0, 100);
            set_pin_config(active_pins, selected_pin, new_frq, new_dty);
            set_pin_phase(active_pins, selected_pin, new_phs);
        }
        // Changing phase
        else if (local_cursor == 3) {
            new_phs = wrap(new_phs + dir, -99, 99);
            set_pin_config(active_pins, selected_pin, new_frq, new_dty);
            set_pin_phase(active_pins, selected_pin, new_phs);
            sync_pwms(active_pins);
        }
    }

    reload_screen();
}

void pwm_button_press() {
    switch (local_cursor) {
        case 0:
            on_item = !on_item;
            reload_screen();
            break;
        case 1:
            if (active_pins[selected_pin].mode != 0) {
                if ((on_item) && (!on_frq_fine)) {
                    on_frq_fine = true;
                }
                else {
                    on_item = !on_item;
                    on_frq_fine = false;
                }
            }
            reload_screen();
            break;
        case 2:
        case 3:
            if (active_pins[selected_pin].mode != 0) {
                on_item = !on_item;
            }
            reload_screen();
            break;
        case 4:
            change_menu(LIST_MENU);
            break;
    }
}

void select_pin(uint8_t idx) {
    selected_pin = idx;
}