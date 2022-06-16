#ifndef WHEELS_H
#define WHEELS_H

//starship wheel scores
void wheel_score_reset(void);
void wheel_score_credits_reset(void);
void wheel_score( int display, char *data);
void wheel_pulse( int wheel );
void wheels_show_int( int display, int digit, unsigned char dat);
void wheels_refresh(void);
void wheel_hstd(unsigned char hstd[6], unsigned char player1[6], unsigned char player2[6], int sleeptime );


#endif  /* WHEELS_H */

