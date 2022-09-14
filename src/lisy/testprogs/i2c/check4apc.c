/* check4apc.c
   based on i2cdetect
   Version 0.1
   bontango 10.2020
*/

#include <errno.h>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
  check for com on APC I2C addr
  0 -> not present
  1 -> present
  use option -v (or anything else as second argument) for being verbose testing
*/

#define I2C_DEVICE "/dev/i2c-1"
#define APC_ADDR   0x44 // I2C Address of APC

int
main(int argc, char* argv[]) {
    int file, res;
    int verbose = 0;

    if (argc >= 2) {
        verbose = 1;
        printf("we are in verbose mode\n");
    }

    //we probe for APC device via I2C first
    file = open(I2C_DEVICE, O_RDWR);
    if (file < 0) {
        printf("could not open bus at /dev/i2c-1\n");
        return -1;
    }

    /* Set slave address */
    if (ioctl(file, I2C_SLAVE, APC_ADDR) < 0) {
        fprintf(stderr,
                "Error: Could not set "
                "address to 0x%02x: %s\n",
                APC_ADDR, strerror(errno));
        return -1;
    }

    if (verbose)
        printf("check i2c bus /dev/i2c-1 at address 0x44\n");
    res = i2c_smbus_read_byte(file);
    if (res >= 0) {
        if (verbose)
            printf("APC at address 0x44 detected\n");
        close(file);
        exit(1);
    } else {
        if (verbose)
            printf("nothing at APC address 0x44 \n");
        close(file);
        exit(0);
    }
}
