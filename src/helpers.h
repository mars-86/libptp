#ifndef __PICTURE_TRANSFER_PROTOCOL_HELPERS_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_HELPERS_INCLUDED_H__

#include "types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

inline void ptp_cpy_string_to_stream(uint8_t* dest, ptp_string_t src)
{
    int i;
    uint8_t* __src = (uint8_t*)src.StringChars;
    for (i = 0; i < src.NumChars; ++i) {
        uint8_t __c = *__src;
        putc(__c, stdout);
        *dest++ = *__src++;
        *dest++ = 0;
    }

    putc('\n', stdout);
    *dest++ = 0;
    *dest = 0;
}

inline int ptp_alloc_string_from_charp(const char* str, ptp_string_t* pstr, int empty_null)
{
    if (str) {
        size_t __len = strlen(str);
        if (__len) {
            // we added 1 for the null terminated character
            pstr->NumChars = __len + 1;
            if (!(pstr->StringChars = (uint16_t*)malloc(pstr->NumChars * PTP_STRING_CHAR_SIZE)))
                return 1;

            memset(pstr->StringChars, 0, pstr->NumChars);
            memcpy(pstr->StringChars, str, pstr->NumChars);

            return 0;
        }
    }

    if (empty_null) {
        // we fill empty string with null character
        pstr->NumChars = 1;
        if (!(pstr->StringChars = (uint16_t*)malloc(pstr->NumChars * PTP_STRING_CHAR_SIZE)))
            return 1;
        memset(pstr->StringChars, 0, pstr->NumChars);
    } else {
        pstr->NumChars = 0;
        pstr->StringChars = NULL;
    }

    return 0;
}

#define ALLOC_STRING(di_prop, stream, offset, size)                                                       \
    do {                                                                                                  \
        if (!(di_prop.StringChars = (uint16_t*)malloc(*(stream + offset) * size)))                        \
            continue;                                                                                     \
        di_prop.NumChars = *(stream + offset);                                                            \
        memcpy(di_prop.StringChars, stream + offset + sizeof(di_prop.NumChars), di_prop.NumChars * size); \
        offset += ((di_prop.NumChars ? di_prop.NumChars * size : 0) + sizeof(di_prop.NumChars));          \
    } while (0)

#define ALLOC_ARRAY(di_prop, stream, offset, elsize, size)                                                     \
    do {                                                                                                       \
        if (!(di_prop.ArrayEntry = (elsize*)malloc(*(stream + offset) * size)))                                \
            continue;                                                                                          \
        di_prop.NumElements = PTP_ARRAY_NUM_ELEM_MASK(stream + offset);                                        \
        memcpy(di_prop.ArrayEntry, stream + offset + sizeof(di_prop.NumElements), di_prop.NumElements * size); \
        offset += ((di_prop.NumElements ? di_prop.NumElements * size : 0) + sizeof(di_prop.NumElements));      \
    } while (0)

#endif // __PICTURE_TRANSFER_PROTOCOL_HELPERS_INCLUDED_H__
