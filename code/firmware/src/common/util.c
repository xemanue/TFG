/**
 * @author Jose Manuel García Cazorla <jmgarcaz@correo.ugr.es>
 * @copyright (C) GranaSAT, GNU General Public License Version 3
 *
 * @brief Common utility functions
 */

#include "common/util.h"

char *itos(int num, int len, char *buf) {
    char aux;

    num = (num > 0) ? num : -1 * num;
    
    for (int i = 0; i < len; i++) {
        buf[i] = num % 10 + '0';
        num = num / 10;
    }

    for (int i = 0; i < len / 2; i++) {
        aux = buf[i];
        buf[i] = buf[len - i - 1];
        buf[len - i - 1] = aux;
    }

    buf[len] = '\0';

    return buf;
}

int wrap(int num, int min, int max) {
    if (num < min) return max;
    if (num > max) return min;
    return num;
}

int wrap_hit(int num, int min, int max, bool *min_hit, bool *max_hit) {
    if (num < min) *min_hit = true;
    else if (num > max) *max_hit = true;
    else { *min_hit = false; *max_hit = false; }

    return wrap(num, min, max);
}

int limit(int num, int min, int max) {
    if (num < min) return min;
    if (num > max) return max;
    return num;
}

int limit_hit(int num, int min, int max, bool *min_hit, bool *max_hit) {
    if (num < min) *min_hit = true;
    else if (num > max) *max_hit = true;
    else { *min_hit = false; *max_hit = false; }

    return limit(num, min, max);
}

int get_num_length(int num) {
    if (num < 10) return 1;
    else if (10 <= num && num < 100) return 2;
    else if (100 <= num && num < 1000)return 3;
    else return 4;
}