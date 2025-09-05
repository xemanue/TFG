/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 * @brief Handling serial communication
 */

#include "sys/io/serial_control.h"
#include "sys/eeprom_control.h"
#include "sys/menu/list_menu.h"
#include "common/config.h"
#include "common/util.h"
#include "pwm/pwm_gen.h"
#include "pwm/virtual_PWM.h"

char rx_buf[SER_BUFS_SIZE];
uint8_t rx_pos = 0;
bool rx_in_progress = false;
char tx_buf[SER_BUFS_SIZE];

void serial_setup() {
    cli();

    // Set baudrate
    UBRR0H = (unsigned char)(SER_UBRR >> 8);
    UBRR0L = (unsigned char)(SER_UBRR);

    // Enable double speed operation
    UCSR0A = (1 << U2X0);

    // Enable RX and TX
    // RXCIE0 needs to be enabled to handle RX interrupts
    UCSR0B = (1 << RXEN0) | (1 << TXEN0) | (1 << RXCIE0);

    // Set frame format to 8 data (Arduino default)
    UCSR0C = (1 << UCSZ00) | (1 << UCSZ01);

    sei();
}

unsigned char serial_read() {
    return UDR0;
}

bool serial_rx_available() {
    return (UCSR0A & (1 << RXC0));
}

bool serial_tx_available() {
    return (UCSR0A & (1 << UDRE0));
}

void serial_write_c(char data) {
    while (!serial_tx_available());

    UDR0 = data;
}

void serial_write_s(char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        serial_write_c(data[i]);
    }
}

void serial_write_n(int data) {
    char buf[8 * sizeof(int) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    do {
        char c = data % 10;
        data /= 10;

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(data);

    serial_write_s(str);
}

void serial_writeln_c(char data) {
    while (!serial_tx_available());
    UDR0 = data;
    while (!serial_tx_available());
    UDR0 = '\n';
}

void serial_writeln_s(char *data) {
    for (int i = 0; data[i] != '\0'; i++) {
        serial_write_c(data[i]);
    }

    serial_write_c('\n');
}

void serial_writeln_n(int data) {
    char buf[8 * sizeof(int) + 1]; // Assumes 8-bit chars plus zero byte.
    char *str = &buf[sizeof(buf) - 1];

    *str = '\0';

    do {
        char c = data % 10;
        data /= 10;

        *--str = c < 10 ? c + '0' : c + 'A' - 10;
    } while(data);

    serial_write_s(str);
    serial_write_c('\n');
}

bool process_serial() {
    // No need to check if RX is available because we use interrupts
    const unsigned char c = serial_read();

    if (rx_in_progress) {
        switch (c) {
            case '\r':
                break;
            case SER_END_CHAR:
                rx_buf[rx_pos] = '\0';
                rx_pos = 0;
                rx_in_progress = false;
                return true;
            default:
                rx_buf[rx_pos] = c;
                rx_pos = limit(++rx_pos, 0, SER_BUFS_SIZE - 1);
                break;
        }
    }
    else if (c == SER_START_CHAR) {
        rx_in_progress = true;
    }

    return false;
}

void send_handshake()
{
    // Response: ^!,@\n

    strcpy(tx_buf, "^!,@");
    serial_writeln_s(tx_buf);
}

void send_password()
{
    // Response: ^!,c,X,X,X\n

    int8_t tmp_p[3];
    char tmp_s[4];

    eeprom_get_password(tmp_p);

    strcpy(tx_buf, "^!,c,");

    // If password is not set, send 'n' to avoid sign issues
    strcat(tx_buf, tmp_p[0] == -1 ? "n" : itos(tmp_p[0], get_num_length(tmp_p[0]), tmp_s));

    strcat(tx_buf, ",");
    strcat(tx_buf, itos(tmp_p[1], get_num_length(tmp_p[1]), tmp_s));
    strcat(tx_buf, ",");
    strcat(tx_buf, itos(tmp_p[2], get_num_length(tmp_p[2]), tmp_s));

    serial_writeln_s(tx_buf);
}

void send_info()
{
    /*
       Response: ^!,i,S,HV,SV,D,M\n

       S = Serial number
       HV = Hardware version
       SV = Software version
       D = Default slot
       M = Maximum number of slots
    */

    char tmp_s[5];
    int16_t tmp_n;

    tmp_n = eeprom_get_serial();

    strcpy(tx_buf, "^!,i,");
    strcat(tx_buf, itos(tmp_n, get_num_length(tmp_n), tmp_s));
    strcat(tx_buf, ",");
    strcat(tx_buf, HW_VERSION);
    strcat(tx_buf, ",");
    strcat(tx_buf, SW_VERSION);
    strcat(tx_buf, ",");
    
    tmp_n = eeprom_get_default_slot();

    // If no default slot is set, send 'n' to avoid sign issues
    strcat(tx_buf, tmp_n == -1 ? "n" : itos(tmp_n, get_num_length(tmp_n), tmp_s));

    strcat(tx_buf, ",");
    strcat(tx_buf, itos(NUM_SLOTS, get_num_length(NUM_SLOTS), tmp_s));

    serial_writeln_s(tx_buf);
}

void send_slots()
{
    /*
       Response: ^!,n,X\n
                 loop X times:
                     ^!,s,SI,SN\n
                     loop 8 times:
                         ^!,p,PI,PN,M,F,D,P\n

       X = Number of slots to be sent
       SI = Slot index
       SN = Slot name
       PI = PWM index
       PN = PWM name
       M = Mode
       F = Frequency
       D = Duty cycle
       P = Phase
    */

    char tmp_s[EE_PWM_NAME_SIZE];
    int16_t tmp_n;

    tmp_n = eeprom_get_used_slots();

    strcpy(tx_buf, "^!,n,");
    strcat(tx_buf, itos(tmp_n, get_num_length(tmp_n), tmp_s));
    serial_writeln_s(tx_buf);

    slot_t to_send;

    for (int i = 0; i < tmp_n; i++)
    {
        eeprom_get_slot(i, &to_send);

        strcpy(tx_buf, "^!,s,");
        strcat(tx_buf, itos(i, get_num_length(i), tmp_s));
        strcat(tx_buf, ",");
        strcat(tx_buf, to_send.name);
        serial_writeln_s(tx_buf);

        for (int j = 0; j < NUM_PINS; j++)
        {
            strcpy(tx_buf, "^!,p,");
            strcat(tx_buf, itos(j, get_num_length(j), tmp_s));
            strcat(tx_buf, ",");
            strcat(tx_buf, to_send.pwms[j].name);
            strcat(tx_buf, ",");
            strcat(tx_buf, itos(to_send.pwms[j].mode,
                   get_num_length(to_send.pwms[j].mode), tmp_s));
            strcat(tx_buf, ",");
            strcat(tx_buf, itos(to_send.pwms[j].frq,
                   get_num_length(to_send.pwms[j].frq), tmp_s));
            strcat(tx_buf, ",");
            strcat(tx_buf, itos(to_send.pwms[j].dty,
                   get_num_length(to_send.pwms[j].dty), tmp_s));
            strcat(tx_buf, ",");
            strcat(tx_buf, itos(to_send.pwms[j].phs,
                   get_num_length(to_send.pwms[j].phs), tmp_s));
            serial_writeln_s(tx_buf);
        }
    }
}

uint8_t rx_num_slots;
uint8_t rx_slot_idx;
slot_t rx_slots[NUM_SLOTS];
uint8_t rx_pwm_idx;
pwm_t rx_pwm;

void process_data()
{
    char *idx;

    idx = strtok(rx_buf, ",");

    if (idx[0] == '?')  // App is asking for something
    {
        idx = strtok(NULL, "\n");

        if (strlen(idx) > 1) { serial_write_s("^!,ERR3\n"); return; }
        
        switch (idx[0])
        {
            case '@': send_handshake(); break;  // Initial handshake
            case 'c': send_password(); break;  // Password
            case 'i': send_info(); break;  // Device info
            case 's': send_slots(); break;  // Slots
            default: serial_write_s("^!,ERR2\n"); return;
        }
    }
    else if (idx[0] == '!')  // App wants to change one of the settings
    {
        int8_t tmp_p[3];
        uint8_t tmp_n;

        idx = strtok(NULL, ",");

        if (strlen(idx) > 1) { serial_write_s("^!,ERR3\n"); return; }

        switch (idx[0])
        {
            case 'c':  // Password
                idx = strtok(NULL, ",");
                tmp_p[0] = atoi(idx);
                idx = strtok(NULL, ",");
                tmp_p[1] = atoi(idx);
                idx = strtok(NULL, ",");
                tmp_p[2] = atoi(idx);

                eeprom_set_password(tmp_p);

                break;

            case 'd':  // Default slot
                idx = strtok(NULL, "\n");
                tmp_n = atoi(idx);
                
                eeprom_set_default_slot(tmp_n);

                break;

            case 'n':  // Number of slots about to be sent
                idx = strtok(NULL, "\n");
                rx_num_slots = atoi(idx);

                eeprom_delete_all_slots();
                unload_active_slot();

                break;

            case 's':  // Slot index and name
                idx = strtok(NULL, ",");
                rx_slot_idx = atoi(idx);
                idx = strtok(NULL, "\n");
                strcpy(rx_slots[rx_slot_idx].name, idx);

                break;

            case 'p':  // PWM
                idx = strtok(NULL, ",");
                rx_pwm_idx = atoi(idx);
                idx = strtok(NULL, ",");
                strcpy(rx_slots[rx_slot_idx].pwms[rx_pwm_idx].name, idx);
                idx = strtok(NULL, ",");
                rx_slots[rx_slot_idx].pwms[rx_pwm_idx].mode = atoi(idx);
                idx = strtok(NULL, ",");
                rx_slots[rx_slot_idx].pwms[rx_pwm_idx].frq = atoi(idx);
                idx = strtok(NULL, ",");
                rx_slots[rx_slot_idx].pwms[rx_pwm_idx].dty = atoi(idx);
                idx = strtok(NULL, "\n");
                rx_slots[rx_slot_idx].pwms[rx_pwm_idx].phs = atoi(idx);

                // Last PWM, save slot in EEPROM
                if (rx_slot_idx == (rx_num_slots - 1) && (rx_pwm_idx == (NUM_PINS - 1))) {
                    for (int i = 0; i < rx_num_slots; i++) {
                        eeprom_new_slot(&rx_slots[i]);
                    }
                }

                break;

            default: serial_write_s("^!,ERR2\n"); return;
        }
    }
    else { serial_write_s("^!,ERR1\n"); return; }  // Not handled
}