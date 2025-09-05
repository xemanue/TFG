/**
 * @author Luis Sanchez <luissanv@ugr.es>
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 * @brief Calculations and management for easy-to-use interface
 * to pwm_gen.h
 */

#include "pwm/virtual_PWM.h"

void set_pin_mode(pwm_pin_t *pins, uint8_t pin, pin_mode mode){
    pins[pin].mode = mode;
}

void set_pin_phase(pwm_pin_t *pins, uint8_t pin, int16_t phs) {
    pins[pin].cnt = pins[pin].cnt + pins[pin].cycles_total*phs/100 - pins[pin].cycles_total*pins[pin].phs/100;
    pins[pin].phs = phs;
}

void set_pin_config(pwm_pin_t *pins, uint8_t pin, uint32_t frq, uint32_t dty) {
    if (frq > 4000) frq = 4000;
    if (dty > 100) dty = 100;

    uint32_t per = 200000U / frq;
    uint32_t ton = per * dty / 100U;

    pins[pin].cycles_total = per;
    pins[pin].cycles_on = ton;
    pins[pin].frq = (uint16_t)frq;
    pins[pin].dty = (uint16_t)dty;

    pin_config(pins, pin, 1);
}

void pins_init(pwm_pin_t *pins){
    pins[0].port = &(PWM_PORT_0);
    pins[0].port_config = &(PWM_PORT_CONF_0);
    pins[0].pin = PWM_PIN_0;

    pins[1].port = &(PWM_PORT_1);
    pins[1].port_config = &(PWM_PORT_CONF_1);
    pins[1].pin = PWM_PIN_1;

    pins[2].port = &(PWM_PORT_2);
    pins[2].port_config = &(PWM_PORT_CONF_2);
    pins[2].pin = PWM_PIN_2;

    pins[3].port = &(PWM_PORT_3);
    pins[3].port_config = &(PWM_PORT_CONF_3);
    pins[3].pin = PWM_PIN_3;

    pins[4].port = &(PWM_PORT_4);
    pins[4].port_config = &(PWM_PORT_CONF_4);
    pins[4].pin = PWM_PIN_4;

    pins[5].port = &(PWM_PORT_5);
    pins[5].port_config = &(PWM_PORT_CONF_5);
    pins[5].pin = PWM_PIN_5;

    pins[6].port = &(PWM_PORT_6);
    pins[6].port_config = &(PWM_PORT_CONF_6);
    pins[6].pin = PWM_PIN_6;

    pins[7].port = &(PWM_PORT_7);
    pins[7].port_config = &(PWM_PORT_CONF_7);
    pins[7].pin = PWM_PIN_7;
}

void sync_pwms(pwm_pin_t *pins) {
    stop_clock();
    
    for (int i = 0; i < NUM_PINS; i++){
        pins[i].cnt = 0; 
        pins[i].cnt = pins[i].cnt + pins[i].cycles_total * pins[i].phs / 100;
    }
    
    start_clock();
}

void pin_config(pwm_pin_t *pins, uint8_t pin, uint8_t state){
    if (state){
        *(pins[pin].port_config) |= _BV(pins[pin].pin);
    }
    else {
        *(pins[pin].port_config) &= (0xFF & (~(_BV(pins[pin].pin))));
    }
}