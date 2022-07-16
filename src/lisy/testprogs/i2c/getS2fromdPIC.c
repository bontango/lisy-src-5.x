/* check4hotspotoption.c
   Version 0.2
   bontango 02.2018
*/

#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <errno.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <linux/i2c-dev.h>


/*
  contact display PIC and ask for S2 setting
*/

#define I2C_DEVICE "/dev/i2c-1"
#define COIL_PIC_ADDR 0x40              // Address of PIC for displays
#define LS80DPCMD_GET_DIPSW_VALUE 4


//globale var, used in most routines
union three {
    unsigned char byte;
    struct {
    unsigned COMMAND_BYTE:7, IS_CMD:1;
    //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
        } bitv;
    struct {
    unsigned DISPLAY:3, DIGIT:3, FREE:1, IS_CMD:1;
    //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
        } bitv2;   //for 80A
    struct {
    unsigned ROW:2, POSITION:4, COOKED:1, IS_CMD:1;
    //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
        } bitv3;   //for 80B
    } mydata_display;

int main(int argc, char *argv[])
{
 int fd_coil_pic;
 unsigned char data;
 int verbose = 0;

 if (argc >= 2) 
 {
  verbose = 1;
  printf("we are in verbose mode\n");
 } 


 if ((fd_coil_pic = open( I2C_DEVICE,O_RDWR)) < 0)
  { 
   printf("could not open I2C device\n");
   return -1;
 }
 // Set the port options and set the address of the device we wish to speak to
 if (ioctl(fd_coil_pic, I2C_SLAVE, COIL_PIC_ADDR) < 0)
  { 
   printf("could not do ioctl on I2C device\n");
   return -1;
 }

 //read main sw version

 /* build control byte */
 mydata_display.bitv.COMMAND_BYTE = LS80DPCMD_GET_DIPSW_VALUE;
 mydata_display.bitv.IS_CMD = 1;        //we are sending a command here

 if ( write(fd_coil_pic,&mydata_display.byte,1) != 1) //success
  { 
   printf("could not write to I2C device\n");
   return -1;
 }

 //wait a bit, PIC migth be slow
 usleep(20000); //wait 200ms secs

 if (read(fd_coil_pic, &data, 1) != 1)
  { 
   printf("could not write to I2C device\n");
   return -1;
 }

 //we now have k3 value in data
 //hotspot option is active if value is either 0 or 2
 if (verbose) printf("S2 value is %d\n",data);

 return(data);

}
