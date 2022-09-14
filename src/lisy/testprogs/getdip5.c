/*
 getdip5.c
 February 2018
 bontango
*/

#include "../displays.h"
#include "../fileio.h"
#include "../hw_lib.h"
#include "../lisy.h"
#include "../utils.h"
#include <stdio.h>
#include <sys/time.h>
#include <wiringPi.h>

//global vars we nned but do not use here
unsigned char swMatrix[9];
t_stru_lisy1_games_csv lisy1_game;
t_stru_lisy35_games_csv lisy35_game;
t_stru_lisy80_games_csv lisy80_game;
t_stru_lisy80_sounds_csv lisy80_sound_stru[32];

//global var for coil min pulse time option ( RTH: not used yet )
int lisy80_coil_min_pulse_time[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
int lisy80_coil_min_pulse_mod = 0; //deaktivated by default

//dummy inits
void
lisy1_init(int lisy80_throttle_val) {}

void
lisy80_init(int lisy80_throttle_val) {}

void
lisy35_init(int lisy80_throttle_val) {}

//dummy shutdowns
void
lisy1_shutdown(void) {}

void
lisy80_shutdown(void) {}

void
lisy35_shutdown(void) {}

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
    return ls80opt.bitv.mpf;
}
