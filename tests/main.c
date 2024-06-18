#include "../src/ptp.h"
#include "assert.h"
#include <fcntl.h>
#include <stdint.h>
#include <stdio.h>
#include <unistd.h>

#define DEV_USB_DIR "/dev/bus/usb"
#define DEVICE "006"

int main(void)
{
    ptp_dev_t dev;
    ptp_res_t res;
    ptp_res_params_t rparams;
    unsigned char buffer[4096];

    FILE* f;

    if (!(f = fopen(DEV_USB_DIR "/003/" DEVICE, "rb"))) {
        printf("FOPEN ERROR\n");
        return 1;
    }

    int c;
    while ((c = fgetc(f)) != EOF)
        printf("%.2X", c);

    fclose(f);

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

    if (usb_detach_interface(dev.fd, 0x00) < 0) {
        printf("DETACH INTERFACE ERROR\n");
        return 1;
    }

    if (usb_claim_interface(dev.fd, 0x00) < 0) {
        printf("CLAIM INTERFACE ERROR\n");
        return 1;
    }

    if ((status = ptp_open_session(&dev, 5, &res)) < 0) {
        perror("OPEN SESSION");

        if (usb_release_interface(dev.fd, 0x00) < 0) {
            printf("RELEASE INTERFACE ERROR\n");
            return 1;
        }

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

    struct device_info* di = ptp_alloc_device_info(buffer);

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

    ptp_free_device_info(di);

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

    ptp_array_t* stid = ptp_alloc_storage_id_array(buffer, res.length);

    ptp_storage_id_t store_id;
    for (int i = 0; i < stid->NumElements; ++i) {
        ptp_storage_id_t* sid = (ptp_storage_id_t*)stid->ArrayEntry + i;
        printf("%.8X\n", ((sid->phisical_id) << 16) | (sid->logical_id));
        if (ptp_has_storage_logical(sid)) {
            printf("PHISICAL ID: %.4X\n", sid->phisical_id);
            printf("LOGICAL ID: %.4X\n", sid->logical_id);
            memcpy(&store_id, sid, sizeof(ptp_storage_id_t));
        }
    }

    ptp_free_storage_id_array(stid);

    for (int i = 0; i < res.length; ++i)
        printf("%.2X", buffer[i]);
    putc('\n', stdout);

    if ((status = ptp_get_storage_info(&dev, ptp_build_storage_id(&store_id), buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    struct storage_info* si = ptp_alloc_storage_info(buffer);

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

    ptp_free_storage_info(si);

    if ((status = ptp_get_num_objects(&dev, ptp_build_storage_id(&store_id), 0, PTP_STORAGE_HANDLE_ROOT, &res, &rparams)) < 0) {
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

    printf("NOBJS: %.2X\n", rparams.Parameter1);

    if ((status = ptp_get_object_handles(&dev, ptp_build_storage_id(&store_id), 0, PTP_STORAGE_HANDLE_ROOT, buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    ptp_array_t* obj_handles = ptp_alloc_object_handle_array(buffer, res.length);

    for (int i = 0; i < obj_handles->NumElements; ++i) {
        ptp_object_handle_t* oh = (ptp_object_handle_t*)obj_handles->ArrayEntry + i;
        printf("%.8X\n", *oh);
    }

    printf("%d\n", obj_handles->NumElements);

    for (int i = 1; i < obj_handles->NumElements; ++i) {
        ptp_object_handle_t* oh = (ptp_object_handle_t*)obj_handles->ArrayEntry + i;
        if ((status = ptp_get_object_info(&dev, *oh, buffer, 4096, &res)) < 0) {
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

        printf("LEN: %d\n", res.length - 12);

        struct object_info* oi = ptp_stream_to_object_info(buffer);

        printf("%.8X\n", oi->StorageID);
        printf("%.4X\n", oi->ObjectFormat);
        printf("%.4X\n", oi->ProtectionStatus);
        printf("%.8X\n", oi->ObjectCompressedSize);
        printf("%.4X\n", oi->ThumbFormat);
        printf("%.8X\n", oi->ThumbCompressedSize);
        printf("%.8X\n", oi->ThumbPixWidth);
        printf("%.8X\n", oi->ThumbPixHeight);
        printf("%.8X\n", oi->ImagePixWidth);
        printf("%.8X\n", oi->ImagePixHeight);
        printf("%.8X\n", oi->ImageBitDepth);
        printf("%.8X\n", oi->ParentObject);
        printf("%.4X\n", oi->AssociationType);
        printf("%.8X\n", oi->AssociationDesc);
        printf("%.8X\n", oi->SequenceNumber);

        for (int i = 0; i < oi->Filename.NumChars; ++i)
            putc(oi->Filename.StringChars[i], stdout);
        putc('\n', stdout);

        for (int i = 0; i < oi->CaptureDate.NumChars; ++i)
            putc(oi->CaptureDate.StringChars[i], stdout);
        putc('\n', stdout);

        for (int i = 0; i < oi->ModificationDate.NumChars; ++i)
            putc(oi->ModificationDate.StringChars[i], stdout);
        putc('\n', stdout);

        for (int i = 0; i < oi->Keywords.NumChars; ++i)
            putc(oi->Keywords.StringChars[i], stdout);
        putc('\n', stdout);

        ptp_free_object_info(oi);
    }

    /*
        if ((status = ptp_get_object(&dev, 0xE, buffer, 4096, &res)) < 0) {
            printf("ERROR\n");
            ptp_free_object_handle_array(obj_handles);
            close(fd);
            return 1;
        }

        if (res.code != PTP_RESPONSE_OK) {
            printf("%s\n", ptp_get_error(res.code));
            close(fd);
            ptp_free_object_handle_array(obj_handles);
            return 1;
        }
    */

    //     printf("LEN: %d\n", res.length);
    /*
        for (int i = 0; i < res.length || i < 4096; ++i)
            printf("%.2X", buffer[i]);
        putc('\n', stdout);
    */

    /*
    if ((status = ptp_get_thumb(&dev, 0x19, buffer, 4096, &res)) < 0) {
        printf("ERROR\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    for (int i = 0; i < res.length && i < 4096; ++i)
        printf("%.2X", buffer[i]);
    putc('\n', stdout);
*/
    size_t __oisize;
    struct object_info* oi = ptp_alloc_object_info("Hello.txt", "20240404T185334.", "20240404T185334.", NULL, &__oisize);

    // oi->StorageID = 0;
    oi->ObjectFormat = PTP_OBJECT_FORMAT_TEXT;
    oi->ObjectCompressedSize = 0x5;
    // oi->ImageBitDepth = 0x0000018;

    printf("OI SIZE %ld\n", __oisize);
    /*
        if ((status = ptp_send_object_info(&dev, 0x10001, PTP_STORAGE_HANDLE_ROOT, oi, __oisize, &res, &rparams))) {
            perror("SEND OBJECT INFO\n");
            close(fd);
            return 1;
        }

        if (res.code != PTP_RESPONSE_OK) {
            printf("OBJECT INFO\n");
            printf("%s\n", ptp_get_error(res.code));
            goto err;
        }
    */
    ptp_free_object_info(oi);

    // ptp_free_object_handle_array(obj_handles);

    uint8_t msg[] = { 0x48, 0x65, 0x6C, 0x6C, 0x6F };
    /*
        if ((status = ptp_send_object(&dev, msg, 5, &res))) {
            perror("SEND OBJECT\n");
            close(fd);
            return 1;
        }

        if (res.code != PTP_RESPONSE_OK) {
            printf("SEND OBJECT\n");
            printf("%s\n", ptp_get_error(res.code));
            goto err;
        }
    */
    /*
        if ((status = ptp_delete_object(&dev, 0x24, 0, &res))) {
            perror("DELETE OBJECT\n");
            close(fd);
            return 1;
        }

        if (res.code != PTP_RESPONSE_OK) {
            printf("SEND OBJECT\n");
            printf("%s\n", ptp_get_error(res.code));
            goto err;
        }
    */

    if ((status = ptp_get_device_prop_desc(&dev, PTP_DEVICE_PROP_CODE_BATTERY_LEVEL, buffer, 4096, &res))) {
        perror("DEVICE PROP DESC\n");
        close(fd);
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("DEVICE PROP DESC\n");
        printf("%s\n", ptp_get_error(res.code));
        goto err;
    }

    printf("%d\n", res.length);

    for (int i = 0; i < res.length && i < 4096; ++i)
        printf("%.2X", buffer[i]);
    putc('\n', stdout);

err:

    ptp_close_session(&dev, &res);

    if (usb_release_interface(dev.fd, 0x00) < 0) {
        printf("RELEASE INTERFACE ERROR\n");
        return 1;
    }

    if (res.code != PTP_RESPONSE_OK) {
        printf("%s\n", ptp_get_error(res.code));
        close(fd);
        return 1;
    }

    close(fd);

    ASSERT_TEST_SUMMARY();

    return 0;
}
