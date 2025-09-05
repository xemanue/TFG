/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <sys_menu.h> @endcode
 * 
 * @brief Appearance of system information menus
 */

#ifndef SYS_MENU_H
#define SYS_MENU_H

#include <Arduino.h>

#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Prints out info on the screen. We use it as a screensaver
 */
void info_menu();

/**
 * @brief Prints out a "memory full" warning on screen.
 */
void warn_menu();

#ifdef __cplusplus
    }
#endif

#endif /* SYS_MENU_H */