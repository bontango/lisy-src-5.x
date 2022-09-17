/*
 test_pic_eeprom.c
 January 2018
 bontango
*/

#include "../displays.h"
#include "../eeprom.h"
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

int
main(int argc, char** argv) {

    int i, val;

    //do init the hardware
    lisy_hw_init(1);

    printf("after init\n");

    //read it
    for (i = 0; i <= 255; i++) {
        //delay(20); delay integrated in read call already
        //val = lisy_eeprom_1byte_read ( i, 0);
        val = lisy_eeprom_1byte_read(i, 1);
        printf("thats what we read for pos %d: %d\n", i, val);
    }

    return (0);
}
