/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 * @brief Appearance of system information menus
 */

#include "sys/menu/sys_menu.h"

#include "common/config.h"
#include "sys/menu_control.h"
#include "sys/lcd_screen.h"

void info_menu() {
    lcd_clrscr();

    lcd_gotoxy(2, 0);
    lcd_puts("GranaSAT PWM BOX");

    lcd_gotoxy(5, 1);
    lcd_puts("HW-Ver:");
    lcd_puts(HW_VERSION);
    
    lcd_gotoxy(5, 2);
    lcd_puts("SW-Ver:");
    lcd_puts(SW_VERSION);
    
    lcd_gotoxy(2, 3);
    lcd_puts("granasat.ugr.es");
}

void warn_menu() {
    lcd_clrscr();

    lcd_gotoxy(5, 1);
    lcd_puts("ATENCION");

    lcd_gotoxy(2, 2);
    lcd_puts("Memoria llena ");
    lcd_putc(SAD_FACE);
}