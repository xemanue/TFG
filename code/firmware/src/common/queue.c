/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @brief Very basic circular queue library
 */

#include "common/queue.h"
#include "sys/io/serial_control.h"
#include "common/config.h"

bool queue_empty(queue_t *q) {
    return q->r_ptr == q->w_ptr;
}

bool queue_full(queue_t *q) {
    return q->r_ptr == ((q->w_ptr + 1) % EV_QUEUE_SIZE);
}

bool queue_pop(queue_t *q, int *data) {
    if (queue_empty(q)) {
        return false;
    }
    
    *data = q->buf[q->r_ptr];
    q->r_ptr = (q->r_ptr + 1) % EV_QUEUE_SIZE;
    
    return true;
}

bool queue_push(queue_t *q, int data) {
    if (queue_full(q)) {
        return false;
    }

    q->buf[q->w_ptr] = data;
    q->w_ptr = (q->w_ptr + 1) % EV_QUEUE_SIZE;
    
    return true;
}