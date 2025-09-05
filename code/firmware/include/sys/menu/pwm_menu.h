/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <pwm_menu.h> @endcode
 * 
 * @brief Appearance and logic of the device's PWM settings menu
 */

#ifndef PWM_MENU_H
#define PWM_MENU_H

#include <Arduino.h>
#include "pwm/virtual_PWM.h"

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Reloads the PWM menu
 */
void pwm_reload();

/**
 * @brief Processes a scroll in the PWM menu
 * 
 * @param[in] dir Direction of the scroll
 */
void pwm_scroll(int dir);

/**
 * @brief Processes a button press on the PWM menu
 */
void pwm_button_press();

/**
 * @brief Selects the pin to be configured
 * 
 * @param[in] idx Index of the pin
 */
void select_pin(uint8_t idx);


#ifdef __cplusplus
    }
#endif

#endif /* PWM_MENU_H */