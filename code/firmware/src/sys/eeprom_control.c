/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @brief Basic structures and routines to transfer data to and 
 * from memory
 */

#include "sys/eeprom_control.h"

#include "common/util.h"
#include "sys/eeprom_control.h"
#include "sys/io/serial_control.h"
#include "pwm/virtual_PWM.h"
#include "sys/lcd_screen.h"

#include <string.h>

eeprom_t eeprom_vars EEMEM = { 0x0 };
eeprom_t ram_vars = { 0x0 };

void slot_to_eeprom(slot_t *slot, uint8_t eeprom_idx) {
    memcpy(&ram_vars.slots[eeprom_idx], slot, sizeof(slot_t));
    eeprom_write_block(&ram_vars.slots[eeprom_idx], &eeprom_vars.slots[eeprom_idx], sizeof(slot_t));
}

void used_to_eeprom() {
    eeprom_write_block(&ram_vars.used_slots, &eeprom_vars.used_slots, sizeof(array_t));
}

void eeprom_setup(pwm_pin_t *pins) {
    array_setup(&ram_vars.used_slots);
    
    // Uninitialized EEPROM, set some defaults
    if (eeprom_read_byte(&eeprom_vars.init_val) != 0x69) {
        ram_vars.init_val = 0x69;
        ram_vars.serial = 77;
        ram_vars.password[0] = -1;
        ram_vars.brightness = 3;
        ram_vars.default_slot = -1;

        eeprom_write_block(&ram_vars, &eeprom_vars, sizeof(eeprom_t));
    }
    else {
        eeprom_read_block(&ram_vars, &eeprom_vars, sizeof(eeprom_t));

        if (ram_vars.default_slot != -1) {
            // TODO: Handle in PWM control file ¿?
            slot_t to_load;
            eeprom_get_slot(ram_vars.default_slot, &to_load);
            
            for (int i = 0; i < NUM_PINS; i++) {
                set_pin_mode(pins, i, to_load.pwms[i].mode);
                set_pin_config(pins, i, to_load.pwms[i].frq, to_load.pwms[i].dty);
                set_pin_phase(pins, i, to_load.pwms[i].phs);
            }

            sync_pwms(pins);
        }
    }
}

uint8_t eeprom_get_init_val() {
    return ram_vars.init_val;
}

uint16_t eeprom_get_serial() {
    return ram_vars.serial;
}

int8_t *eeprom_get_password(int8_t *dest) {
    memcpy(dest, ram_vars.password, sizeof(ram_vars.password));
    return dest;
}

int8_t eeprom_get_default_slot() {
    return ram_vars.default_slot;
}

uint8_t eeprom_get_used_slots() {
    return array_size(&ram_vars.used_slots);
}

uint8_t eeprom_get_brightness() {
    return ram_vars.brightness;
}

slot_t *eeprom_get_slot(uint8_t ui_idx, slot_t *dest) {
    uint8_t eeprom_idx = array_get(&ram_vars.used_slots, ui_idx);
    memcpy(dest, &ram_vars.slots[eeprom_idx], sizeof(slot_t));
    return dest;
}

char *eeprom_get_slot_name(uint8_t ui_idx, char *dest) {
    uint8_t eeprom_idx = array_get(&ram_vars.used_slots, ui_idx);
    memcpy(dest, ram_vars.slots[eeprom_idx].name, EE_SLOT_NAME_SIZE * sizeof(char));
    return dest;
}

void eeprom_set_serial(uint16_t value) {
    if (value != ram_vars.serial) {
        ram_vars.serial = value;
        eeprom_write_word(&eeprom_vars.serial, ram_vars.serial);
    }
}

void eeprom_set_password(int8_t *values) {
    uint8_t different = 0;

    for (int i = 0; i < 3 && !different; i++) {
        if (values[i] != ram_vars.password[i]) different = 1;
    }

    if (different) {
        memcpy(ram_vars.password, values, 3 * sizeof(int8_t));
        eeprom_write_block(ram_vars.password, eeprom_vars.password, 3 * sizeof(int8_t));
    }
}

void eeprom_set_default_slot(int8_t value) {
    ram_vars.default_slot = value;
    eeprom_write_byte((uint8_t *)&eeprom_vars.default_slot, value);
}

void eeprom_set_brightness(uint8_t value) {
    if (value != ram_vars.brightness) {
        ram_vars.brightness = value;
        eeprom_write_byte(&eeprom_vars.brightness, ram_vars.brightness);
    }
}

bool eeprom_new_slot(slot_t *slot) {
    int8_t eeprom_idx = -1;

    for (int i = 0; i < NUM_SLOTS && eeprom_idx == -1; i++) {
        if (ram_vars.slots[i].used == false) eeprom_idx = i;
    }

    if (eeprom_idx != -1) {
        array_add(&ram_vars.used_slots, eeprom_idx);
        used_to_eeprom();

        if (slot->name[0] == '\0')
        {
            char tmp1[EE_SLOT_NAME_SIZE] = "New ";
            char tmp2[3];
            strcat(tmp1, itos(eeprom_idx + 1, get_num_length(eeprom_idx + 1), tmp2));
    
            strcpy(slot->name, tmp1);
        }

        slot->used = true;

        slot_to_eeprom(slot, eeprom_idx);

        return true;
    }

    return false;
}

void eeprom_overwrite_slot(uint8_t ui_idx, slot_t *slot) {
    uint8_t eeprom_idx = array_get(&ram_vars.used_slots, ui_idx);
    slot_to_eeprom(slot, eeprom_idx);
}

void eeprom_delete_slot(uint8_t ui_idx) {
    uint8_t eeprom_idx = array_get(&ram_vars.used_slots, ui_idx);

    slot_t to_delete;
    eeprom_get_slot(eeprom_idx, &to_delete);
    memset(&to_delete, 0, sizeof(slot_t));
    
    slot_to_eeprom(&to_delete, eeprom_idx);
    
    array_remove(&ram_vars.used_slots, ui_idx);
    used_to_eeprom();

    if (ram_vars.default_slot == eeprom_idx) {
        eeprom_set_default_slot(-1);
    }
}

void eeprom_delete_all_slots() {
    memset(&ram_vars.slots, 0, NUM_SLOTS*sizeof(slot_t));
    array_empty(&ram_vars.used_slots);
    used_to_eeprom();
}

void eeprom_set_pwm(uint8_t ui_idx, uint8_t pwm_idx, pwm_t *pwm)
{
    slot_t slot;
    eeprom_get_slot(ui_idx, &slot);

    memcpy(&slot.pwms[pwm_idx], pwm, sizeof(pwm_t));

    uint8_t eeprom_idx = array_get(&ram_vars.used_slots, ui_idx);
    slot_to_eeprom(&slot, eeprom_idx);
}

void ram_set_slot(slot_t *slot, uint8_t idx) {
    memcpy(&slot, &ram_vars.slots[idx], sizeof(slot_t));
}

void print_ram() {
    serial_writeln_s("-- RAM ----------");

    serial_write_s("init_val: ");
    serial_writeln_n(eeprom_get_init_val());
    serial_write_s("serial: ");
    serial_writeln_n(eeprom_get_serial());

    int8_t password[3];
    eeprom_get_password(password);
    serial_write_s("password: ");
    for (int i = 0; i < 3; i++) {
        if (i != 2) { serial_write_n(password[i]); serial_write_s(", "); }
        else { serial_writeln_n(password[i]); }
    }

    serial_write_s("brightness: ");
    serial_writeln_n(eeprom_get_brightness());
    serial_write_c('\n');
}

void print_eeprom() {
    uint8_t true_init_val = eeprom_read_byte(&eeprom_vars.init_val);
    uint16_t true_serial = eeprom_read_word(&eeprom_vars.serial);
    int8_t true_password[3];
    eeprom_read_block(true_password, eeprom_vars.password, 3*sizeof(int8_t));
    uint8_t true_brightness = eeprom_read_byte(&eeprom_vars.brightness);

    serial_writeln_s("-- TRUE EEPROM ----------");

    serial_write_s("init_val: ");
    serial_writeln_n(true_init_val);
    serial_write_s("serial: ");
    serial_writeln_n(true_serial);

    serial_write_s("password: ");
    for (int i = 0; i < 3; i++) {
        if (i != 2) { serial_write_n(true_password[i]); serial_write_s(", "); }
        else { serial_writeln_n(true_password[i]); }
    }

    serial_write_s("brightness: ");
    serial_writeln_n(true_brightness);
    serial_write_c('\n');
}

void print_pwm(pwm_t *pwm) {
    serial_write_s("-- ");
    serial_write_s(pwm->name);
    serial_writeln_s(" ----------");

    serial_write_s("mode: ");
    serial_writeln_n(pwm->mode);
    serial_write_s("frq: ");
    serial_writeln_n(pwm->frq);
    serial_write_s("dty: ");
    serial_writeln_n(pwm->dty);
    serial_write_s("phs: ");
    serial_writeln_n(pwm->phs);

    serial_write_c('\n');
}

void print_slot(slot_t *slot) {
    serial_write_s("== ");
    serial_write_s(slot->name);
    serial_writeln_s(" ==========");

    for (int i = 0; i < NUM_PINS; i++) {
        print_pwm(&slot->pwms[i]);
    }
}

void print_used() {
    // serial_writeln_s("RAM used slots:");
    
    // array_print(&ram_vars.used_slots);

/*     eeprom_read_block(ram_vars.used_slots, eeprom_vars.used_slots, NUM_SLOTS*sizeof(uint8_t));

    serial_writeln_s("EEPROM used slots:");
    
    for (int i = 0; i < NUM_SLOTS; i++) {
        if (i == (NUM_SLOTS - 1)) {
            serial_writeln_n(ram_vars.used_slots[i]);
            serial_write_c('\n');
        }
        else {
            serial_write_n(ram_vars.used_slots[i]);
            serial_write_s(", ");
        }
    } */
}

void eeprom_test() {
    serial_writeln_s("SLOTS USADOS EN RAM:");
    array_print(&ram_vars.used_slots);

    array_t eeprom_slots;
    array_setup(&eeprom_slots);
    eeprom_read_block(&eeprom_slots, &eeprom_vars.used_slots, sizeof(array_t));

    serial_write_c('\n');

    serial_writeln_s("SLOTS USADOS EN EEPROM:");
    array_print(&eeprom_slots);

    serial_write_c('\n');

    for (int i = 0; i < array_size(&ram_vars.used_slots); i++) {
        print_slot(&ram_vars.slots[(array_get(&ram_vars.used_slots, i))]);
    }

    /* int8_t pass[3];
    eeprom_get_password(pass);
    serial_writeln_s("CONTRASEÑA:");

    for (int i = 0; i < 3; i++)
    {
        serial_write_n(pass[i]);
    }

    serial_write_c('\n');

    int8_t defslot = eeprom_get_default_slot();
    serial_writeln_s("DEFAULT:");
    serial_write_n(defslot);

    serial_write_c('\n'); */


    /* slot_t slot;
    eeprom_get_slot(0, &slot);
    print_slot(&slot);

    serial_write_c('\n'); */


    /* 
    for (int i = 0; i < array_size(&ram_vars.used_slots); i++) {
        slot_t slot;
        eeprom_get_slot(array_get(&ram_vars.used_slots, i), &slot);

        print_slot(&slot);
        serial_write_c('\n');
    } */
}