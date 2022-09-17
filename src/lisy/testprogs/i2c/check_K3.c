/* check_K3.c
   Version 0.2
   bontango 04.2019
*/

#include <errno.h>
#include <fcntl.h>
#include <linux/i2c-dev.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wiringPi.h>

/*
  check for k3, second jumper and give back value
  0 == both options active
  1 == first options active
  2 == second options active
  3 == no options active
  -1 if error
  use option -v (or anything else as second argument) for being verbose testing

  Version 0.2: we also check if running on LISY_mini and use gpio then instead of i2c
*/

#define I2C_DEVICE         "/dev/i2c-1"
#define COIL_PIC_ADDR      0x41 // Address of PIC for coils
#define LS80COILCMD_GET_K3 7

#define LISYMINI_STROBE_1  0
#define LISYMINI_STROBE_2  2
#define LISYMINI_STROBE_3  13
#define LISYMINI_STROBE_4  25

#define LISYMINI_RET_1     10
#define LISYMINI_RET_2     11
#define LISYMINI_RET_3     26
#define LISYMINI_RET_4     27
#define LISYMINI_RET_5     28

#define LISYMINI_WAITTIME  10

//globale var, used in most routines
union three {
    unsigned char byte;

    struct {
        unsigned COMMAND_BYTE : 7, IS_CMD : 1;
        //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
    } bitv;

    struct {
        unsigned DISPLAY : 3, DIGIT : 3, FREE : 1, IS_CMD : 1;
        //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
    } bitv2; //for 80A

    struct {
        unsigned ROW : 2, POSITION : 4, COOKED : 1, IS_CMD : 1;
        //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
    } bitv3; //for 80B
} mydata_display;

int
main(int argc, char* argv[]) {
    int fd_coil_pic;
    unsigned char data, data2;
    int verbose = 0;

    if (argc >= 2) {
        verbose = 1;
        printf("we are in verbose mode\n");
    }

    //do a quick check if we are running on LISY_mini
    //before trying to access I2C
    // this means we have a 10K pull up at pin 23 GPIO11 (14)
    // which has the red LED connected for LISY1/80/35
    // we test it with making GPIO11 (wiringpi 14) an input
    // and read the value. It will be 1 for LISY_Mini and 0 for other LISYs or
    // if the PI is 'standalone'

    // wiringPi library
    if (wiringPiSetup() < 0)
        return (-1);

    pinMode(14, INPUT);
    if (digitalRead(14)) {
        if (verbose)
            printf("LISY_Mini detected\n");

        //set gpois we need
        pinMode(LISYMINI_STROBE_1, OUTPUT);
        pinMode(LISYMINI_STROBE_2, OUTPUT);
        pinMode(LISYMINI_STROBE_3, OUTPUT);
        pinMode(LISYMINI_STROBE_4, OUTPUT);
        pinMode(LISYMINI_RET_4, INPUT);
        pullUpDnControl(LISYMINI_RET_4, PUD_DOWN);
        //strobe 2
        digitalWrite(LISYMINI_STROBE_1, 0);
        digitalWrite(LISYMINI_STROBE_2, 0);
        digitalWrite(LISYMINI_STROBE_3, 1);
        digitalWrite(LISYMINI_STROBE_4, 0);
        //wait a bit
        delay(LISYMINI_WAITTIME); //x ms delay from wiringpi
        //read option via ret4
        data = digitalRead(LISYMINI_RET_4);
        if (data)
            data = 0;
        else
            data = 1;
        //strobe 3
        digitalWrite(LISYMINI_STROBE_1, 0);
        digitalWrite(LISYMINI_STROBE_2, 1);
        digitalWrite(LISYMINI_STROBE_3, 0);
        digitalWrite(LISYMINI_STROBE_4, 0);
        //wait a bit
        delay(LISYMINI_WAITTIME); //x ms delay from wiringpi
        //read option via ret4
        data2 = digitalRead(LISYMINI_RET_4);
        if (data2)
            data2 = 0;
        else
            data2 = 1;
        data = data + 2 * data2;
        //we now have k3 value in data
        //hotspot option is active if value is either 0 or 2
        if (verbose)
            printf("K3 value is %d\n", data);

        if (verbose) {
            switch (data) {
                case 0:
                    printf("both options active\n");
                    break;
                case 1:
                    printf("first option active\n");
                    break;
                case 2:
                    printf("second option active\n");
                    break;
                case 3:
                    printf("no option active\n");
                    break;
            }
        } //verbose
        return (data);
    }

    if ((fd_coil_pic = open(I2C_DEVICE, O_RDWR)) < 0) {
        printf("could not open I2C device\n");
        return -1;
    }
    // Set the port options and set the address of the device we wish to speak to
    if (ioctl(fd_coil_pic, I2C_SLAVE, COIL_PIC_ADDR) < 0) {
        printf("could not do ioctl on I2C device\n");
        return -1;
    }

    //read main sw version

    /* build control byte */
    mydata_display.bitv.COMMAND_BYTE = LS80COILCMD_GET_K3;
    mydata_display.bitv.IS_CMD = 1; //we are sending a command here

    if (write(fd_coil_pic, &mydata_display.byte, 1) != 1) //success
    {
        printf("could not write to I2C device\n");
        return -1;
    }

    //wait a bit, PIC migth be slow
    usleep(200); //wait 200ms secs

    if (read(fd_coil_pic, &data, 1) != 1) {
        printf("could not write to I2C device\n");
        return -1;
    }

    //we now have k3 value in data
    //hotspot option is active if value is either 0 or 2
    if (verbose)
        printf("K3 value is %d\n", data);

    if (verbose) {
        switch (data) {
            case 0:
                printf("both options active\n");
                break;
            case 1:
                printf("first option active\n");
                break;
            case 2:
                printf("second option active\n");
                break;
            case 3:
                printf("no option active\n");
                break;
        }
    } //verbose

    return (data);
}
