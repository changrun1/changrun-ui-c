
#include "tools.h"


int utf8_char_length(const char *c) {
    if ((c[0] & 0x80) == 0) {
        return 1; // 1-byte (ASCII)
    } else if ((c[0] & 0xE0) == 0xC0) {
        return 2; // 2-byte
    } else if ((c[0] & 0xF0) == 0xE0) {
        return 3; // 3-byte
    } else if ((c[0] & 0xF8) == 0xF0) {
        return 4; // 4-byte
    }
    return -1; // 
}

