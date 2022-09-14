//get_dip_from_S1
//get specific dip from S1
//all LISY versions

#include "../../coils.h"
#include "../../displays.h"
#include "../../eeprom.h"
#include "../../fadecandy.h"
#include "../../fileio.h"
#include "../../hw_lib.h"
#include "../../lisy.h"
#include "../../lisy_api.h"
#include "../../lisy_w.h"
#include "../../sound.h"
#include "../../switches.h"
#include "../../usbserial.h"
#include "../../utils.h"
#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <fcntl.h>
#include <net/if.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <termios.h>
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

//global var for internal game_name structure, set by  lisy35_file_get_gamename in main
t_stru_lisy1_games_csv lisy1_game;
t_stru_lisy35_games_csv lisy35_game;
t_stru_lisy80_games_csv lisy80_game;
t_stru_lisymini_games_csv lisymini_game;

int
main(int argc, char* argv[]) {
    char lisy_gamename[20];
    char lisy_variant[20];
    int dip_no, res;

    //short check of number of arguments
    if (argc != 3) {
        printf("use: %s lisy_variant dip_number\n", argv[0]);
        exit(1);
    }

    //lets get the variant from the command line
    //no validation check at the moment
    strcpy(lisy_variant, argv[1]);

    //status of dip number to return
    dip_no = atoi(argv[2]);

    //check which pinball we are going to control
    //this will also call lisy_hw_init
    if ((res = lisy_set_gamename(lisy_variant, lisy_gamename)) != 0) {
        fprintf(stderr, "//get_dip_from_S1: no matching game or other error\n\r");
        return (-1);
    }

    printf("%d\n", CHECK_BIT(ls80opt.byte, dip_no - 1));
    return 0;
}
