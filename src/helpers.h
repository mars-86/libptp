#ifndef __PICTURE_TRANSFER_PROTOCOL_HELPERS_INCLUDED_H__
#define __PICTURE_TRANSFER_PROTOCOL_HELPERS_INCLUDED_H__

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

#endif // __PICTURE_TRANSFER_PROTOCOL_HELPERS_INCLUDED_H__
