/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @brief Very basic array library
 */

#include "common/array.h"
#include "sys/io/serial_control.h"
#include <string.h>

void array_setup(array_t *a) {
    a->ptr = 0;
    a->max_size = NUM_SLOTS;
}

unsigned char array_size(array_t *a) {
    return a->ptr;
}

unsigned char array_get(array_t *a, uint8_t pos) {
    if (pos < a->ptr) {
        return a->buf[pos];
    }

    return UINT8_MAX;
}

bool array_add(array_t *a, unsigned char data) {
    if (a->ptr != a->max_size) {
        a->buf[a->ptr] = data;
        a->ptr++;
        return true;
    }

    return false;
}

bool array_remove(array_t *a, uint8_t pos) {
    if (pos < a->ptr) {
        uint8_t i = pos;

        while (i < a->ptr) {
            a->buf[i] = a->buf[i + 1];
            i++;
        }

        a->ptr--;

        return true;
    }

    return false;
}

void array_empty(array_t *a) {
    a->ptr = 0;
}

void array_print(array_t *a) {
    serial_writeln_s("-- Array ----------");
    serial_write_s("Size: ");
    serial_writeln_n(array_size(a));
    serial_writeln_s("Elements: ");

    int i = 0;
    
    while (i < a->ptr) {
        if (i == (a->ptr - 1)) {
            serial_writeln_n(a->buf[i++]);
            serial_write_c('\n');
        }
        else {
            serial_write_n(a->buf[i++]);
            serial_write_s(", ");
        }
    }
}