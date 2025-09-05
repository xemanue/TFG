/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <event_control.h> @endcode
 * 
 * @brief Basic event handling
 */

#ifndef EVENT_CONTROL_H
#define EVENT_CONTROL_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Defines the different kinds of event
 */
typedef enum event_t {
    EV_NONE, /**< Null */
    EV_ROT_L = -1, /**< Left rotary encoder rotation */
    EV_ROT_R = 1, /**< Right rotary encoder rotation */
    EV_ROT_P, /**< Push button */
    EV_ROT_H, /**< Button hold */
    EV_SERIAL /**< Serial message ready to be parsed */
} event_t;

/**
 * @brief Handles the different types of event
 * @see event_t
 * 
 * @param ev Event to handle
 */
void event_handler(event_t ev);

#ifdef __cplusplus
    }
#endif

#endif // EVENT_CONTROL_H