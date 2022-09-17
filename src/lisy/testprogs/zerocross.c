/*
  RTH 08.2018
  lisy35 lampdriver test
*/

#include <stdio.h>
#include <string.h>

//typedefs
typedef union typed {
    unsigned char byte;

    struct {
        unsigned b0 : 1, b1 : 1, b2 : 1, b3 : 1, b4 : 1, b5 : 1, b6 : 1, b7 : 1;
    } bitv;
} bitv_t;

/* global definitions */
bitv_t inhibit[16];
unsigned char bally_lamps[60];

void
lamp_set(int lamp, int action) {

    unsigned char inhibit_bit;

    //we prepare the inhibits here for the interrupt refresh routine
    //so the interrupt is shorter
    //if inhibit is 1, we do NOT set the lamp
    if (action)
        inhibit_bit = 0;
    else
        inhibit_bit = 1;

    switch (lamp) {
        case 0 ... 14: //decoder 1
            inhibit[lamp].bitv.b0 = inhibit_bit;
            break;
        case 15 ... 29: //decoder 2
            lamp = lamp - 15;
            inhibit[lamp].bitv.b1 = inhibit_bit;
            break;
        case 30 ... 44: //decoder 3
            lamp = lamp - 30;
            inhibit[lamp].bitv.b2 = inhibit_bit;
            break;
        case 45 ... 59: //decoder 4
            lamp = lamp - 45;
            inhibit[lamp].bitv.b3 = inhibit_bit;
            break;
    }
}

int
main(int argc, char* argv[]) {
    int i, value, action;
    char mybuf[256];

    value = atoi(argv[1]);
    action = atoi(argv[2]);

    printf("sending %d action:%d to the lampdriverboard \n", value, action);

    //all lamps zero
    for (i = 0; i <= 59; i++)
        bally_lamps[i] = 0;
    //all inhibits one
    for (i = 0; i <= 15; i++)
        inhibit[i].byte = 0xff;

    lamp_set(value, action);

    for (i = 0; i <= 15; i++) {

        printf("inhibit[%d] : ", i);
        printf("(%d) inhibit: ", value);
        printf("%d ", inhibit[i].bitv.b3);
        printf("%d ", inhibit[i].bitv.b2);
        printf("%d ", inhibit[i].bitv.b1);
        printf("%d\n", inhibit[i].bitv.b0);
    }

    return 0;
} //main
