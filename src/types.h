#ifndef __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__

#include <stdint.h>

struct ptp_string {
    uint8_t NumChars;
    uint16_t* StringChars; // each char is 2 bytes
};

typedef struct ptp_string ptp_string_t;

#endif // __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__
