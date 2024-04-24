#include "crfb_utils.h"

void crfb_int_to_little(int* value) {
    *value = ((*value >> 24) & 0xff) |
        ((*value << 8) & 0xff0000) |
        ((*value >> 8) & 0xff00) |
        ((*value << 24) & 0xff000000);
}

void crfb_uint_to_little(unsigned int* value) {
    *value = ((*value >> 24) & 0xff) |
        ((*value << 8) & 0xff0000) |
        ((*value >> 8) & 0xff00) |
        ((*value << 24) & 0xff000000);
}

void crfb_short_to_little(short* value) {
    *value = ((*value & 0xFF00) >> 8) | ((*value & 0x00FF) << 8);
}

void crfb_ushort_to_little(unsigned short* value) {
    *value = ((*value & 0xFF00) >> 8) | ((*value & 0x00FF) << 8);
}
