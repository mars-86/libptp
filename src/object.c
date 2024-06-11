#include "object.h"
#include "ptp.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

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
