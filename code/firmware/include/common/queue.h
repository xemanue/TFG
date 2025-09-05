/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <array.h> @endcode
 *
 * @brief Very basic circular queue library
 */

#ifndef QUEUE_H
#define QUEUE_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

#include "config.h"

/**
 * @brief Definition of the queue type
 */
typedef struct queue_t {
    int buf[EV_QUEUE_SIZE]; /**< Queue buffer */
    uint8_t r_ptr; /**< Read "pointer" */
    uint8_t w_ptr; /**< Write "pointer" */
} queue_t;

/**
 * @brief Checks if the queue is empty
 * 
 * @param[in,out] q Queue pointer
 * @return true Queue is empty
 * @return false Queue is not empty
 */
bool queue_empty(queue_t *q);

/**
 * @brief Checks if the queue is full
 * 
 * @param[in,out] q Queue pointer
 * @return true Queue is full
 * @return false Queue is not full
 */
bool queue_full(queue_t *q);

/**
 * @brief Pops element from the queue
 * 
 * @param[in,out] q Queue pointer
 * @param[out] data Element's value
 * @return true Done successfully
 * @return false Error, queue is empty
 */
bool queue_pop(queue_t *q, int *data);

/**
 * @brief Pushes an element into the queue
 * 
 * @param[in,out] q Queue pointer
 * @param[in] data New element's value
 * @return true Done successfully
 * @return false Error, queue is full
 */
bool queue_push(queue_t *q, int data);

#ifdef __cplusplus
    }
#endif

#endif // QUEUE_H