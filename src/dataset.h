#ifndef __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__

#include "types.h"
#include <stdint.h>
#include <stdio.h>

#define PTP_DEVICE_FUNCTIONAL_MODE_STANDARD 0x0000
#define PTP_DEVICE_FUNCTIONAL_MODE_SLEEP_STATE 0x0001
#define PTP_DEVICE_RESET_PROP_VALUES_ALL 0xFFFFFFFF

struct device_info {
    uint16_t StandardVersion;
    uint32_t VendorExtensionID;
    uint16_t VendorExtensionVersion;
    ptp_string_t VendorExtensionDesc;
    uint16_t FunctionalMode;
    ptp_array_t OperationsSupported;
    ptp_array_t EventsSupported;
    ptp_array_t DevicePropertiesSupported;
    ptp_array_t CaptureFormats;
    ptp_array_t ImageFormats;
    ptp_string_t Manufacturer;
    ptp_string_t Model;
    ptp_string_t DeviceVersion;
    ptp_string_t SerialNumber;
} __attribute__((packed));

struct device_info* ptp_alloc_device_info(const uint8_t* stream);

void ptp_free_device_info(const struct device_info* dev_info);

#endif // __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__
