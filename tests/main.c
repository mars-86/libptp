#include "../src/ptp.h"
#include "assert.h"
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>

#define DEV_USB_DIR "/dev/bus/usb"
#define DEVICE "022"

int main(void)
{
    ptp_dev_t dev;
    ptp_res_t res;
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

    if ((status = ptp_open_session(&dev, 1, &res)) < 0) {
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

    close(fd);
    for (int i = 0; i < res.length; ++i)
        printf("%.2X", buffer[i]);
    // putc(buffer[i], stdout);
    putc('\n', stdout);

    ASSERT_TEST_SUMMARY();

    return 0;
}
