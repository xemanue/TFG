/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @file
 * @code #include <serial_control.h> @endcode
 * 
 * @brief Handling serial communication
 */

#ifndef SERIAL_CONTROL_H
#define SERIAL_CONTROL_H

#include <Arduino.h>


#ifdef __cplusplus
    extern "C" {
#endif

/**
 * @brief Sets up serial communication
 * @details Sets the baud rate to 115200 by enabling double
 * speed operation, enables RX and TX registers, and sets data
 * frames to 8-bit format
 */
void serial_setup();

/**
 * @brief Reads data from the serial port
 * 
 * @return unsigned char Data read
 */
unsigned char serial_read();

/**
 * @brief Checks for incoming data
 * 
 * @return true If there is data available to read (RX is NOT
 * empty)
 * @return false If there is no data available to read (RX is
 * empty)
 */
bool serial_rx_available();

/**
 * @brief Checks for outgoing data
 * 
 * @return true if the port is available for data to be written
 * (TX is empty)
 * @return false if the port is not available for data to be
 * written (TX is NOT empty)
 */
bool serial_tx_available();

/**
 * @brief Sends a byte through the serial port
 * 
 * @param data Byte to be written
 */
void serial_write_c(char data);

/**
 * @brief Sends a string through the serial port
 * 
 * @param data String to be written
 */
void serial_write_s(char *data);

/**
 * @brief Sends an integer through the serial port
 * 
 * @param data Integer to be written
 */
void serial_write_n(int data);

/**
 * @brief Sends a byte through the serial port, adding a newline
 * 
 * @param data Byte to be written
 */
void serial_writeln_c(char data);

/**
 * @brief Sends a string through the serial port, adding a
 * newline
 * 
 * @param data String to be written
 */
void serial_writeln_s(char *data);

/**
 * @brief Sends an integer through the serial port, adding a
 * newline
 * 
 * @param data Integer to be written
 */
void serial_writeln_n(int data);

/**
 * @brief Organizes valid serial input into a buffer
 */
bool process_serial();

/**
 * @brief Parses the serial buffer
 */
void process_data();

#ifdef __cplusplus
    }
#endif

#endif // SERIAL_CONTROL_H