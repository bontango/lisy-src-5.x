# lisy80 Makefile
# RTH 07.2017
# for lisy_swreplay prog

CC      = /usr/bin/gcc
CFLAGS  = -Wall -g -D_REENTRANT -DDEBUG
LDFLAGS = -lwiringPi

OBJ = lisy_swreplay.o

lisy_swreplay: $(OBJ)
	$(CC) $(CFLAGS) -o lisy_swreplay $(OBJ) $(LDFLAGS)

%.o: ../%.c ../%.h
	$(CC) $(CFLAGS) -c $<

