/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <slow_menu.h> @endcode
 * 
 * @brief Appearance and logic of the device's slow signals menu
 */

#ifndef SLOW_MENU_H
#define SLOW_MENU_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

int8_t slow_running;

/**
 * @brief Initializes variables that need to be set on runtime,
 * and turns off PWM signals
 */
void slow_menu_setup();

/**
 * @brief Reloads the slow signals menu
 */
void slow_reload();

/**
 * @brief Processes a scroll in the slow signals menu
 * 
 * @param[in] dir Direction of the scroll
 */
void slow_scroll(int8_t dir);

/**
 * @brief Processes a button press on the slow signals menu
 */
void slow_button_press();

/**
 * @brief Manages the running sequence
 * 
 * @param[in] half_seconds Time counter
 */
void slow_signal(uint64_t half_seconds);

#ifdef __cplusplus
    }
#endif

#endif /* SLOW_MENU_H */