#include "../src/ptp.h"
#include "assert.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define DEV_USB_DIR "/dev/bus/usb"
#define DEVICE "025"

int main(void)
{
    ptp_dev_t dev;
    ptp_res_t res;
    ptp_res_params_t rparams;
    unsigned char buffer[4096];

    int fd = open(DEV_USB_DIR "/003/" DEVICE, O_RDWR);

    printf("%d\n", fd);

    if (fd == -1) {
        perror("open");
        return 1;
    }

    dev.fd = fd;
    dev.endp_in = 1;
    dev.endp_out = 1;
    dev.endp_in_max_pack_size = 512;
    dev.endp_out_max_pack_size = 512;

    ASSERT_TEST_INIT();

    int status;

    if ((status = ptp_open_session(&dev, 2, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    ASSERT_EXPR(res.code == PTP_RESPONSE_OK);

    if ((status = ptp_get_device_info(&dev, buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    ASSERT_EXPR(res.code == PTP_RESPONSE_OK);
    ASSERT_NUM_VAL(res.length, 553);
    /*
        for (int i = 0; i < res.length; ++i)
            printf("%.2X", buffer[i]);
        // putc(buffer[i], stdout);
        putc('\n', stdout);

        for (int i = 12; i < 14; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 14; i < 18; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 18; i < 20; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 21; i < 259; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 21; i < 21 + (0x77 * 2); ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 259; i < 261; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 265; i < 341; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 345; i < 355; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 359; i < 369; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 377; i < 417; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 418; i < 479; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 479; i < 498; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 498; i < 526; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 527; i < 553; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 0; i < 300; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);
    */
    struct device_info* di = alloc_device_info(buffer, res.length);

    printf("%.4X\n", di->StandardVersion);
    printf("%.8X\n", di->VendorExtensionID);
    printf("%.4X\n", di->VendorExtensionVersion);

    for (int i = 0; i < di->VendorExtensionDesc.NumChars; ++i)
        putc(di->VendorExtensionDesc.StringChars[i], stdout);
    putc('\n', stdout);

    printf("%.4X\n", di->FunctionalMode);

    for (int i = 0; i < di->OperationsSupported.NumElements; i++) {
        uint16_t* __elem = (uint16_t*)di->OperationsSupported.ArrayEntry + i;
        printf("%.4X", *__elem);
    }
    putc('\n', stdout);

    for (int i = 0; i < di->EventsSupported.NumElements; i++) {
        uint16_t* __elem = (uint16_t*)di->EventsSupported.ArrayEntry + i;
        printf("%.4X", *__elem);
    }
    putc('\n', stdout);

    for (int i = 0; i < di->DevicePropertiesSupported.NumElements; i++) {
        uint16_t* __elem = (uint16_t*)di->DevicePropertiesSupported.ArrayEntry + i;
        printf("%.4X", *__elem);
    }
    putc('\n', stdout);

    for (int i = 0; i < di->CaptureFormats.NumElements; i++) {
        uint16_t* __elem = (uint16_t*)di->CaptureFormats.ArrayEntry + i;
        printf("%.4X", *__elem);
    }
    putc('\n', stdout);

    for (int i = 0; i < di->ImageFormats.NumElements; i++) {
        uint16_t* __elem = (uint16_t*)di->ImageFormats.ArrayEntry + i;
        printf("%.4X", *__elem);
    }
    putc('\n', stdout);

    for (int i = 0; i < di->Manufacturer.NumChars; ++i)
        putc(di->Manufacturer.StringChars[i], stdout);
    putc('\n', stdout);

    for (int i = 0; i < di->Model.NumChars; ++i)
        putc(di->Model.StringChars[i], stdout);
    putc('\n', stdout);

    for (int i = 0; i < di->DeviceVersion.NumChars; ++i)
        putc(di->DeviceVersion.StringChars[i], stdout);
    putc('\n', stdout);

    for (int i = 0; i < di->SerialNumber.NumChars; ++i)
        putc(di->SerialNumber.StringChars[i], stdout);
    putc('\n', stdout);

    free_device_info(di);

    if ((status = ptp_get_storage_id(&dev, buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    for (int i = 0; i < res.length; ++i)
        printf("%.2X", buffer[i]);
    putc('\n', stdout);

    if ((status = ptp_get_storage_info(&dev, 0x10001, buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }
    /*
        for (int i = 12; i < 14; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 14; i < 16; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 16; i < 18; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 18; i < 26; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 26; i < 34; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 34; i < 38; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);

        for (int i = 38; i < 50; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 50; i < 84; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);

        for (int i = 0; i < res.length; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);
    */
    struct storage_info* si = alloc_storage_info(buffer, res.length);

    printf("%.4X\n", si->StorageType);
    printf("%.4X\n", si->FilesystemType);
    printf("%.4X\n", si->AccessCapability);
    printf("%.16lX\n", si->MaxCapacity);
    printf("%.16lX\n", si->FreeSpaceInBytes);
    printf("%.8X\n", si->FreeSpaceInImages);

    for (int i = 0; i < si->StorageDescription.NumChars; ++i)
        putc(si->StorageDescription.StringChars[i], stdout);
    putc('\n', stdout);

    for (int i = 0; i < si->VolumeLabel.NumChars; ++i)
        putc(si->VolumeLabel.StringChars[i], stdout);
    putc('\n', stdout);

    free_storage_info(si);

    if ((status = ptp_get_num_objects(&dev, 0x10001, 0, 0, &res, &rparams)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    printf("NOBJS: %.2X\n", rparams.Parameter1);

    if ((status = ptp_get_object_handles(&dev, 0x10001, 0, 0xFFFFFFFF, buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    for (int i = 13; i < res.length; i += 4)
        printf("%.8X\n", ((buffer[i] << 24) | (buffer[i + 1] << 16) | (buffer[i + 2] << 8) | buffer[i + 3]));
    putc('\n', stdout);

    for (int i = 0; i < res.length; ++i)
        printf("%.2X", buffer[i]);
    putc('\n', stdout);

    for (int i = 1; i < 13; ++i) {
        if ((status = ptp_get_object_info(&dev, i, buffer, 4096, &res)) < 0) {
            printf("ERROR\n");
            close(fd);
            return 1;
        }

        if (res.code != PTP_RESPONSE_OK) {
            printf("%s\n", ptp_get_error(res.code));
            close(fd);
            return 1;
        }

        for (int i = 0; i < res.length; ++i)
            printf("%c", buffer[i]);
        putc('\n', stdout);
    }

    if ((status = ptp_get_object_info(&dev, 0x0A, buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    for (int i = 0; i < res.length; ++i)
        printf("%.2X", buffer[i]);
    putc('\n', stdout);

    ptp_close_session(&dev, &res);

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    close(fd);

    ASSERT_TEST_SUMMARY();

    return 0;
}
