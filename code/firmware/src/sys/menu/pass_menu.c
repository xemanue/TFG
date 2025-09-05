/**
 *  @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 *  @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 *  @brief Appearance and logic of the device's password input
 *  menu
 */

#include "sys/menu/pass_menu.h"

#include "common/util.h"
#include "sys/menu_control.h"
#include "sys/eeprom_control.h"

static uint8_t local_cursor = 0;
static bool on_item = false;
static int8_t input[3] = { 0, 0, 0 };

void pass_reload() {
    lcd_clrscr();

    lcd_gotoxy(2, 0);
    lcd_puts_P("Input password:");

    switch (local_cursor) {
        case 0:
            lcd_gotoxy(1, 3);
            lcd_putc(LEFT_ARROW);
            break;
        case 1:
            lcd_gotoxy(7, 3);
            lcd_putc(UP_ARROW);
            break;
        case 2:
            lcd_gotoxy(9, 3);
            lcd_putc(UP_ARROW);
            break;
        case 3:
            lcd_gotoxy(11, 3);
            lcd_putc(UP_ARROW);
            break;
        case 4:
            lcd_gotoxy(17, 4);
            lcd_putc(RIGHT_ARROW);
            break;
    }

    if (on_item) {
        switch (local_cursor) {
            case 1:
                lcd_gotoxy(7, 1);
                lcd_putc(UP_ARROW);
                lcd_gotoxy(7, 3);
                lcd_putc(DOWN_ARROW);
                break;
            case 2:
                lcd_gotoxy(9, 1);
                lcd_putc(UP_ARROW);
                lcd_gotoxy(9, 3);
                lcd_putc(DOWN_ARROW);
                break;
            case 3:
                lcd_gotoxy(11, 1);
                lcd_putc(UP_ARROW);
                lcd_gotoxy(11, 3);
                lcd_putc(DOWN_ARROW);
        }
    }

    char buf[10];

    lcd_gotoxy(7, 2);
    lcd_puts(itos(input[0], 1, buf));
  
    lcd_gotoxy(9, 2);
    lcd_puts(itos(input[1], 1, buf));

    lcd_gotoxy(11, 2);
    lcd_puts(itos(input[2], 1, buf));

    lcd_gotoxy(18, 3);
    lcd_puts_P("OK");

    lcd_gotoxy(0, 3);
    lcd_putc(BACK_ARROW);
}

void pass_scroll(int dir) {
    if (on_item) {
        input[local_cursor - 1] = wrap(input[local_cursor - 1] + dir, 0, 9);
    }
    else {
        local_cursor = wrap(local_cursor + dir, 0, 4);
    }

    pass_reload();
}

void pass_button_press() {
    if (local_cursor == 0) {
        revert_menu();
    }
    else if (local_cursor == 4) {
        int8_t eeprom_pass[3];
        eeprom_get_password(eeprom_pass);

        if (memcmp(input, eeprom_pass, sizeof(input)) == 0) {
            on_item = false;
            set_locked(false);
            change_menu(LIST_MENU);
        }
        else {
            lcd_clrscr();
            lcd_gotoxy(7, 1);
            
            lcd_puts_P("WRONG ");
            lcd_putc(SAD_FACE);
            lcd_gotoxy(5, 2);
            
            lcd_puts_P("Try again!");
            delay (2000);
            reload_screen();
        }
    }
    else {
        if (!on_item) on_item = true;
        else on_item = false;
        reload_screen();
    }
}