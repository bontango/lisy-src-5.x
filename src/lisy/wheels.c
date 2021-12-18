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
#include <pthread.h>
#include <semaphore.h>
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

//from lisy35.c
extern unsigned char lisy35_flipper_disable_status;

//semaphores
sem_t wheel_sem[2][5];

//internal to wheels
int oldpos[2][5];

/*
coil and switches
5 - 41;Display 1;Pos0  100K;
6 - 42;Display 1;Pos0  10K;
7 - 43;Display 1;Pos0  10K;
8 - 44;Display 1;Pos0  100;
9 - 45;Display 1;Pos0  10;
*/

//we do it with threads in order not to block pinmame
//while pulsing (slow) wheels

//player1 digit1
static void* wheel_thread0 (void *arg) 
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[0][0]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 5, 1);
  delay(lisy_home_ss_special_coil_map[5].pulsetime);
  lisyH_special_coil_set( 5, 0);
  delay(lisy_home_ss_special_coil_map[5].delay);
 }
 return(arg);
}
//player1 digit2
static void* wheel_thread1 (void *arg) 
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[0][1]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 6, 1);
  delay(lisy_home_ss_special_coil_map[6].pulsetime);
  lisyH_special_coil_set( 6, 0);
  delay(lisy_home_ss_special_coil_map[6].delay);
 }
 return(arg);
}
//player1 digit3
static void* wheel_thread2 (void *arg) 
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[0][2]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 7, 1);
  delay(lisy_home_ss_special_coil_map[7].pulsetime);
  lisyH_special_coil_set( 7, 0);
  delay(lisy_home_ss_special_coil_map[7].delay);
 }
 return(arg);
}
//player1 digit4
static void* wheel_thread3 (void *arg) 
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[0][3]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 8, 1);
  delay(lisy_home_ss_special_coil_map[8].pulsetime);
  lisyH_special_coil_set( 8, 0);
  delay(lisy_home_ss_special_coil_map[8].delay);
 }
 return(arg);
}
//player1 digit5
static void* wheel_thread4 (void *arg) 
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[0][4]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 9, 1);
  delay(lisy_home_ss_special_coil_map[9].pulsetime);
  lisyH_special_coil_set( 9, 0);
  delay(lisy_home_ss_special_coil_map[9].delay);
 }
 return(arg);
}

//create the threads for the wheel pulsing
void wheels_init( void )
{
 pthread_t th[10];

 //init semaphor
 sem_init(&wheel_sem[0][0],0,0);

 //create threads
 pthread_create (&th[0], NULL, wheel_thread0, NULL);
 pthread_create (&th[1], NULL, wheel_thread1, NULL);
 pthread_create (&th[2], NULL, wheel_thread2, NULL);
 pthread_create (&th[3], NULL, wheel_thread3, NULL);
 pthread_create (&th[4], NULL, wheel_thread4, NULL);
 pthread_detach (th[0]);
 pthread_detach (th[1]);
 pthread_detach (th[2]);
 pthread_detach (th[3]);
 pthread_detach (th[4]);

}

/* wheel_pulse
	coil -> nr of coil
	this routine respect pulsetime and mapping
	from config file LisyH (Starship)
*/
void wheel_pulse ( int coil )
{

 if ( lisy_home_ss_special_coil_map[coil].mapped_to_coil != 0)
 {
  lisyh_coil_set(  lisy_home_ss_special_coil_map[coil].mapped_to_coil, 1);
  delay (lisy_home_ss_special_coil_map[coil].pulsetime); // milliseconds delay from wiringpi library
  lisyh_coil_set(  lisy_home_ss_special_coil_map[coil].mapped_to_coil, 0);
 }

}

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
     lisy35_switch_handler( 1 ); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],0)) wheel_pulse(5); else is_zero[0][0]=1;
     lisy35_switch_handler( 1 ); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],1)) wheel_pulse(6); else is_zero[0][1]=1;
     lisy35_switch_handler( 1 ); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],2)) wheel_pulse(7); else is_zero[0][2]=1;
     lisy35_switch_handler( 1 ); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],3)) wheel_pulse(8); else is_zero[0][3]=1;
     lisy35_switch_handler( 1 ); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],4)) wheel_pulse(9); else is_zero[0][4]=1;
     //extra delay if not all wheels are at zero
     if (is_zero[0][0]+is_zero[0][1]+is_zero[0][2]+is_zero[0][3]+is_zero[0][4] != 5) delay(300);
        }

   //reset postion as well
   for(i=0; i<5; i++) oldpos[0][i] = 0;
   for(i=0; i<5; i++) oldpos[1][i] = 0;

  if ( ls80dbg.bitv.coils )
  {
    sprintf(debugbuf,"Wheels: set wheels to zero");
    lisy80_debug(debugbuf);
  }
}



void wheel_thread_pulse( int wheel)
{

  switch(wheel)
  {
   case 5: sem_post(&wheel_sem[0][0]); break;
   case 6: sem_post(&wheel_sem[0][1]); break;
   case 7: sem_post(&wheel_sem[0][2]); break;
   case 8: sem_post(&wheel_sem[0][3]); break;
   case 9: sem_post(&wheel_sem[0][4]); break;

  }


}


void wheel_score( int display, char *data)
{
   int i,k;
   int pos[2][5],pulses[2][5];
   static unsigned char first = 1;

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

	while ( pulses[0][0]-- >0 ) wheel_thread_pulse(5);
	while ( pulses[0][1]-- >0 ) wheel_thread_pulse(6);
	while ( pulses[0][2]-- >0 ) wheel_thread_pulse(7);
	while ( pulses[0][3]-- >0 ) wheel_thread_pulse(8);
	while ( pulses[0][4]-- >0 ) wheel_thread_pulse(9);
}

void wheels_show_int( int display, int digit, unsigned char dat)
{

   int i;
   int pos[2][5],pulses;

   //reset displays at first call
   static unsigned char first = 1;

   //ignore 'spaces' , display >1 and digit >6
   if ( dat > 9 ) return;
   if ( display > 2 ) return;
   if ( digit > 6 ) return;
   //ignore credit display for the moment
   if ( display == 0 ) return;

   if ( lisy35_flipper_disable_status == 0) //flipper enabled?
     {
	//adjust numbers
	display--;
	digit--;
        //assign position
	pos[display][digit] = dat;
	//calculate pulses
        pulses = oldpos[display][digit] - pos[display][digit];
        if (  pulses > 0 )  pulses = abs( 10 -  pulses);
        if (  pulses < 0 )  pulses = abs( pulses);
        //store new value
        oldpos[display][digit] = pos[display][digit];

	//if ( ls80dbg.bitv.displays )
        if ( 1 )
  	{
	  sprintf(debugbuf,"wheels: display:%d digit:%d dat:%d (%d pulses needed)\n",display,digit,dat,pulses);
    	  lisy80_debug(debugbuf);
  	}

	switch(digit)
	{
	  case 1: while ( pulses-- >0 ) wheel_thread_pulse(5); break;
	  case 2: while ( pulses-- >0 ) wheel_thread_pulse(6); break;
	  case 3: while ( pulses-- >0 ) wheel_thread_pulse(7); break;
	  case 4: while ( pulses-- >0 ) wheel_thread_pulse(8); break;
	  case 5: while ( pulses-- >0 ) wheel_thread_pulse(9); break;
	}

    }
}
