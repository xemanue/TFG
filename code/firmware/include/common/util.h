/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <util.h> @endcode
 *
 * @brief Common utility functions
 */

#ifndef UTIL_H
#define UTIL_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Converts integer to string (char array)
 * 
 * @param[in] num Number to convert
 * @param[in] len "Length" of the number
 * @param[out] buf Output buffer (make sure it's at least size
 * len + 1)
 * @return char* Output buffer
 */
char *itos(int num, int len, char *buf);

/**
 * @brief Wraps a number before and after a certain limit (Eg.
 * 5 + 1, 0, 5 will return 0)
 * 
 * @param[in] num Number to wrap
 * @param[in] min Lower limit (included)
 * @param[in] max Upper limit (included)
 * @return int Wrapped number
 */
int wrap(int num, int min, int max);

/**
 * @brief Same as the wrap function, but also detects when the
 * number has wrapped
 * 
 * @param[in] num Number to wrap
 * @param[in] min Lower limit (included)
 * @param[in] max Upper limit (included)
 * @param[out] min_hit Will be true when the lower bound has
 * been reached (number is now max)
 * @param[out] max_hit Will be true when the upper bound has
 * been reached (number is now min)
 * @return int Wrapped number
 */
int wrap_hit(int num, int min, int max, bool *min_hit, bool *max_hit);

/**
 * @brief Limits a number between two values (Eg. 5 + 1, 0, 5
 * will return 5)
 * 
 * @param[in] num Number to limit
 * @param[in] min Lower limit
 * @param[in] max Upper limit
 * @return int Number between the limits
 */
int limit(int num, int min, int max);

/**
 * @brief Same as the limit function, but also detects when the
 * number has hit one of the limits
 * 
 * @param[in] num Number to limit
 * @param[in] min Lower limit
 * @param[in] max Upper limit
 * @param[out] min_hit Will be true when the lower bound has
 * been reached
 * (number is now min)
 * @param[out] max_hit Will be true when the upper bound has
 * been reached
 * (number is now max)
 * @return int Number between the limits
 */
int limit_hit(int num, int min, int max, bool *min_hit, bool *max_hit);

/**
 * @brief Gets a number's "length" (number of chars needed to
 * represent it)
 * 
 * @param[in] num Number to limit
 * @return int Number between the limits
 */
int get_num_length(int num);


#ifdef __cplusplus
    }
#endif

#endif