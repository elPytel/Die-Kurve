CFLAGS+= -Wall -Werror -std=gnu99 -g 
LDFLAGS= -lm 

HW=aposem
BINARIES=aposem-main

CFLAGS+=$(shell sdl2-config --cflags)
LDFLAGS+=$(shell sdl2-config --libs)

all: ${BINARIES}

OBJS=${patsubst %.c,%.o,${wildcard *.c}}

aposem-main: ${OBJS}
	${CC} ${OBJS} ${LDFLAGS} -o $@

${OBJS}: %.o: %.c
	${CC} -c ${CFLAGS} $< -o $@

zip:
	zip ${HW}-brute.zip nucleo/prgsem-mbed.cpp ${wildcard *.c} ${wildcard *.h}

clean:
	rm -f ${BINARIES} ${OBJS}
	rm -f ${HW}-brute.zip
run: all
	./${BINARIES}
