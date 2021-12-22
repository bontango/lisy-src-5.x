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
//player2 digit1
static void* wheel_thread5 (void *arg)
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[1][0]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 12, 1);
  delay(lisy_home_ss_special_coil_map[12].pulsetime);
  lisyH_special_coil_set( 12, 0);
  delay(lisy_home_ss_special_coil_map[12].delay);
 }
 return(arg);
}
//player2 digit2
static void* wheel_thread6 (void *arg)
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[1][1]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 13, 1);
  delay(lisy_home_ss_special_coil_map[13].pulsetime);
  lisyH_special_coil_set( 13, 0);
  delay(lisy_home_ss_special_coil_map[13].delay);
 }
 return(arg);
}
//player2 digit3
static void* wheel_thread7 (void *arg)
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[1][2]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 14, 1);
  delay(lisy_home_ss_special_coil_map[14].pulsetime);
  lisyH_special_coil_set( 14, 0);
  delay(lisy_home_ss_special_coil_map[14].delay);
 }
 return(arg);
}
//player2 digit4
static void* wheel_thread8 (void *arg)
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[1][3]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 15, 1);
  delay(lisy_home_ss_special_coil_map[15].pulsetime);
  lisyH_special_coil_set( 15, 0);
  delay(lisy_home_ss_special_coil_map[15].delay);
 }
 return(arg);
}
//player2 digit5
static void* wheel_thread9 (void *arg)
{
 while(1) //endless loop
 {
  //wait for semaphor
  sem_wait(&wheel_sem[1][4]);
  //puls the wheel and wait afterwards
  lisyH_special_coil_set( 16, 1);
  delay(lisy_home_ss_special_coil_map[16].pulsetime);
  lisyH_special_coil_set( 16, 0);
  delay(lisy_home_ss_special_coil_map[16].delay);
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
 pthread_create (&th[5], NULL, wheel_thread5, NULL);
 pthread_create (&th[6], NULL, wheel_thread6, NULL);
 pthread_create (&th[7], NULL, wheel_thread7, NULL);
 pthread_create (&th[8], NULL, wheel_thread8, NULL);
 pthread_create (&th[9], NULL, wheel_thread9, NULL);
 pthread_detach (th[0]);
 pthread_detach (th[1]);
 pthread_detach (th[2]);
 pthread_detach (th[3]);
 pthread_detach (th[4]);
 pthread_detach (th[5]);
 pthread_detach (th[6]);
 pthread_detach (th[7]);
 pthread_detach (th[8]);
 pthread_detach (th[9]);

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
  //delay (lisy_home_ss_special_coil_map[coil].delay); // milliseconds delay from wiringpi library
 }

}

//set all wheels to 'zero' position
//display1 only at the moment
void wheel_score_reset( void )
{

   int i, check_for_all_zero;
   int is_zero[2][5] =  {{0,0,0,0,0},{0,0,0,0,0}};

   //set 5 digits
   //maximum 9 steps
   for(i=1; i<=10; i++)
   {
     //display1
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],0)) wheel_pulse(5); else is_zero[0][0]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],1)) wheel_pulse(6); else is_zero[0][1]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],2)) wheel_pulse(7); else is_zero[0][2]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],3)) wheel_pulse(8); else is_zero[0][3]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[6],4)) wheel_pulse(9); else is_zero[0][4]=1;
     //display2
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[8],0)) wheel_pulse(12); else is_zero[1][0]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[8],1)) wheel_pulse(13); else is_zero[1][1]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[8],2)) wheel_pulse(14); else is_zero[1][2]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[8],3)) wheel_pulse(15); else is_zero[1][3]=1;
     lisy35_switchmatrix_update(); //update internal matrix to detect zero switch
     if ( CHECK_BIT(swMatrixLISY35[8],4)) wheel_pulse(16); else is_zero[1][4]=1;



     //extra delay if not all wheels are at zero
     check_for_all_zero = is_zero[0][0]+is_zero[0][1]+is_zero[0][2]+is_zero[0][3]+is_zero[0][4];
     check_for_all_zero = check_for_all_zero+is_zero[1][0]+is_zero[1][1]+is_zero[1][2]+is_zero[1][3]+is_zero[1][4];
     if (check_for_all_zero != 10) delay(300);
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
	//display1
   case 5: sem_post(&wheel_sem[0][0]); break;
   case 6: sem_post(&wheel_sem[0][1]); break;
   case 7: sem_post(&wheel_sem[0][2]); break;
   case 8: sem_post(&wheel_sem[0][3]); break;
   case 9: sem_post(&wheel_sem[0][4]); break;
	//display2
   case 12: sem_post(&wheel_sem[1][0]); break;
   case 13: sem_post(&wheel_sem[1][1]); break;
   case 14: sem_post(&wheel_sem[1][2]); break;
   case 15: sem_post(&wheel_sem[1][3]); break;
   case 16: sem_post(&wheel_sem[1][4]); break;

  }


}

//called from lisy200_control only
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
	for(i=0; i<5; i++) 
	 { 
	   pulses[1][i] = oldpos[1][i] - pos[1][i];
	   if (  pulses[1][i] > 0 )  pulses[1][i] = abs( 10 -  pulses[1][i]);
	   if (  pulses[1][i] < 0 )  pulses[1][i] = abs( pulses[1][i]);
	   //store new value
	   oldpos[1][i] = pos[1][i];
	 }

	//display2
	while ( pulses[0][0]-- >0 ) wheel_thread_pulse(5);
	while ( pulses[0][1]-- >0 ) wheel_thread_pulse(6);
	while ( pulses[0][2]-- >0 ) wheel_thread_pulse(7);
	while ( pulses[0][3]-- >0 ) wheel_thread_pulse(8);
	while ( pulses[0][4]-- >0 ) wheel_thread_pulse(9);
	//display2
	while ( pulses[1][0]-- >0 ) wheel_thread_pulse(12);
	while ( pulses[1][1]-- >0 ) wheel_thread_pulse(13);
	while ( pulses[1][2]-- >0 ) wheel_thread_pulse(14);
	while ( pulses[1][3]-- >0 ) wheel_thread_pulse(15);
	while ( pulses[1][4]-- >0 ) wheel_thread_pulse(16);
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

     if(display==0)
     {
	switch(digit)
	{
	  case 1: while ( pulses-- >0 ) wheel_thread_pulse(5); break;
	  case 2: while ( pulses-- >0 ) wheel_thread_pulse(6); break;
	  case 3: while ( pulses-- >0 ) wheel_thread_pulse(7); break;
	  case 4: while ( pulses-- >0 ) wheel_thread_pulse(8); break;
	  case 5: while ( pulses-- >0 ) wheel_thread_pulse(9); break;
	}
     }
     if(display==1)
     {
        switch(digit)
        {
          case 1: while ( pulses-- >0 ) wheel_thread_pulse(12); break;
          case 2: while ( pulses-- >0 ) wheel_thread_pulse(13); break;
          case 3: while ( pulses-- >0 ) wheel_thread_pulse(14); break;
          case 4: while ( pulses-- >0 ) wheel_thread_pulse(15); break;
          case 5: while ( pulses-- >0 ) wheel_thread_pulse(16); break;
        }
     }

    }//flipper enabled?
}
