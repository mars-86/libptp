#include "storage.h"
#include "helpers.h"
#include "ptp.h"
#include "types.h"
#include <stdlib.h>
#include <string.h>

struct storage_info* alloc_storage_info(const uint8_t* stream)
{
    struct storage_info* __si = (struct storage_info*)malloc(sizeof(struct storage_info));

    if (!__si)
        return NULL;

    int __st_offset = PTP_CONTAINER_DATA_OFFSET;

    memset(__si, 0, sizeof(struct storage_info));

    // copy first 26 bytes StorageType FilesystemType AccessCapability MaxCapacity FreeSpaceInBytes FreeSpaceInImages
    memcpy(__si, stream + __st_offset, 26);
    __st_offset += 26;

    ALLOC_STRING(__si->StorageDescription, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__si->StorageDescription.StringChars)
        goto err;

    ALLOC_STRING(__si->VolumeLabel, stream, __st_offset, PTP_STRING_CHAR_SIZE);
    if (!__si->VolumeLabel.StringChars)
        goto err;

    return __si;

err:
    free_storage_info(__si);
    return NULL;
}

void free_storage_info(const struct storage_info* si)
{
    if (si->StorageDescription.StringChars)
        free(si->StorageDescription.StringChars);

    if (si->VolumeLabel.StringChars)
        free(si->VolumeLabel.StringChars);

    free((struct storage_info*)si);
}

ptp_array_t* alloc_storage_id_array(const uint8_t* stream, size_t len)
{
    ptp_array_t* __sia = (ptp_array_t*)malloc(sizeof(ptp_array_t));

    if (!__sia)
        return NULL;

    int __st_offset = PTP_CONTAINER_DATA_OFFSET;

    __sia->NumElements = (len - PTP_CONTAINER_DATA_OFFSET) / sizeof(storage_id);
    __sia->ArrayEntry = (storage_id*)malloc(__sia->NumElements * sizeof(storage_id));

    if (!__sia->ArrayEntry) {
        free((ptp_array_t*)__sia);
        return NULL;
    }

    memcpy(__sia->ArrayEntry, stream + __st_offset, __sia->NumElements * sizeof(storage_id));

    return __sia;
}

void free_storage_id_array(const ptp_array_t* sia)
{
    if (sia->ArrayEntry)
        free(sia->ArrayEntry);

    free((ptp_array_t*)sia);
}
