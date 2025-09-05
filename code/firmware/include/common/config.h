/**
 * @author Luis Sanchez Velasco <hfegetude@gmail.com>
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 * @file
 * @code #include <config.h> @endcode
 *
 * @brief Configuration defines for the whole firmware
 */

#define HW_VERSION_23
#define SW_VERSION_20
#define SERIAL_NUMBER 0x4D


#ifdef HW_VERSION_23
    #define HW_VERSION "2.3"

    //**************************//
    // PWMs

    #define NUM_PINS 8

    #define PWM_PORT_0 PORTE
    #define PWM_PORT_CONF_0 DDRE
    #define PWM_PIN_0 4

    #define PWM_PORT_1 PORTE
    #define PWM_PORT_CONF_1 DDRE
    #define PWM_PIN_1 5

    #define PWM_PORT_2 PORTG
    #define PWM_PORT_CONF_2 DDRG
    #define PWM_PIN_2 5

    #define PWM_PORT_3 PORTH
    #define PWM_PORT_CONF_3 DDRH
    #define PWM_PIN_3 3

    #define PWM_PORT_4 PORTH
    #define PWM_PORT_CONF_4 DDRH
    #define PWM_PIN_4 4

    #define PWM_PORT_5 PORTD
    #define PWM_PORT_CONF_5 DDRD
    #define PWM_PIN_5 7

    #define PWM_PORT_6 PORTH
    #define PWM_PORT_CONF_6 DDRH
    #define PWM_PIN_6 5

    #define PWM_PORT_7 PORTB
    #define PWM_PORT_CONF_7 DDRB
    #define PWM_PIN_7 5

    //**************************//
    // Rotary encoder

    #define ROT_PORT PORTD
    #define ROT_DDR DDRD
    #define ROT_PIN PIND

    #define ROT_BIT_A 1
    #define ROT_BIT_PUSH 2
    #define ROT_BIT_B 3

    //**************************//
    // LCD

    #define LCD_WIDTH 20
    #define LCD_HEIGHT 4

#endif

#ifdef SW_VERSION_20
    #define SW_VERSION "2.0"

    //**************************//
    // Event queue

    #define EV_QUEUE_SIZE 32

    //**************************//
    // Memory

    #define NUM_SLOTS 15

    #define EE_PWM_NAME_SIZE 20 // Including '\0'
    #define EE_SLOT_NAME_SIZE 12 // Including '\0'
    #define EE_PASS_SIZE 3

    //**************************//
    // Rotary encoder

    #define ROT_DEBOUNCE 100 // ms
    #define ROT_HOLD_TIME 2000 //ms
    #define ROT_HOLD_COOLDOWN 2000 // ms

    //**************************//
    // Serial

    #define SER_BAUD 115200
    #define SER_UBRR ((F_CPU / (SER_BAUD * 8UL)) - 1)  // UART clock is 8 instead of 16 due to double speed operation
    #define SER_BUFS_SIZE 64
    #define SER_START_CHAR '^'
    #define SER_END_CHAR '\n'

    //**************************//
    // General UI

    #define UI_TIMEOUT 30  // s
    #define UI_BOOT_DELAY 3  // s
    
    #define LCD_NUM_CUSTOM_CHARS 8

    #define LCD_MIN_BRIGHTNESS 0
    #define LCD_MAX_BRIGHTNESS 100

    //**************************//
    // List menu

    #define LST_NUM_ENTRIES 12

    #define LST_LOAD_INDEX 8
    #define LST_SAVE_INDEX 9
    #define LST_DELETE_INDEX 10
    #define LST_BRIGHTNESS_INDEX 11

    //**************************//
    // Slow menu

    #define SLOW_NUM_ENTRIES 5

    #define SLOW_BL_INDEX 0
    #define SLOW_FL_INDEX 1
    #define SLOW_FLLR_INDEX 2
    #define SLOW_FLM_INDEX 3
    #define SLOW_TA_INDEX 4

    #define SLOW_BL_PIN 0
    #define SLOW_FL_PIN 1
    #define SLOW_FLLR_PIN 2
    #define SLOW_FLM_PIN 3
    #define SLOW_TAG_PIN 4
    #define SLOW_ABB_PIN 5
#endif