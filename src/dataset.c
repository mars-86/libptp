#include "dataset.h"
#include "ptp.h"
#include <stdlib.h>

struct device_info* alloc_device_info(const uint8_t* stream, size_t len)
{
    struct device_info* __di = (struct device_info*)malloc(sizeof(struct device_info));

    if (!__di)
        return NULL;

    int i;
    for (i = PTP_CONTAINER_DATA_OFFSET; i < len; ++i) {
    }

    return __di;
}
