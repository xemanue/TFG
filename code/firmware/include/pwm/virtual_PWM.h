/**
 * @author Luis Sanchez <luissanv@ugr.es>
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <virtual_PWM.h> @endcode
 * 
 * @brief Calculations and management for easy-to-use interface
 * to @ref pwm_gen.h
 */

#ifndef VIRTUAL_PWM_H
#define VIRTUAL_PWM_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

#include "common/config.h"
#include "pwm/pwm_gen.h"

/**
 * @brief Sets a PWM pin's mode
 * 
 * @param[in] pins PWM pins structure
 * @param[in] pin Index of the pin to modify
 * @param[in] mode See @ref pin_mode
 */
void set_pin_mode(pwm_pin_t *pins, uint8_t pin, pin_mode mode);

/**
 * @brief Set the frequency and duty cycle of a particular pin
 * 
 * @param[in,out] pins Vector containing the PWM pins
 * @param[in] pin Pin to be modified
 * @param[in] frq Frequency (Hz) to be set (0 - 400 Hz)
 * @param[in] dty Duty cycle (%) to be set (0 - 100)
 */
void set_pin_config(pwm_pin_t *pins, uint8_t pin, uint32_t frq, uint32_t dty);

/**
 * @brief Set the phase of a particular pin
 * @details Experimentally got 106 counter value to obtain 400
 * Hz. Since frequency changes linearly with counter value, we
 * can use the cross product to calculate the desired counter
 * value
 * 
 * @param[in,out] pins Vector containing the PWM pins
 * @param[in] pin Pin to be modified
 * @param[in] phs Phase (%) to be set (-99 - +99)
 */
void set_pin_phase(pwm_pin_t *pins, uint8_t pin, int16_t phs);

/**
 * @brief Intializes every pin
 * @note Implementation needs to be modified if more than 8 pins
 * are used
 *
 * @param [in,out] pins vector containing every PWM pin's
 * structure
 */
void pins_init(pwm_pin_t *pins);

/**
 * @brief Sync the phase of every PWM
 * @details Go through all the pins, set their counter to 0 and
 * then substract their phase
 * 
 * @param[in,out] pins PWM pins structure
 */
void sync_pwms(pwm_pin_t *pins);

/**
 * @brief Sets the GPIO configuration register to be either
 * input or output
 * 
 * @param[in,out] pins PWM pins structure
 * @param[in] pin Index of the pin to act upon
 * @param[in] state State to set the register to
 */
void pin_config(pwm_pin_t *pins, uint8_t pin, uint8_t state);

#ifdef __cplusplus
    }
#endif

#endif /* VIRTUAL_PWM */