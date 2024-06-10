#include "dataset.h"
#include "ptp.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

#define VENDOR_EXTENSION_DESC_POS 20

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

struct device_info* alloc_device_info(const uint8_t* stream, size_t len)
{
    struct device_info* __di = (struct device_info*)malloc(sizeof(struct device_info));

    if (!__di)
        return NULL;

    int __st_offset = PTP_CONTAINER_DATA_OFFSET;

    memset(__di, 0, sizeof(struct device_info));

    // copy first 8 bytes StandardVersion, VendorExtensionID, VendorExtensionVersion;
    memcpy(__di, stream + __st_offset, 8);
    __st_offset += 8;

    ALLOC_STRING(__di->VendorExtensionDesc, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__di->VendorExtensionDesc.StringChars)
        goto err;

    __di->FunctionalMode = (uint16_t)(*(stream + __st_offset));
    __st_offset += sizeof(__di->FunctionalMode);

    ALLOC_ARRAY(__di->OperationsSupported, stream, __st_offset, uint16_t, PTP_OPERATION_CODE_SIZE);
    if (!__di->OperationsSupported.ArrayEntry)
        goto err;

    ALLOC_ARRAY(__di->EventsSupported, stream, __st_offset, uint16_t, PTP_EVENT_CODE_SIZE);
    if (!__di->EventsSupported.ArrayEntry)
        goto err;

    ALLOC_ARRAY(__di->DevicePropertiesSupported, stream, __st_offset, uint16_t, PTP_DEVICE_PROP_CODE_SIZE);
    if (!__di->DevicePropertiesSupported.ArrayEntry)
        goto err;

    ALLOC_ARRAY(__di->CaptureFormats, stream, __st_offset, uint16_t, PTP_OBJECT_FORMAT_CODE_SIZE);
    if (!__di->CaptureFormats.ArrayEntry)
        goto err;

    ALLOC_ARRAY(__di->ImageFormats, stream, __st_offset, uint16_t, PTP_OBJECT_FORMAT_CODE_SIZE);
    if (!__di->ImageFormats.ArrayEntry)
        goto err;

    ALLOC_STRING(__di->Manufacturer, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__di->Manufacturer.StringChars)
        goto err;

    ALLOC_STRING(__di->Model, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__di->Model.StringChars)
        goto err;

    ALLOC_STRING(__di->DeviceVersion, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__di->DeviceVersion.StringChars)
        goto err;

    ALLOC_STRING(__di->SerialNumber, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__di->SerialNumber.StringChars)
        goto err;

    return __di;

err:
    free_device_info(__di);
    return NULL;
}

void free_device_info(const struct device_info* di)
{
    if (di->VendorExtensionDesc.StringChars)
        free(di->VendorExtensionDesc.StringChars);

    if (di->OperationsSupported.ArrayEntry)
        free(di->OperationsSupported.ArrayEntry);

    if (di->EventsSupported.ArrayEntry)
        free(di->EventsSupported.ArrayEntry);

    if (di->DevicePropertiesSupported.ArrayEntry)
        free(di->DevicePropertiesSupported.ArrayEntry);

    if (di->CaptureFormats.ArrayEntry)
        free(di->CaptureFormats.ArrayEntry);

    if (di->ImageFormats.ArrayEntry)
        free(di->ImageFormats.ArrayEntry);

    if (di->Manufacturer.StringChars)
        free(di->Manufacturer.StringChars);

    if (di->Model.StringChars)
        free(di->Model.StringChars);

    if (di->DeviceVersion.StringChars)
        free(di->DeviceVersion.StringChars);

    if (di->SerialNumber.StringChars)
        free(di->SerialNumber.StringChars);
}
