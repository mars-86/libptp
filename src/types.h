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

#define PTP_DATA_TYPE_UNDEFINED 0x0000 // Undefined
#define PTP_DATA_TYPE_INT8 0x0001 // INT8 Signed 8 bit integer
#define PTP_DATA_TYPE_UINT8 0x0002 // UINT8 Unsigned 8 bit integer
#define PTP_DATA_TYPE_INT16 0x0003 // INT16 Signed 16 bit integer
#define PTP_DATA_TYPE_UINT16 0x0004 // UINT16 Unsigned 16 bit integer
#define PTP_DATA_TYPE_INT32 0x0005 // INT32 Signed 32 bit integer
#define PTP_DATA_TYPE_UINT32 0x0006 // UINT32 Unsigned 32 bit integer
#define PTP_DATA_TYPE_INT64 0x0007 // INT64 Signed 64 bit integer
#define PTP_DATA_TYPE_UINT64 0x0008 // UINT64 Unsigned 64 bit integer
#define PTP_DATA_TYPE_INT128 0x0009 // INT128 Signed 128 bit integer
#define PTP_DATA_TYPE_UINT128 0x000A // UINT128 Unsigned 128 bit integer
#define PTP_DATA_TYPE_AINT8 0x4001 // AINT8 Array of Signed 8 bit integers
#define PTP_DATA_TYPE_AUINT8 0x4002 // AUINT8 Array of Unsigned 8 bit integers
#define PTP_DATA_TYPE_AINT16 0x4003 // AINT16 Array of Signed 16 bit integers
#define PTP_DATA_TYPE_AUINT16 0x4004 // AUINT16 Array of Unsigned 16 bit integers
#define PTP_DATA_TYPE_AINT32 0x4005 // AINT32 Array of Signed 32 bit integers
#define PTP_DATA_TYPE_AUINT32 0x4006 // AUINT32 Array of Unsigned 32 bit integers
#define PTP_DATA_TYPE_AINT64 0x4007 // AINT64 Array of Signed 64 bit integers
#define PTP_DATA_TYPE_AUINT64 0x4008 // AUINT64 Array of Unsigned 64 bit integers
#define PTP_DATA_TYPE_AINT128 0x4009 // AINT128 Array of Signed 128 bit integers
#define PTP_DATA_TYPE_AUINT128 0x400A // AUINT128 Array of Unsigned 128 bit integers
#define PTP_DATA_TYPE_STR 0xFFFF // STR Variable-length Unicode String

// TODO review this packed struct
struct ptp_string {
    uint16_t* StringChars; // each char is 2 bytes
    uint8_t NumChars;
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

// TODO review this packed struct
struct ptp_array {
    void* ArrayEntry;
    uint32_t NumElements;
} __attribute__((packed));

typedef struct ptp_array ptp_array_t;

#endif // __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__
