#ifndef __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__

#include <stdint.h>

#define PTP_OPERATION_CODE_SIZE 2
#define PTP_RESPONSE_CODE_SIZE 2
#define PTP_EVENT_CODE_SIZE 2
#define PTP_DEVICE_PROP_CODE_SIZE 2
#define PTP_OBJECT_FORMAT_CODE_SIZE 2
#define PTP_STORAGE_ID_SIZE 4
#define PTP_OBJECT_HANDLE_SIZE 4

#define PTP_STRING_CHAR_SIZE 2

#define PTP_ARRAY_NUM_ELEM_MASK(stream) ((*(stream + 3) << 24) | (*(stream + 2) << 16) | (*(stream + 1) << 8) | (*(stream)))

struct ptp_string {
    uint8_t NumChars;
    uint16_t* StringChars; // each char is 2 bytes
} __attribute__((packed));

typedef struct ptp_string ptp_string_t;

struct ptp_string_datetime {
    uint8_t NumChars;
    uint8_t YYYY[4]; // year
    uint8_t MM[2]; // month
    uint8_t DD[2]; // day
    uint8_t T; // T
    uint8_t hh[2]; // hour
    uint8_t mm[2]; // min
    uint8_t ss[4]; // sec + sec tenths (optional)
    uint8_t TZ[5]; // timezone (optional)
};

typedef struct ptp_string_datetime ptp_string_datetime_t;

struct ptp_array {
    uint32_t NumElements;
    void* ArrayEntry;
} __attribute__((packed));

typedef struct ptp_array ptp_array_t;

#endif // __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__
