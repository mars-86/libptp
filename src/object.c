#include "object.h"
#include "helpers.h"
#include "ptp.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

struct object_info* alloc_object_info(const uint8_t* stream)
{
    struct object_info* __oi = (struct object_info*)malloc(sizeof(struct object_info));

    if (!__oi)
        return NULL;

    int __st_offset = PTP_CONTAINER_DATA_OFFSET;

    memset(__oi, 0, sizeof(struct object_info));

    // copy first 52 bytes StorageType FilesystemType AccessCapability MaxCapacity FreeSpaceInBytes FreeSpaceInImages
    memcpy(__oi, stream + __st_offset, 52);
    __st_offset += 52;

    ALLOC_STRING(__oi->Filename, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__oi->Filename.StringChars)
        goto err;

    ALLOC_STRING(__oi->CaptureDate, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__oi->CaptureDate.StringChars)
        goto err;

    ALLOC_STRING(__oi->ModificationDate, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__oi->ModificationDate.StringChars)
        goto err;

    ALLOC_STRING(__oi->Keywords, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__oi->Keywords.StringChars)
        goto err;

    return __oi;

err:
    free_object_info(__oi);
    return NULL;
}

void free_object_info(const struct object_info* oi)
{
    if (oi->Filename.StringChars)
        free(oi->Filename.StringChars);

    if (oi->CaptureDate.StringChars)
        free(oi->CaptureDate.StringChars);

    if (oi->ModificationDate.StringChars)
        free(oi->ModificationDate.StringChars);

    if (oi->Keywords.StringChars)
        free(oi->Keywords.StringChars);

    free((struct object_info*)oi);
}

ptp_array_t* alloc_object_handle_array(const uint8_t* stream, size_t len)
{
    ptp_array_t* __sia = (ptp_array_t*)malloc(sizeof(ptp_array_t));

    if (!__sia)
        return NULL;

    int __st_offset = PTP_CONTAINER_DATA_OFFSET;

    __sia->NumElements = (len - PTP_CONTAINER_DATA_OFFSET) / sizeof(ptp_object_handle);
    __sia->ArrayEntry = (ptp_object_handle*)malloc(__sia->NumElements * sizeof(ptp_object_handle));

    if (!__sia->ArrayEntry) {
        free((ptp_array_t*)__sia);
        return NULL;
    }

    memcpy(__sia->ArrayEntry, stream + __st_offset, __sia->NumElements * sizeof(ptp_object_handle));

    return __sia;
}

void free_object_handle_array(const ptp_array_t* oa)
{
    if (oa->ArrayEntry)
        free(oa->ArrayEntry);

    free((ptp_array_t*)oa);
}
