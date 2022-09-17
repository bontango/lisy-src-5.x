/*
  RTH 04.2019
  lisy mini Switch testes
*/

#include <stdio.h>
#include <unistd.h>
#include <wiringPi.h>

#define LISYMINI_STROBE_1 25
#define LISYMINI_STROBE_2 13
#define LISYMINI_STROBE_3 2
#define LISYMINI_STROBE_4 0

#define LISYMINI_RET_1    10
#define LISYMINI_RET_2    11
#define LISYMINI_RET_3    26
#define LISYMINI_RET_4    27
#define LISYMINI_RET_5    28

#define LISYMINI_WAITTIME 10

typedef union {
    unsigned char byte;

    struct {
        unsigned one : 1, two : 1, three : 1, four : 1, five : 1, six : 1, seven : 1, eight : 1;
        //signed b0:1, b1:1, b2:1, b3:1, b4:1, b5:1, b6:1, b7:1;
    } bitv;
} bitfield_t;

int
main(int argc, char* argv[]) {

    bitfield_t S1, S2, K1, K2, K3;

    // wiringPi library
    wiringPiSetup();

    pinMode(LISYMINI_STROBE_1, OUTPUT);
    pinMode(LISYMINI_STROBE_2, OUTPUT);
    pinMode(LISYMINI_STROBE_3, OUTPUT);
    pinMode(LISYMINI_STROBE_4, OUTPUT);

    pinMode(LISYMINI_RET_1, INPUT);
    pinMode(LISYMINI_RET_2, INPUT);
    pinMode(LISYMINI_RET_3, INPUT);
    pinMode(LISYMINI_RET_4, INPUT);
    pinMode(LISYMINI_RET_5, INPUT);

    pullUpDnControl(LISYMINI_RET_1, PUD_DOWN);
    pullUpDnControl(LISYMINI_RET_2, PUD_DOWN);
    pullUpDnControl(LISYMINI_RET_3, PUD_DOWN);
    pullUpDnControl(LISYMINI_RET_4, PUD_DOWN);
    pullUpDnControl(LISYMINI_RET_5, PUD_DOWN);

    //strobe 4
    digitalWrite(LISYMINI_STROBE_1, 0);
    digitalWrite(LISYMINI_STROBE_2, 0);
    digitalWrite(LISYMINI_STROBE_3, 0);
    digitalWrite(LISYMINI_STROBE_4, 1);
    //wait a bit
    delay(LISYMINI_WAITTIME); //100ms delay from wiringpi
    S2.bitv.eight = digitalRead(LISYMINI_RET_1);
    S2.bitv.four = digitalRead(LISYMINI_RET_2);
    K1.bitv.five = digitalRead(LISYMINI_RET_3);
    K1.bitv.one = digitalRead(LISYMINI_RET_4);
    S1.bitv.five = digitalRead(LISYMINI_RET_5);

    //strobe 3
    digitalWrite(LISYMINI_STROBE_1, 0);
    digitalWrite(LISYMINI_STROBE_2, 0);
    digitalWrite(LISYMINI_STROBE_3, 1);
    digitalWrite(LISYMINI_STROBE_4, 0);
    //wait a bit
    delay(LISYMINI_WAITTIME); //100ms delay from wiringpi
    S2.bitv.seven = digitalRead(LISYMINI_RET_1);
    S2.bitv.three = digitalRead(LISYMINI_RET_2);
    K1.bitv.four = digitalRead(LISYMINI_RET_3);
    K3.bitv.two = digitalRead(LISYMINI_RET_4);
    S1.bitv.four = digitalRead(LISYMINI_RET_5);

    //strobe 2
    digitalWrite(LISYMINI_STROBE_1, 0);
    digitalWrite(LISYMINI_STROBE_2, 1);
    digitalWrite(LISYMINI_STROBE_3, 0);
    digitalWrite(LISYMINI_STROBE_4, 0);
    //wait a bit
    delay(LISYMINI_WAITTIME); //100ms delay from wiringpi
    S2.bitv.six = digitalRead(LISYMINI_RET_1);
    S2.bitv.two = digitalRead(LISYMINI_RET_2);
    K1.bitv.three = digitalRead(LISYMINI_RET_3);
    K3.bitv.one = digitalRead(LISYMINI_RET_4);
    S1.bitv.three = digitalRead(LISYMINI_RET_5);

    //strobe 1
    digitalWrite(LISYMINI_STROBE_1, 1);
    digitalWrite(LISYMINI_STROBE_2, 0);
    digitalWrite(LISYMINI_STROBE_3, 0);
    digitalWrite(LISYMINI_STROBE_4, 0);
    //wait a bit
    delay(LISYMINI_WAITTIME); //100ms delay from wiringpi
    S2.bitv.five = digitalRead(LISYMINI_RET_1);
    S2.bitv.one = digitalRead(LISYMINI_RET_2);
    K1.bitv.two = digitalRead(LISYMINI_RET_3);
    K2.bitv.one = digitalRead(LISYMINI_RET_4);
    S1.bitv.one = digitalRead(LISYMINI_RET_5);

    printf("S1   S2       K1    K3 K2\n");
    printf("1345 12345678 12345 12 1\n");
    printf("%d%d%d%d ", S1.bitv.one, S1.bitv.three, S1.bitv.four, S1.bitv.five);
    printf("%d%d%d%d", S2.bitv.one, S2.bitv.two, S2.bitv.three, S2.bitv.four);
    printf("%d%d%d%d ", S2.bitv.five, S2.bitv.six, S2.bitv.seven, S2.bitv.eight);
    printf("%d%d%d%d", K1.bitv.one, K1.bitv.two, K1.bitv.three, K1.bitv.four);
    printf("%d %d%d %d", K1.bitv.five, K3.bitv.one, K3.bitv.two, K2.bitv.one);
    printf("\n");

} //main
