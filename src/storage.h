#ifndef __PICTURE_TRANSFER_PROTOCOL_STORAGE_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_STORAGE_INCLUDED_H__

#include "types.h"
#include <stdint.h>
#include <stdio.h>

#define PTP_STORAGE_TYPE_UNDEFINED 0x0000
#define PTP_STORAGE_TYPE_FIXED_ROM 0x0001
#define PTP_STORAGE_TYPE_REMOVABLE_ROM 0x0002
#define PTP_STORAGE_TYPE_FIXED_RAM 0x0003
#define PTP_STORAGE_TYPE_REMOVABLE_RAM 0x0004

#define PTP_STORAGE_FILESYSTEM_UNDEFINED 0x0000
#define PTP_STORAGE_FILESYSTEM_GENERIC_FLAT 0x0001
#define PTP_STORAGE_FILESYSTEM_GENERIC_HIERARCHICAL 0x0002
#define PTP_STORAGE_FILESYSTEM_DCF 0x0003

#define PTP_STORAGE_ACCESS_CAPABILITY_RW 0x0000
#define PTP_STORAGE_ACCESS_CAPABILITY_RO_WITHOUT_OD 0x0001
#define PTP_STORAGE_ACCESS_CAPABILITY_RO_WITH_OD 0x0002

#define PTP_STORAGE_MAX_CAPACITY_UNUSED 0xFFFFFFFFFFFFFFFF
#define PTP_STORAGE_FREE_SPACE_IN_BYTES_UNUSED 0xFFFFFFFFFFFFFFFF
#define PTP_STORAGE_FREE_SPACE_IN_IMAGES_UNUSED 0xFFFFFFFF

#define PTP_STORAGE_PHYSICAL_ID_MASK(storage_id) ((storage_id & 0xFFFF0000) >> 16)
#define PTP_STORAGE_LOGICAL_ID_MASK(storage_id) (storage_id & 0x0000FFFF)

#define PTP_STORAGE_ID_UNUSED 0x00000000
#define PTP_STORAGE_HANDLE_ROOT 0xFFFFFFFF
#define PTP_STORAGE_HANDLE_UNUSED 0x00000000

struct ptp_storage_id {
    uint16_t phisical_id;
    uint16_t logical_id;
};

typedef struct ptp_storage_id ptp_storage_id_t;

// TODO review this packed struct
struct storage_info {
    uint16_t StorageType;
    uint16_t FilesystemType;
    uint16_t AccessCapability;
    uint64_t MaxCapacity;
    uint64_t FreeSpaceInBytes;
    uint32_t FreeSpaceInImages;
    ptp_string_t StorageDescription;
    ptp_string_t VolumeLabel;
} __attribute__((packed));

inline uint32_t ptp_build_storage_id(ptp_storage_id_t* sid)
{
    return ((sid->phisical_id) << 16 | (sid->logical_id));
}

inline uint8_t ptp_has_storage_logical(ptp_storage_id_t* sid)
{
    return sid->logical_id != 0;
}

struct storage_info* ptp_alloc_storage_info(const uint8_t* stream);

void ptp_free_storage_info(const struct storage_info* si);

ptp_array_t* ptp_alloc_storage_id_array(const uint8_t* stream, size_t len);

void ptp_free_storage_id_array(const ptp_array_t* sia);

#endif // __PICTURE_TRANSFER_PROTOCOL_STORAGE_INCLUDED_H__
