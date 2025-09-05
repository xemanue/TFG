/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <menu_control.h> @endcode
 * 
 * @brief Manages the different menu types
 */

#include <Arduino.h>
#include <inttypes.h>

#ifdef __cplusplus
    extern "C" {
#endif


#ifndef LCD_MENU_H
#define LCD_MENU_H

#include "pwm/virtual_PWM.h"
#include "sys/lcd_screen.h"

pwm_pin_t *active_pins;

/**
 * @brief Every different menu type
 */
typedef enum Menu {
    INFO_MENU, /**< Information splashscreen */
    WARN_MENU, /**< Memory full warning */
    LIST_MENU, /**< Main list menu */
    PWM_MENU, /**< Contains a PWM's parameters */
    PASS_MENU, /**< Password menu */
    SLOW_MENU /**< Slow signals menu */
} Menu;

/**
 * @brief Available custom characters
 */
typedef enum custom_char {
    RIGHT_ARROW,
    LEFT_ARROW,
    UP_ARROW,
    DOWN_ARROW,
    BACK_ARROW,
    SAD_FACE
} custom_char;

/**
 * @brief Initializes the LCD screen snd some menu variables
 * 
 * @param[in] pwm_pins PWMs to be displayed in the PWM menu
 */
void menu_setup(pwm_pin_t *pwm_pins);

/**
 * @brief Changes the current menu
 * 
 * @param next_menu Next menu to be displayed
 */
void change_menu(Menu next_menu);

/**
 * @brief Changes back to the previous menu. Useful after the\
 * screensaver, for example
 */
void revert_menu();

/**
 * @brief Gets the current menu
 * 
 * @return Menu Currently displayed menu
 * @see Menu
 */
Menu get_current_menu();

/**
 * @brief Handles reloading the screen
 */
void reload_screen();

/**
 * @brief Processes a scroll
 * 
 * @param[in] dir Direction of the scroll
 */
void scroll(int dir);

/**
 * @brief Processes a button press
 */
void button_press();

/**
 * @brief Gets the device's lock status
 * 
 * @return true If the device hasn't been unlocked
 * @return false If the device has been unlocked
 */
bool get_locked();

/**
 * @brief Locks or unlocks the device
 * 
 * @param new_state New lock status
 */
void set_locked(bool new_state);

/**
 * @brief Set a new LCD brightness value
 * 
 * @param value New brightness value
 */
void set_brightness(uint8_t value);

/**
 * @brief Gets the current LCD brightness value
 * 
 * @return uint8_t Current LCD brightness value
 */
uint8_t get_brightness();


#ifdef __cplusplus
    }
#endif

#endif