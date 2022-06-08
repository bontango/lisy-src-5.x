#ifndef WHEELS_H
#define WHEELS_H

//starship wheel scores
void wheel_score_reset(void);
void wheel_score_credits_reset(void);
void wheel_score( int display, char *data);
void wheel_pulse( int wheel );
void wheels_show_int( int display, int digit, unsigned char dat);
void wheels_refresh(void);
void wheel_hstd(unsigned char ss_hstd_disp[][6], unsigned char ss_game_disp[][6], int sleeptime );


#endif  /* WHEELS_H */

