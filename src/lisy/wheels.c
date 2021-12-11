/*
 whheels.c
 part of lisy_home for Starship
 December 2021
 bontango
*/

#include <SDL2/SDL.h>
#include <SDL2/SDL_mixer.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <wiringPi.h>
#include "lisy35.h"
#include "fileio.h"
#include "hw_lib.h"
#include "displays.h"
#include "lisy_home.h"
#include "coils.h"
#include "switches.h"
#include "utils.h"
#include "eeprom.h"
#include "sound.h"
#include "lisy_home.h"
#include "fadecandy.h"
#include "externals.h"
#include "lisy.h"


//for Starship Switches
extern unsigned char swMatrixLISY35[9];

//set the scoring wheel to value
//6digit version only, ignoring digit 1
/*
coil and switches
5 - 41;Display 1;Pos0  100K;
6 - 42;Display 1;Pos0  10K;
7 - 43;Display 1;Pos0  10K;
8 - 44;Display 1;Pos0  100;
9 - 45;Display 1;Pos0  10;
*/


//set all wheels to 'zero' position
//display1 only at the moment
void wheel_score_reset( void )
{

   int i;
   int is_zero[2][5] =  {{0,0,0,0,0},{0,0,0,0,0}};

   //set 5 digits
   //maximum 9 steps
   for(i=1; i<=10; i++)
   {
     if ( CHECK_BIT(swMatrixLISY35[6],0)) lisyH_special_coil_pulse(5); else is_zero[0][0]=1;
     if ( CHECK_BIT(swMatrixLISY35[6],1)) lisyH_special_coil_pulse(6); else is_zero[0][1]=1;
     if ( CHECK_BIT(swMatrixLISY35[6],2)) lisyH_special_coil_pulse(7); else is_zero[0][2]=1;
     if ( CHECK_BIT(swMatrixLISY35[6],3)) lisyH_special_coil_pulse(8); else is_zero[0][3]=1;
     if ( CHECK_BIT(swMatrixLISY35[6],4)) lisyH_special_coil_pulse(9); else is_zero[0][4]=1;
     //extra delay if not all wheels are at zero
     if (is_zero[0][0]+is_zero[0][1]+is_zero[0][2]+is_zero[0][3]+is_zero[0][4] != 5) delay(300);
        }
}

void wheel_score( int display, char *data)
{
   int i,k;
   int pos[2][5],pulses[2][5];
   static int oldpos[2][5];
   int delay_needed = 0;
   unsigned char first = 1;

   // -48 to get an int out of the ascii code ( 0..9)
   for(i=0; i<5; i++) pos[display-1][i] = data[i] - 48;

   //first call set to zero
   if (first)
   {
	if ( ls80dbg.bitv.displays )
  	{
    	  sprintf(debugbuf,"Wheels: first call, set wheels to zero");
    	  lisy80_debug(debugbuf);
  	}
	first = 0;
	wheel_score_reset();
	for(i=0; i<5; i++) oldpos[0][i] = 0;
	for(i=0; i<5; i++) oldpos[1][i] = 0;
	if ( ls80dbg.bitv.displays )
  	{
    	  sprintf(debugbuf,"Wheels: set to zero done");
    	  lisy80_debug(debugbuf);
  	}
   }

	if ( ls80dbg.bitv.displays )
  	{
    	  sprintf(debugbuf,"set display %d to %d%d%d%d%d",display,
		pos[display-1][0],pos[display-1][1],pos[display-1][2],pos[display-1][3],pos[display-1][4]);
    	  lisy80_debug(debugbuf);
  	}

	//calculate number of pulses needed
	for(i=0; i<5; i++) 
	 { 
	   pulses[0][i] = oldpos[0][i] - pos[0][i];
	   if (  pulses[0][i] > 0 )  pulses[0][i] = abs( 10 -  pulses[0][i]);
	   if (  pulses[0][i] < 0 )  pulses[0][i] = abs( pulses[0][i]);
	   //store new value
	   oldpos[0][i] = pos[0][i];
	 }

	while ( pulses[0][0]-- >0 ) { lisyH_special_coil_pulse(5); delay(300); }
	while ( pulses[0][1]-- >0 ) { lisyH_special_coil_pulse(6); delay(300); }
	while ( pulses[0][2]-- >0 ) { lisyH_special_coil_pulse(7); delay(300); }
	while ( pulses[0][3]-- >0 ) { lisyH_special_coil_pulse(8); delay(300); }
	while ( pulses[0][4]-- >0 ) { lisyH_special_coil_pulse(9); delay(300); }

}
                                                   
