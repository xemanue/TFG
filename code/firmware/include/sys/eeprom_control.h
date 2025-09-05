/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <eeprom_control.h> @endcode
 *
 * @brief Basic structures and routines to transfer data to and 
 * from memory
 */

#ifndef EEPROM_CONTROL_H
#define EEPROM_CONTROL_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

#include <avr/eeprom.h>
#include "pwm/pwm_gen.h"
#include "common/array.h"

/**
 * @brief Basic PWM representation in the EEPROM
 */
typedef struct pwm_t {
    char name[EE_PWM_NAME_SIZE]; /**< Signal name */
    uint8_t mode; /**< Signal mode (OFF, PWM, OFF) */
    uint16_t frq; /**< PWM frequency */
    uint16_t dty; /**< PWM duty cycle*/
    uint16_t phs; /**< PWM phase */
} pwm_t;

/**
 * @brief Basic memory slot
 */
typedef struct slot_t {
    char name[EE_SLOT_NAME_SIZE]; /**< Slot name */
    bool used; /**< Whether the slot currently contains data ot not */

    pwm_t pwms[NUM_PINS]; /**< The slot's 8 PWMs */
} slot_t;

/**
 * @brief EEPROM memory structure
 */
typedef struct eeprom_t {
    uint8_t init_val; /**< Initialization value, which will be set to 0x69 when memory is initialized */

    uint16_t serial; /**< Device's serial number */
    int8_t password[3]; /**< Currently set password */
    int8_t default_slot; /**< Slot to be loaded on startup */
    uint8_t brightness; /**< LCD brightness value */

    slot_t slots[NUM_SLOTS]; /**< Memory slots */
    array_t used_slots; /**< Vector containing the indices of used slots */
} eeprom_t;

/**
 * @brief Initialization routine for the EEPROM
 * @details Checks whether the memory is initialized or not
 * - If it is, loads the set default slot (if there is one)
 * - If it isn't, sets some default values
 * 
 * @param pins PWM pins
 */
void eeprom_setup(pwm_pin_t *pins);

/**
 * @brief Gets the initialization value
 * 
 * @return uint8_t Initialization value
 */
uint8_t eeprom_get_init_val();

/**
 * @brief Gets the serial number
 * 
 * @return uint16_t Serial number
 */
uint16_t eeprom_get_serial();

/**
 * @brief Gets the set password
 * 
 * @param[out] dest Array where the password will be stored
 * (must contain 3 elements)
 * @return int8_t* Pointer to the array
 */
int8_t *eeprom_get_password(int8_t *dest);

/**
 * @brief Gets the set default slot
 * 
 * @return int8_t Default slot index
 */
int8_t eeprom_get_default_slot();

/**
 * @brief Gets the number of used slots
 * 
 * @return uint8_t Used number of slots
 */
uint8_t eeprom_get_used_slots();

/**
 * @brief Gets the stored LCD brightness
 * 
 * @return uint8_t LCD brightness
 */
uint8_t eeprom_get_brightness();

/**
 * @brief Gets a given memory slot
 * 
 * @param[in] ui_idx Index of the slot (LIST MENU ORDER)
 * @param[out] dest Returned slot
 * @return slot_t* Pointer to the returned slot
 */
slot_t *eeprom_get_slot(uint8_t ui_idx, slot_t *dest);
char *eeprom_get_slot_name(uint8_t ui_idx, char *dest);

/**
 * @brief Sets the devices serial number
 * 
 * @param[in] value New serial number
 */
void eeprom_set_serial(uint16_t value);

/**
 * @brief Sets a new password
 * 
 * @param[in] values Password to be set (must contain 3 elements)
 */
void eeprom_set_password(int8_t *values);

/**
 * @brief Sets a new default slot
 * 
 * @param[in] value Index of the new default slot
 */
void eeprom_set_default_slot(int8_t value);

/**
 * @brief Stores a new brightness value
 * 
 * @param[in] value Brightness valur to be stored
 */
void eeprom_set_brightness(uint8_t value);

/**
 * @brief Saves configuration to a new memory slot
 * 
 * @param[in] slot Slot to be saved
 * @return true If slot has been saved
 * @return false If memory is full
 */
bool eeprom_new_slot(slot_t *slot);

/**
 * @brief Overwrites an existing configuration
 * 
 * @param[in] ui_idx Index of the slot to overwrite (LIST MENU
 * ORDER)
 * @param[in] slot New configuration
 */
void eeprom_overwrite_slot(uint8_t ui_idx, slot_t *slot);

/**
 * @brief Deletes a given slot
 * 
 * @param[in] ui_idx Index of the slot to be deleted (LIST MENU
 * ORDER)
 */
void eeprom_delete_slot(uint8_t ui_idx);

/**
 * @brief Deletes every slot
 */
void eeprom_delete_all_slots();

/**
 * @brief Sets the configuration of a given signal from a given
 * slot
 * 
 * @param[in] ui_idx Index of the slot (LIST MENU ORDER)
 * @param[in] pwm_idx Index of the signal
 * @param[in] pwm New configuration
 */
void eeprom_set_pwm(uint8_t ui_idx, uint8_t pwm_idx, pwm_t *pwm);

/**
 * @brief Prints a given PWM to the serial port
 * @see serial_control.h
 * 
 * @param[in] pwm PWM to be printed
 */
void print_pwm(pwm_t *pwm);

/**
 * @brief Prints a given memory slot to the serial port
 * @see serial_control.h
 * 
 * @param[in] slot Slot to be printed
 */
void print_slot(slot_t *slot);

/**
 * @brief Prints the variables stored in the RAM
 */
void print_ram();

/**
 * @brief Prints the variables stored in the EEPROM
 */
void print_eeprom();

/**
 * @brief Prints the used slots array
 */
void print_used();

/**
 * @brief Prints some EEPROM variables
 */
void eeprom_test();

#ifdef __cplusplus
    }
#endif

#endif /* EEPROM_CONTROL_H */