/**
 * @author Luis Sanchez <luissanv@ugr.es>
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <pwm_gen.h> @endcode
 * 
 * @brief Basic routines to manage the control timer and
 * generate PWMs
 */

#ifndef PWM_GEN_H
#define PWM_GEN_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

#include "common/config.h"

/**
 * @brief Modes a pin can be set to
 */
typedef enum pin_mode {
    OFF_MODE = 0, /**< Logic 0 */
    PWM_MODE = 1, /**< Pin will output a PWM using its set parameters */
    ON_MODE = 2 /**< Logic 1 */
} pin_mode;

/**
 * @brief Structure to represent a PWM pin
 */
typedef struct pwm_pin_t {
    uint8_t *port; /**< GPIO port in the ATMEGA2560 */
    uint8_t pin; /**< GPIO port's bit in the ATMEGA2560 */
    uint8_t *port_config; /**< GPIO configuration register in the ATMEGA2560 */

    uint32_t cycles_on; /**< Number of interrupt cycles in which the pin shall be HIGH */
    uint32_t cycles_total; /**< Number of interrupt cycles that constitute a period */
    uint32_t cnt; /**< Interrupt cycles counter */

    pin_mode mode; /**< Channel mode */
    uint16_t frq; /**< Intended frequency for the pin */
    uint16_t dty; /**< Intended duty cycle for the pin */
    int16_t phs; /**< Intended phase for the pin */
} pwm_pin_t;

/**
 * @brief Sets up internal PWM clock 0 to generate an interrupt
 * at a 40 kHz firing rate
 * @details Sets CTCs mode with TOP at 24, with 8 clock
 * prescaler.
 * @see <a href="http://ww1.microchip.com/downloads/en/DeviceDoc/Atmel-2549-8-bit-AVR-Microcontroller-ATmega640-1280-1281-2560-2561_datasheet.pdf#page=126">The ATmega2560's datasheet</a>
 */
void setup_pwm_interrupt();

/**
 * @brief Starts the clock
 */
void start_clock();

/**
 * @brief Stops the clock
 */
void stop_clock();

/**
 * @brief To be called on each interrupt cycle, handles setting
 * PWMs ON and OFF
 * @details Goes through the PWM pins vector and checks if the
 * number of cycles passed is equal to the max time on the PWM
 * or the repeat cycle
 * @param[in] pwm_pins Vector containing all the PWM structures
 */
void pwm_cycle(pwm_pin_t *pwm_pins);

#ifdef __cplusplus
    }
#endif

#endif /* PWM_GEN_H */