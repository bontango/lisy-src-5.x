#ifndef WHEELS_H
#define WHEELS_H

//starship wheel scores
void wheels_init( void );
void wheel_score_reset(void);
void wheel_score( int display, char *data);
void wheel_pulse( int wheel );
void wheels_show_int( int display, int digit, unsigned char dat);


#endif  /* WHEELS_H */

