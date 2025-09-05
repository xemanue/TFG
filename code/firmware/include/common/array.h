/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <array.h> @endcode
 *
 * @brief Very basic array library
 */

#ifndef ARRAY_H
#define ARRAY_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

#include "config.h"

/**
 * @brief Definition of the array type
 */
typedef struct array_t {
    uint8_t buf[NUM_SLOTS]; /**< Actual array */
    uint8_t ptr; /**< Current number of elements */
    uint8_t max_size; /**< Maximum number of elements */
} array_t;

/**
 * @brief Initializes the array
 * 
 * @param[in,out] a Array pointer
 */
void array_setup(array_t *a);

/**
 * @brief Current size getter
 * 
 * @param[in,out] a Array pointer
 * @return uint8_t Current array size
 */
uint8_t array_size(array_t *a);

/**
 * @brief Maximum size getter
 * 
 * @param[in,out] a Array pointer
 * @return uint8_t Maximum array size
 */
uint8_t array_max_size(array_t *a);

/**
 * @brief Array element getter
 * 
 * @param[in,out] a Array pointer
 * @param[in] pos Element index
 * @return uint8_t Element's value
 */
uint8_t array_get(array_t *a, uint8_t pos);

/**
 * @brief Adds an element to the array
 * 
 * @param[in,out] a Array pointer
 * @param[in] data New element's value
 * @return true Done successfully
 * @return false Error, array is full
 */
bool array_add(array_t *a, unsigned char data);

/**
 * @brief Removes an element from the array
 * 
 * @param[in,out] a Array pointer
 * @param[in] pos Element index
 * @return true Done successfully
 * @return false Error, array is already empty
 */
bool array_remove(array_t *a, uint8_t pos);

/**
 * @brief Remove every element from the array
 * 
 * @param[in,out] a Array pointer
 */
void array_empty(array_t *a);

/**
 * @brief Prints the array to the serial port
 * @see serial_control.h
 * 
 * @param[in,out] a Array pointer
 */
void array_print(array_t *a);


#ifdef __cplusplus
    }
#endif

#endif /* ARRAY_H */