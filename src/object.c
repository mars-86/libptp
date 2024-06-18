#include "object.h"
#include "helpers.h"
#include "ptp.h"
#include "types.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct object_info* ptp_alloc_object_info(const char* filename, const char* capture_date, const char* modification_date, const char* keywords, size_t* obj_size)
{
    struct object_info* __oi = (struct object_info*)malloc(sizeof(struct object_info));

    if (!__oi)
        return NULL;

    memset(__oi, 0, sizeof(struct object_info));

    if (ptp_alloc_string_from_charp(filename, &__oi->Filename, 0) != 0)
        goto err;

    if (ptp_alloc_string_from_charp(capture_date, &__oi->CaptureDate, 0) != 0)
        goto err;

    if (ptp_alloc_string_from_charp(modification_date, &__oi->ModificationDate, 0) != 0)
        goto err;

    if (ptp_alloc_string_from_charp(keywords, &__oi->Keywords, 0) != 0)
        goto err;

    printf("%d\n", __oi->Filename.NumChars);
    printf("%d\n", __oi->CaptureDate.NumChars);
    printf("%d\n", __oi->ModificationDate.NumChars);
    printf("%d\n", __oi->Keywords.NumChars);

    if (obj_size) {
        size_t __blen = sizeof(struct object_info) - (sizeof(ptp_string_t) * 4);
        *obj_size = __blen + (4 + ((__oi->Filename.NumChars + __oi->CaptureDate.NumChars + __oi->ModificationDate.NumChars + __oi->Keywords.NumChars) * PTP_STRING_CHAR_SIZE));
    }

    return __oi;

err:
    ptp_free_object_info(__oi);
    return NULL;
}

uint8_t* ptp_object_info_to_stream(const struct object_info* oi)
{
    size_t __blen = sizeof(struct object_info) - (sizeof(ptp_string_t) * 4),
           __slen = 4 + ((oi->Filename.NumChars + oi->CaptureDate.NumChars + oi->ModificationDate.NumChars + oi->Keywords.NumChars) * PTP_STRING_CHAR_SIZE);
    uint8_t* __stream = (uint8_t*)malloc((__blen + __slen) * sizeof(uint8_t));
    int __offset = 0;

    printf("%ld\n", __blen);
    printf("%ld\n", __slen);
    printf("%ld\n", __blen + __slen);

    memcpy(__stream, oi, __blen);

    __offset += __blen;

    for (int i = 0; i < oi->Filename.NumChars; ++i)
        printf("%.2X", oi->Filename.StringChars[i]);
    putc('\n', stdout);

    __stream[__offset++] = oi->Filename.NumChars;
    ptp_cpy_string_to_stream(__stream + __offset, oi->Filename);
    __offset += oi->Filename.NumChars * PTP_STRING_CHAR_SIZE;

    __stream[__offset++] = oi->CaptureDate.NumChars;
    ptp_cpy_string_to_stream(__stream + __offset, oi->CaptureDate);
    __offset += oi->CaptureDate.NumChars * PTP_STRING_CHAR_SIZE;

    __stream[__offset++] = oi->ModificationDate.NumChars;
    ptp_cpy_string_to_stream(__stream + __offset, oi->ModificationDate);
    __offset += oi->ModificationDate.NumChars * PTP_STRING_CHAR_SIZE;

    __stream[__offset++] = oi->Keywords.NumChars;
    ptp_cpy_string_to_stream(__stream + __offset, oi->Keywords);

    for (int i = 0; i < __blen + __slen; ++i)
        printf("%.2X", __stream[i]);
    putchar('\n');

    return __stream;
}

struct object_info* ptp_stream_to_object_info(const uint8_t* stream)
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
    ptp_free_object_info(__oi);
    return NULL;
}

void ptp_free_object_info(const struct object_info* oi)
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

ptp_array_t* ptp_alloc_object_handle_array(const uint8_t* stream, size_t len)
{
    ptp_array_t* __sia = (ptp_array_t*)malloc(sizeof(ptp_array_t));

    if (!__sia)
        return NULL;

    int __st_offset = PTP_CONTAINER_DATA_OFFSET;

    __sia->NumElements = (len - PTP_CONTAINER_DATA_OFFSET) / sizeof(ptp_object_handle_t);
    __sia->ArrayEntry = (ptp_object_handle_t*)malloc(__sia->NumElements * sizeof(ptp_object_handle_t));

    if (!__sia->ArrayEntry) {
        free((ptp_array_t*)__sia);
        return NULL;
    }

    memcpy(__sia->ArrayEntry, stream + __st_offset, __sia->NumElements * sizeof(ptp_object_handle_t));

    return __sia;
}

void ptp_free_object_handle_array(const ptp_array_t* oa)
{
    if (oa->ArrayEntry)
        free(oa->ArrayEntry);

    free((ptp_array_t*)oa);
}
