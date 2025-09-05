/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <pass_menu.h> @endcode
 * 
 * @brief Appearance and logic of the device's password input
 * menu
 */

#ifndef PASS_MENU_H
#define PASS_MENU_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Reloads the password menu
 */
void pass_reload();

/**
 * @brief Processes a scroll in the password menu
 * 
 * @param[in] dir Direction of the scroll
 */
void pass_scroll(int dir);

/**
 * @brief Processes a button press on the password menu
 */
void pass_button_press();

#ifdef __cplusplus
    }
#endif

#endif /* PASS_MENU_H */