#ifndef __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__

#include <cstdint>
#include <stdint.h>

#define PTP_DEVICE_FUNCTIONAL_MODE_STANDARD 0x0000
#define PTP_DEVICE_FUNCTIONAL_MODE_SLEEP_STATE 0x0001
#define PTP_DEVICE_RESET_PROP_VALUES_ALL 0xFFFFFFFF

struct device_info {
    uint16_t StandardVersion;
    uint32_t VendorExtensionID;
    uint16_t VendorExtensionVersion;
    uint16_t* VendorExtensionDesc; // each char is 2 bytes
    uint16_t FunctionalMode;
    uint16_t* OperationsSupported;
    uint16_t* EventsSupported;
    uint16_t* DevicePropertiesSupported;
    uint8_t* CaptureFormats;
    uint32_t* ImageFormats;
    uint16_t* Manufacturer; // each char is 2 bytes
    uint16_t* Model; // each char is 2 bytes
    uint16_t* DeviceVersion; // each char is 2 bytes
    uint16_t* SerialNumber; // each char is 2 bytes
} __attribute__((packed));

device_info* alloc_device_info(const uint8_t* stream);

void free_device_info(const device_info* dev_info);

#endif // __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__
