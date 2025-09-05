/**
 * @author Luis Sanchez <luissanv@ugr.es>
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @brief Basic routines to manage the control timer and
 * generate PWMs
 */

#include "pwm/pwm_gen.h"

void setup_pwm_interrupt() {
    TCCR2A = 0;
    TCCR2B = 0;
    TCNT2 = 0; // Counter value to 0
    OCR2A = 96; // Compare match register to 40 kHz increments

    TCCR2A |= (1 << WGM21); // CTC mode
    TCCR2B |= (1 << CS21); // 8 prescaler
    TIMSK2 |= (1 << OCIE2A); // Enable timer compare interrupt

    #ifdef DEBUG_INTERRUPT
        DDRB |= _BV(7);
        TCCR2A |= (1 << COM2A0);
    #endif

    sei();
}

void start_clock() {
    TIMSK2 |= (1 << OCIE2A);
}

void stop_clock() {
    TIMSK2 &= ~(1 << OCIE2A);
}

void turn_on(uint8_t *port, uint8_t pin) {
    *port |= _BV(pin);
}

void turn_off(uint8_t *port, uint8_t pin) {
    *port &= (0xFF & (~(_BV(pin))));
}

void pwm_cycle(pwm_pin_t * pwm_pins) {
    for (int i = 0; i < NUM_PINS; i++) {
        switch (pwm_pins[i].mode) {
            case OFF_MODE:
                turn_off(pwm_pins[i].port, pwm_pins[i].pin);
                break;
            case PWM_MODE:
                if (pwm_pins[i].cnt >= pwm_pins[i].cycles_total) {
                    if(pwm_pins[i].cycles_on != 0){
                        turn_on(pwm_pins[i].port, pwm_pins[i].pin);
                    }

                    pwm_pins[i].cnt = 0; // Reset counter
                }
                else {
                    if (pwm_pins[i].cnt == pwm_pins[i].cycles_on) {
                        turn_off(pwm_pins[i].port, pwm_pins[i].pin);
                    }
                }

                pwm_pins[i].cnt++;
                break;
            case ON_MODE:
                turn_on(pwm_pins[i].port, pwm_pins[i].pin);
                break;
            default:
                break;
        }
    }
}