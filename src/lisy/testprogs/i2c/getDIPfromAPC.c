/* getDIPfromAPC.c
   based on check4apc.c
   Version 0.2
   bontango 20.2020

   new protokoll

 - Setting 2 -> Alle Options DIP Schalter als Wert von 0 - 255

- Setting 3 -> Alle Game DIP Schalter (S2) als Wert von 0 - 255 (Nummer des PinMame Spiels)

- Settings 4 -> Alle Debug Jumper K1, K2 und K3 als Wert von 0 - 255 (K1 stellt dann die Bits 0-4, K2 Bit 5 / 6 und K3 ist Bit 7) 
*/

#include <errno.h>
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <linux/i2c.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>

/*
  get value of dip switch [first arg]
	
  use option -v (or anything else as third argument) for being verbose testing
*/

#define I2C_DEVICE "/dev/i2c-1"
#define APC_ADDR   0x44 // I2C Address of APC

int
main(int argc, char* argv[]) {
    int i, j, file, ret;
    unsigned char dipno;
    unsigned char dipval;
    unsigned char data[10];
    int verbose = 0;
    int8_t nextbyte;
    char content[10];
    char idstr[10];

    if (argc < 2) {
        printf("use: %s dip_no [-v]\n", argv[0]);
        exit(-1);
    }

    dipno = atoi(argv[1]);

    if (argc >= 3) {
        verbose = 1;
        printf("we are in verbose mode\n");
        printf("try to get value of dip number %d\n", dipno);
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

    //make sure APC is connected and send/recv buiffer is in sync
    data[0] = 0; //cmd for get connected hardware
    strcpy(idstr, "APC");
    //send it
    if ((ret = write(file, data, 1)) != 1) {
        printf("Error writing to I2C, return:%d %s\n", ret, strerror(errno));
        return -1;
    }

    //receive answer
    i = 0;
    j = 0;
    do {
        if ((ret = read(file, &nextbyte, 1)) != 1) {
            printf("Error reading from serial, return:%d %s\n", ret, strerror(errno));
            return -1;
        }
        content[i] = nextbyte;
        if (verbose) {
            printf("API_read_string: Byte no %d is %i (0x%02x)\"%c\"\n", j, (int8_t)nextbyte, nextbyte, nextbyte);
        }

        //check if idstr mactch content
        if (idstr[i] == content[i])
            i++;
        j++;

    } while ((strncmp(idstr, content, strlen(idstr)) != 0) & (j < 10)); //read until idstr is content
                                                                        //will block when not

    //read trailing \0
    if ((ret = read(file, &nextbyte, 1)) != 1) {
        printf("Error reading from serial, return:%d %s\n", ret, strerror(errno));
        return -1;
    }
    content[i] = nextbyte;

    //verbose?
    if (verbose) {
        printf("connected HW is: %s\n", content);
    }

    //now it is safe to get dipswitch
    data[0] = 0x40;  //cmd for get value of dip file
    data[1] = 1;     //number of setting group
    data[2] = dipno; //number of dipswitch

    //send it
    if ((ret = write(file, data, 3)) != 3) {
        printf("Error writing to I2C, return:%d %s\n", ret, strerror(errno));
        return -1;
    }

    //read answer, first byte
    if ((ret = read(file, &dipval, 1)) != 1) {
        printf("Error reading from I2C, return:%d %s\n", ret, strerror(errno));
        return -1;
    }

    if (verbose) {
        printf("return: %d bytes red\n", dipval);
    }

    if (dipval != 3)
        return -2;

    //read answer, second byte
    if ((ret = read(file, &dipval, 1)) != 1) {
        printf("Error reading from I2C, return:%d %s\n", ret, strerror(errno));
        return -1;
    }

    if (verbose) {
        printf("value of Dip number %d is %d\n", dipno, dipval);
    }

    exit(dipval);
}
