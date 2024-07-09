#include "dataset.h"
#include "helpers.h"
#include "ptp.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

#define STATIC_DEV_PROP_DESCRIPTION(prop, stream, offset, datatype) \
    prop = (datatype)(*(stream + offset));                          \
    offset += sizeof(datatype)

#define ALLOC_DTS(prop, stream, offset, n, datatype) \
    prop = (datatype*)malloc(n * sizeof(datatype));  \
    prop = (datatype*)((stream + offset));           \
    offset += sizeof(datatype)

#define DEV_PROP_FORM(prop, stream, offset, form_flag, datatype)                                         \
    do {                                                                                                 \
        if (__prop_desc->FormFlag == PTP_DEVICE_PROP_FORM_RANGE) {                                       \
            prop = (struct ptp_dev_prop_range*)malloc(sizeof(struct ptp_dev_prop_range));                \
            struct ptp_dev_prop_range* __dpr = __prop_desc->FORM;                                        \
            __dpr->MinimumValue = *(stream + __st_offset);                                               \
            __dpr->MaximumValue = *(stream + __st_offset);                                               \
            __dpr->StepSize = *(stream + __st_offset);                                                   \
        } else if (__prop_desc->FormFlag == PTP_DEVICE_PROP_FORM_ENUM) {                                 \
            prop = (struct ptp_dev_prop_enum*)malloc(sizeof(struct ptp_dev_prop_enum));                  \
            struct ptp_dev_prop_enum* __dpe = __prop_desc->FORM;                                         \
            __dpe->NumberOfValues = (datatype)(*(stream + offset));                                      \
            offset += sizeof(__dpe->NumberOfValues);                                                     \
            __dpe->SupportedValue = (datatype*)(__dpe->NumberOfValues * sizeof(datatype));               \
            if (__dpe->SupportedValue)                                                                   \
                memcpy(__dpe->SupportedValue, stream + offset, __dpe->NumberOfValues * sizeof(datatype)) \
        }                                                                                                \
        while (0)

#define SET_DEV_PROP_DESCRIPTION(dev_prop_desc, stream, offset, datatype)                         \
    do {                                                                                          \
        STATIC_DEV_PROP_DESCRIPTION(dev_prop_desc->DevicePropertyCode, stream, offset, uint16_t); \
        STATIC_DEV_PROP_DESCRIPTION(dev_prop_desc->DataType, stream, offset, uint16_t);           \
        STATIC_DEV_PROP_DESCRIPTION(dev_prop_desc->GetSet, stream, offset, uint8_t);              \
        ALLOC_DTS(dev_prop_desc->FactoryDefaultValue, stream, offset, 1, datatype);               \
        ALLOC_DTS(dev_prop_desc->DefaultValue, stream, offset, 1, datatype);                      \
        STATIC_DEV_PROP_DESCRIPTION(dev_prop_desc->FormFlag, stream, offset, uint8_t);            \
    } while (0)

static inline int __alloc_dev_prop_desc(struct ptp_dev_prop_desc** dpd, const uint8_t* stream, size_t offset)
{
    struct ptp_dev_prop_desc* __dpd = *dpd;
    switch (__dpd->DataType) {
    case PTP_DATA_TYPE_INT8:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, int8_t);
        break;
    case PTP_DATA_TYPE_UINT8:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, uint8_t);
        break;
    case PTP_DATA_TYPE_INT16:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, int16_t);
        break;
    case PTP_DATA_TYPE_UINT16:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, uint16_t);
        break;
    case PTP_DATA_TYPE_INT32:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, int32_t);
        break;
    case PTP_DATA_TYPE_UINT32:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, uint32_t);
        break;
    case PTP_DATA_TYPE_INT64:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, int64_t);
        break;
    case PTP_DATA_TYPE_UINT64:
        SET_DEV_PROP_DESCRIPTION(__dpd, stream, offset, uint64_t);
        break;
    default:;
    }

    if ((!__dpd->FactoryDefaultValue) || (!__dpd->DefaultValue) || (!__dpd->FORM))
        return 1;

    if (__dpd->FormFlag == PTP_DEVICE_PROP_FORM_RANGE) {
        struct ptp_dev_prop_range* __dpr = (struct ptp_dev_prop_range*)__dpd->FORM;
        if ((!__dpr->MinimumValue) || (!__dpr->MaximumValue) || (!__dpr->StepSize))
            return 1;
    } else if (__dpd->FormFlag == PTP_DEVICE_PROP_FORM_ENUM) {
        struct ptp_dev_prop_enum* __dpe = (struct ptp_dev_prop_enum*)__dpd->FORM;
        if (!__dpe->SupportedValue)
            return 1;
    }

    return 0;
}

struct ptp_device_info* ptp_alloc_device_info(const uint8_t* stream)
{
    struct ptp_device_info* __di = (struct ptp_device_info*)malloc(sizeof(struct ptp_device_info));

    if (!__di)
        return NULL;

    int __st_offset = PTP_CONTAINER_DATA_OFFSET;

    memset(__di, 0, sizeof(struct ptp_device_info));

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
    ptp_free_device_info(__di);
    return NULL;
}

void ptp_free_device_info(const struct ptp_device_info* di)
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

    free((struct ptp_device_info*)di);
}

struct ptp_dev_prop_desc* ptp_alloc_dev_prop_desc(const uint8_t* stream)
{
    struct ptp_dev_prop_desc* __prop_desc = (struct ptp_dev_prop_desc*)malloc(sizeof(struct ptp_dev_prop_desc));

    if (!__prop_desc)
        return NULL;

    size_t __st_offset = PTP_CONTAINER_DATA_OFFSET;

    __alloc_dev_prop_desc(&__prop_desc, stream, __st_offset);

    return __prop_desc;
}

void ptp_free_dev_prop_desc(const struct ptp_dev_prop_desc* dev_prop_desc)
{
    if (dev_prop_desc->FactoryDefaultValue)
        free(dev_prop_desc->FactoryDefaultValue);

    if (dev_prop_desc->DefaultValue)
        free(dev_prop_desc->DefaultValue);

    if (dev_prop_desc->FORM)
        free(dev_prop_desc->FORM);

    if (dev_prop_desc)
        free((struct ptp_dev_prop_desc*)dev_prop_desc);
}
