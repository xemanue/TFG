/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <rotary_control.h> @endcode
 * 
 * @brief Handling rotary encoder input
 */

#include "sys/io/rotary_control.h"
#include "sys/io/serial_control.h"
#include "common/config.h"

typedef enum rotation_state_t {
    R_START,
    R_CW_FINAL,
    R_CW_BEGIN,
    R_CW_NEXT,
    R_CCW_BEGIN,
    R_CCW_FINAL,
    R_CCW_NEXT
} rotation_state_t;

const rotation_state_t rotation_states[7][4] = {
    // R_START
    { R_START,    R_CW_BEGIN,  R_CCW_BEGIN, R_START },
    // R_CW_FINAL
    { R_CW_NEXT,  R_START,     R_CW_FINAL,  R_START | 16 },
    // R_CW_BEGIN
    { R_CW_NEXT,  R_CW_BEGIN,  R_START,     R_START },
    // R_CW_NEXT
    { R_CW_NEXT,  R_CW_BEGIN,  R_CW_FINAL,  R_START },
    // R_CCW_BEGIN
    { R_CCW_NEXT, R_START,     R_CCW_BEGIN, R_START },
    // R_CCW_FINAL
    { R_CCW_NEXT, R_CCW_FINAL, R_START,     R_START | 32 },
    // R_CCW_NEXT
    { R_CCW_NEXT, R_CCW_FINAL, R_CCW_BEGIN, R_START },
};

unsigned char current_rotation = R_START;

uint64_t time_last_push = 0;
uint64_t time_last_release = 0;
uint64_t time_held = 0;

void rotary_setup() {
    cli();

    // Enable input mode
    ROT_DDR &= ~((1 << ROT_BIT_A) | (1 << ROT_BIT_B) | (1 << ROT_BIT_PUSH));

    // Enable pull-up resistors
    ROT_PORT |= (1 << ROT_BIT_A) | (1 << ROT_BIT_B) | (1 << ROT_BIT_PUSH);

    // INT1 -> Rotary bit A, both edges
    // INT2 -> Rotary button, both edges
    // INT3 -> Rotary bit B, both edges
    EICRA |= (1 << ISC10) | (1 << ISC20) | (1 << ISC30);
    EIMSK |= (1 << INT1) | (1 << INT2) | (1 << INT3);

    sei();
}

rotary_result_t rotation_handler() {
    unsigned char a = (ROT_PIN >> ROT_BIT_A) & 0x01;
    unsigned char b = (ROT_PIN >> ROT_BIT_B) & 0x01;

    unsigned char pinstate = (a << 1) | b;
    current_rotation = rotation_states[current_rotation & 0xf][pinstate];

    switch (current_rotation & 0x30) {
        case R_DIR_CW: return R_DIR_CW;
        case R_DIR_CCW: return R_DIR_CCW;
        default: return R_DIR_NONE;
    }
}

button_result_t push_handler_int() {
    unsigned char button_state = ~(ROT_PIN >> ROT_BIT_PUSH) & 0x01;
    
    if (button_state) {
        if (millis() - time_last_push > ROT_DEBOUNCE) {
            time_last_push = millis();
        }
    }
    else {
        time_last_release = millis();
        time_held = time_last_release - time_last_push;

        if (time_held > ROT_HOLD_TIME) return B_HOLD;
        else return B_PUSH;
    }

   return B_NONE;
}