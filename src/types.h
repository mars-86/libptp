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

struct ptp_string {
    uint8_t NumChars;
    uint16_t* StringChars; // each char is 2 bytes
};

typedef struct ptp_string ptp_string_t;

struct ptp_array {
    uint32_t NumElements;
    void* ArrayEntry;
};

typedef struct ptp_array ptp_array_t;

#endif // __PICTURE_TRANSFER_PROTOCOL_TYPES_INCLUDED_H__
