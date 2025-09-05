/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 * 
 * @brief Basic event handling
 */

#include "sys/event_control.h"

#include "sys/io/serial_control.h"
#include "sys/menu_control.h"
#include "sys/menu/slow_menu.h"
#include "common/queue.h"
#include "common/config.h"

void event_handler(event_t ev) {
    switch (ev) {
        case EV_ROT_L:
            scroll(EV_ROT_L);
            break;
        case EV_ROT_R:
            scroll(EV_ROT_R);
            break;
        case EV_ROT_P:
            button_press();
            break;
        case EV_ROT_H:
            if (get_current_menu() == SLOW_MENU) change_menu(LIST_MENU);
            else change_menu(SLOW_MENU);
            break;
        case EV_SERIAL:
            process_data();
            break;
        case EV_NONE:
            break;
    }
}