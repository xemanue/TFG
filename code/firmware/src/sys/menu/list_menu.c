/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 * @brief Appearance and logic of the device's main menu
 */

#include "sys/menu/list_menu.h"

#include "common/util.h"
#include "sys/menu/pwm_menu.h"
#include "sys/menu_control.h"
#include "sys/lcd_screen.h"
#include "sys/eeprom_control.h"

static char entries[LST_NUM_ENTRIES][LCD_WIDTH] = {
    "", "", "", "", "", "", "", "", // PWM names will be set at runtime
    "LOAD",
    "SAVE",
    "DELETE",
    "BRIGHTNESS"
};

static int8_t active_slot;
static uint8_t local_cursor = 0;
static uint8_t global_cursor = 0;
static bool on_load = false;
static uint8_t on_save = 0;
static uint8_t on_delete = 0;
static bool on_brightness = false;
static bool selected_confirm = false;
static uint8_t selected_slot = 0;

void list_menu_setup() {
    active_slot = eeprom_get_default_slot();
    list_update_names();
}

void list_reload() {
    lcd_clrscr();

    // Print cursor
    lcd_gotoxy(0, local_cursor);
    lcd_putc(RIGHT_ARROW);

    // Print options
    for (int i = global_cursor; i < (global_cursor + LCD_LINES); i++) {
        lcd_gotoxy(1, i - global_cursor);
        lcd_puts(entries[i]);
    }

    // Print selections
    char tmp[EE_PWM_NAME_SIZE];

    if (on_load) {
        lcd_gotoxy(5, local_cursor);
        lcd_putc(LEFT_ARROW);

        if (selected_slot == 0) lcd_puts("BACK");
        else lcd_puts(eeprom_get_slot_name(selected_slot - 1, tmp));

        lcd_putc(RIGHT_ARROW);
    }
    else if (on_save != 0) {
        lcd_gotoxy(5, local_cursor);

        if (on_save == 1) {
            lcd_putc(LEFT_ARROW);

            if (selected_slot == 0) lcd_puts("BACK");
            else if (0 < selected_slot &&
                     selected_slot < eeprom_get_used_slots() + 1) {
                lcd_puts(eeprom_get_slot_name(selected_slot - 1, tmp));
            }
            else lcd_puts("NEW");

            lcd_putc(RIGHT_ARROW);
        }
        else if (on_save == 2) {
            lcd_putc(LEFT_ARROW);

            if (selected_confirm) lcd_puts("CONFIRM");
            else lcd_puts("CANCEL");

            lcd_putc(RIGHT_ARROW);
        }
    }
    else if (on_delete != 0) {
        lcd_gotoxy(7, local_cursor);

        if (on_delete == 1) {
            lcd_putc(LEFT_ARROW);

            if (selected_slot == 0) lcd_puts("BACK");
            else lcd_puts(eeprom_get_slot_name(selected_slot - 1, tmp));

            lcd_putc(RIGHT_ARROW);
        }
        else if (on_delete == 2) {
            lcd_putc(LEFT_ARROW);

            if (selected_confirm) lcd_puts("CONFIRM");
            else lcd_puts("CANCEL");

            lcd_putc(RIGHT_ARROW);
        }
    }
    else if (on_brightness) {
        lcd_gotoxy(11, local_cursor);
        lcd_putc(LEFT_ARROW);

        lcd_puts(itos(5 - get_brightness() / 20, 1, tmp));

        lcd_putc(RIGHT_ARROW);
    }
}

void list_scroll(int8_t dir) {
    if (on_load || on_delete == 1) {
        selected_slot = wrap(selected_slot + dir, 0, eeprom_get_used_slots());
        reload_screen();

        return;
    }
    else if (on_save == 1) {
        selected_slot = wrap(selected_slot + dir, 0, eeprom_get_used_slots() + 1); // Available slots + NEW
        reload_screen();

        return;
    }
    else if (on_save == 2 || on_delete == 2) {
        selected_confirm = !selected_confirm;
        reload_screen();

        return;
    }
    else if (on_brightness){
        set_brightness(wrap(get_brightness() - (dir * 20), LCD_MIN_BRIGHTNESS, LCD_MAX_BRIGHTNESS));
        reload_screen();

        return;
    }

    bool min, max;
    local_cursor = limit_hit(local_cursor + dir, 0, 3, &min, &max);

    if (min || max) {
        global_cursor = wrap_hit(global_cursor + dir, 0, LST_NUM_ENTRIES - LCD_LINES, &min, &max);

        if (max) { // It hit the max value, so it wrapped
            local_cursor = 0;
        }
        else if (min) { // It hit the min value, so it wrapped
            local_cursor = 3;
        }
    }

    reload_screen();
}

void list_button_press() {
    uint8_t selected = global_cursor + local_cursor;

    switch (selected) {
        // LOAD option
        case LST_LOAD_INDEX: 
            if (get_locked()) {
                change_menu(PASS_MENU);
                return;
            }

            if (on_load == 0) {
                selected_slot = 0;
                on_load = true;
            }
            else {
                if (selected_slot != 0) {
                    active_slot = selected_slot - 1;

                    slot_t to_load;
                    eeprom_get_slot(active_slot, &to_load);

                    for (int i = 0; i < 8; i++){
                        set_pin_mode(active_pins, i, to_load.pwms[i].mode);
                        set_pin_config(active_pins, i,
                                          to_load.pwms[i].frq,
                                          to_load.pwms[i].dty);
                        set_pin_phase(active_pins, i, to_load.pwms[i].phs);
                    }

                    sync_pwms(active_pins);
                }

                on_load = false;
            }

            list_update_names();
            reload_screen();

            break;
        // SAVE option
        case LST_SAVE_INDEX:
            if (get_locked()) {
                change_menu(PASS_MENU);
                return;
            }

            if (on_save == 0) {
                selected_slot = 0;
                on_save = 1;
            }
            else if (on_save == 1) {
                if (selected_slot == 0) on_save = 0;
                else if (0 < selected_slot &&
                         selected_slot < (eeprom_get_used_slots() + 1)) {
                    selected_confirm = 0;
                    on_save = 2;
                }
                else {
                    slot_t new;
                    strcpy(new.name, "\0");

                    for (int i = 0; i < NUM_PINS; i++) {
                        strcpy(new.pwms[i].name, entries[i]);
                        new.pwms[i].mode = active_pins[i].mode;
                        new.pwms[i].frq = active_pins[i].frq;
                        new.pwms[i].dty = active_pins[i].dty;
                        new.pwms[i].phs = active_pins[i].phs;
                    }

                    if (!eeprom_new_slot(&new)) {
                        change_menu(WARN_MENU);
                    }

                    on_save = 0;
                }
            }
            else {
                if (selected_confirm) {
                    slot_t new;
                    eeprom_get_slot(selected_slot - 1, &new);

                    for (int i = 0; i < NUM_PINS; i++) {
                        strcpy(new.pwms[i].name, entries[i]);
                        new.pwms[i].mode = active_pins[i].mode;
                        new.pwms[i].frq = active_pins[i].frq;
                        new.pwms[i].dty = active_pins[i].dty;
                        new.pwms[i].phs = active_pins[i].phs;
                    }

                    eeprom_overwrite_slot(selected_slot - 1, &new);
                    on_save = 0;
                }
                else on_save = 1;
            }

            reload_screen();
            break;
        // DELETE option
        case LST_DELETE_INDEX:
            if (get_locked()) {
                change_menu(PASS_MENU);
                return;
            }

            if (on_delete == 0) {
                selected_slot = 0;
                on_delete = 1;
            }
            else if (on_delete == 1) {
                if (selected_slot == 0) on_delete = 0;
                else {
                    selected_confirm = 0;
                    on_delete = 2;
                }
            }
            else {
                if (selected_confirm) {
                    eeprom_delete_slot(selected_slot - 1);
                    on_delete = 0;
                }
                else on_delete = 1;
            }

            reload_screen();
            break;

        // BRIGHTNESS option
        case LST_BRIGHTNESS_INDEX:
            if (on_brightness) {
                eeprom_set_brightness(get_brightness());
            }
            on_brightness = !on_brightness;
            reload_screen();
            break;

        default:
            select_pin(selected);
            change_menu(PWM_MENU);
            break;
    }
}

void list_update_names() {
    char tmp[10];
    char num[2];

    if (active_slot == -1) {
        for (int i = 0; i < NUM_PINS; i++) {
            strcpy(tmp, "PWM ");
            strcat(tmp, itos(i + 1, 1, num));
            strcpy((char *)entries[i], tmp);
        }
    }
    else {
        slot_t default_slot;
        eeprom_get_slot(active_slot, &default_slot);

        for (int i = 0; i < NUM_PINS; i++) {
            strcpy((char *)entries[i], default_slot.pwms[i].name);
        }
    }
}

void unload_active_slot() {
    active_slot = -1;

    for (int i = 0; i < 8; i++) {
        set_pin_mode(active_pins, i, 0);
        set_pin_config(active_pins, i, 0, 0);
        set_pin_phase(active_pins, i, 0);
    }

    list_update_names();
    reload_screen();
}