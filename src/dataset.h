#ifndef __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__

#include "types.h"
#include <stdint.h>
#include <stdio.h>

#define PTP_DEVICE_FUNCTIONAL_MODE_STANDARD 0x0000
#define PTP_DEVICE_FUNCTIONAL_MODE_SLEEP_STATE 0x0001
#define PTP_DEVICE_RESET_PROP_VALUES_ALL 0xFFFFFFFF

// prop codes
#define PTP_DEVICE_PROP_CODE_UNDEFINED 0x5000
#define PTP_DEVICE_PROP_CODE_BATTERY_LEVEL 0x5001
#define PTP_DEVICE_PROP_CODE_FUNCTIONAL_MODE 0x5002
#define PTP_DEVICE_PROP_CODE_IMAGE_SIZE 0x5003
#define PTP_DEVICE_PROP_CODE_COMPRESSION_SETTING 0x5004
#define PTP_DEVICE_PROP_CODE_WHITE_BALANCE 0x5005
#define PTP_DEVICE_PROP_CODE_RGB_GAIN 0x5006
#define PTP_DEVICE_PROP_CODE_F_NUMBER 0x5007
#define PTP_DEVICE_PROP_CODE_FOCAL_LENGTH 0x5008
#define PTP_DEVICE_PROP_CODE_FOCUS_DISTANCE 0x5009
#define PTP_DEVICE_PROP_CODE_FOCUS_MODE 0x500A
#define PTP_DEVICE_PROP_CODE_EXPOSURE_METERING_MODE 0x500B
#define PTP_DEVICE_PROP_CODE_FLASH_MODE 0x500C
#define PTP_DEVICE_PROP_CODE_EXPOSURE_TIME 0x500D
#define PTP_DEVICE_PROP_CODE_EXPOSURE_PROGRAM_MODE 0x500E
#define PTP_DEVICE_PROP_CODE_EXPOSURE_INDEX 0x500F
#define PTP_DEVICE_PROP_CODE_EXPOSURE_BIAS_COMPENSATION 0x5010
#define PTP_DEVICE_PROP_CODE_DATE_TIME 0x5011
#define PTP_DEVICE_PROP_CODE_CAPTURE_DELAY 0x5012
#define PTP_DEVICE_PROP_CODE_STILL_CAPTURE_MODE 0x5013
#define PTP_DEVICE_PROP_CODE_CONTRAST 0x5014
#define PTP_DEVICE_PROP_CODE_SHARPNESS 0x5015
#define PTP_DEVICE_PROP_CODE_DIGITAL_ZOOM 0x5016
#define PTP_DEVICE_PROP_CODE_EFFECT_MODE 0x5017
#define PTP_DEVICE_PROP_CODE_BURST_NUMBER 0x5018
#define PTP_DEVICE_PROP_CODE_BURST_INTERVAL 0x5019
#define PTP_DEVICE_PROP_CODE_TIMELAPSE_NUMBER 0x501A
#define PTP_DEVICE_PROP_CODE_TIMELAPSE_INTERVAL 0x501B
#define PTP_DEVICE_PROP_CODE_FOCUS_METERING_MODE 0x501C
#define PTP_DEVICE_PROP_CODE_UPLOAD_URL 0x501D
#define PTP_DEVICE_PROP_CODE_ARTIST 0x501E
#define PTP_DEVICE_PROP_CODE_COPYRIGHT_INFO 0x501F

// prop codes data types
#define PTP_DEVICE_PROP_FORM_NONE 0x00
#define PTP_DEVICE_PROP_FORM_RANGE 0x01
#define PTP_DEVICE_PROP_FORM_ENUM 0x02

// TODO review this packed struct
struct ptp_dev_prop_range {
    void* MinimumValue;
    void* MaximumValue;
    void* StepSize;
};

// TODO review this packed struct
struct ptp_dev_prop_enum {
    uint16_t NumberOfValues;
    void* SupportedValue;
};

/*
 * Device Property Describing Dataset
 *
 * Device Property Code
 * DataType
 * GetSet
 * Factory Default
 * Current Value
 * Form Flag
 * FORM
 */
struct ptp_dev_prop_desc {
    uint16_t DevicePropertyCode;
    uint16_t DataType;
    uint8_t GetSet;
    uint8_t FormFlag; // re-arranged form flag for less padding
    void* FactoryDefaultValue;
    void* DefaultValue;
    void* FORM;
};

struct ptp_dev_prop_dataset {
    uint8_t form;
    struct ptp_dev_prop_range* prop_range;
    struct ptp_dev_prop_enum* prop_enum;
};

// TODO review this packed struct
struct ptp_device_info {
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

struct ptp_device_info* ptp_alloc_device_info(const uint8_t* stream);

void ptp_free_device_info(const struct ptp_device_info* dev_info);

struct ptp_dev_prop_desc* ptp_alloc_dev_prop_desc(const uint8_t* stream);

void ptp_free_dev_prop_desc(const struct ptp_dev_prop_desc* dev_prop_desc);

#endif // __PICTURE_TRANSFER_PROTOCOL_DATASET_INCLUDED_H__
