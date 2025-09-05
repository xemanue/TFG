/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <list_menu.h> @endcode
 * 
 * @brief Appearance and logic of the device's main menu
 */

#ifndef LIST_MENU_H
#define LIST_MENU_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Initializes variables that need to be set on runtime
 */
void list_menu_setup();

/**
 * @brief Reloads the list menu
 */
void list_reload();

/**
 * @brief Processes a scroll in the list menu
 * 
 * @param[in] dir Direction of the scroll
 */
void list_scroll(int8_t dir);

/**
 * @brief Processes a button press on the list menu
 */
void list_button_press();

/**
 * @brief Sets the PWM names to the values stored in EEPROM
 */
void list_update_names();

/**
 * @brief Restores initial state
 */
void unload_active_slot();


#ifdef __cplusplus
    }
#endif

#endif /* LIST_MENU_H */