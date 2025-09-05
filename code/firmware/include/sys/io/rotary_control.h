/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <rotary_control.h> @endcode
 * 
 * @brief Handling rotary encoder input
 */

#ifndef ROTARY_CONTROL_H
#define ROTARY_CONTROL_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Possible rotation results
 * @details Using the set values for CW and CCW turns allows to
 * calculate the result using bitwise operations
 * @see push_handler_int
 */
typedef enum rotary_result_t {
    R_DIR_NONE, /**< No turn */
    R_DIR_CW = 16, /**< Clockwise turn */
    R_DIR_CCW = 32 /**< Counterclockwise turn */
} rotary_result_t;

/**
 * @brief Possible button results
 */
typedef enum button_result_t {
    B_NONE, /**< No action */
    B_PUSH, /**< Button push */
    B_HOLD /**< Button hold */
} button_result_t;

/**
 * @brief Configures the necessary pins to operate the rotary 
 * encoder
 * @details Enables input mode with pull-up resistors for the
 * corresponding pins (@see config.h), and enables on-change 
 * interrupts
 */
void rotary_setup();

/**
 * @brief Handles the rotary encoder's state changes on each
 * interrupt
 * 
 * @return rotary_result_t Result of the operation
 * @see rotary_result_t
 */
rotary_result_t rotation_handler();

/**
 * @brief Implements the push button's logic. Meant to be used
 * as part of an interrupt routine
 * 
 * @return button_result_t Result of the operation
 * @see button_result_t
 */
button_result_t push_handler_int();

/**
 * @brief Implements the push button's logic. Meant to be used
 * when polling the pins. Not currently in use, kept for future
 * reference if needed
 * 
 * @return button_result_t Result of the operation
 * @see button_result_t
 */
button_result_t push_handler_pol();

#ifdef __cplusplus
    }
#endif

#endif // ROTARY_CONTROL_H