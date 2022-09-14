/*
 getdip4andS2.c.c
 V 2.0
 February 2021
 bontango
*/

#include "../coils.h"
#include "../displays.h"
#include "../eeprom.h"
#include "../fadecandy.h"
#include "../fileio.h"
#include "../hw_lib.h"
#include "../lisy.h"
#include "../lisy_api.h"
#include "../lisy_api_com.h"
#include "../lisy_w.h"
#include "../sound.h"
#include "../switches.h"
#include "../usbserial.h"
#include "../utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <arpa/inet.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <wiringPi.h>

//fake definiton needed in lisy_w
void core_setSw(int myswitch, unsigned char action){};

//fake definiton needed in lisy1
void cpunum_set_clockscale(int cpu, float clockscale){};

//fake definiton needed in lisy80
typedef struct {
    struct {
        unsigned int soundBoard;
    } hw;
} core_tGameData;

core_tGameData* core_gameData;

//fake definiton needed for mame functions
typedef struct {
    unsigned char lampMatrix[2];
} t_coreGlobals;

t_coreGlobals coreGlobals;

void
lisy_nvram_write_to_file(void) {}

void sound_stream_update(int* dum){};
unsigned char sound_stream = 0;
unsigned char sound_enabled = 0;
const char* sndbrd_typestr(int board){};

//read dip4 of S1 ( mpf option
//and valueR of S2 (game selektion)
//return value is 0 for open switch and 1 for a closed one
//value of S2 is printed with 3-digits ( e.g. '019' for 19 )

int
main(int argc, char** argv) {

    unsigned char dip_switch_val;

    //do init the hardware
    lisy_hw_init(1);

    //read setting of S2 ( Game number )
    dip_switch_val = display_get_dipsw_value();
    //get only the relevant bits
    if (lisy_hardware_revision == 100)
        dip_switch_val &= 0x0F; //4 bits for LISY1
    else if (lisy_hardware_revision != 350)
        dip_switch_val &= 0x3F; //6bits for all others except LISY35

    //print it out
    printf("%03d\n", dip_switch_val);

    //and give back value of dip4
    return ls80opt.bitv.sevendigit;
}
