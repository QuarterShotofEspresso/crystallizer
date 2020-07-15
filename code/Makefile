MCU=atmega1284
F_CPU=8000000
CC=avr-gcc
OBJCOPY=avr-objcopy
CFLAGS=-std=c99 -Wall -g -Os -mmcu=${MCU} -DF_CPU=${F_CPU} -I./includes
TARGET=builds/main
SRCS=src/main.c
PROGRAMMER=atmelice_isp

all:
	${CC} ${CFLAGS} -o ${TARGET} ${SRCS}
	${OBJCOPY} -j .text -j .data -O ihex ${TARGET}.bin ${TARGET}.hex

flash:
	avrdude -p ${MCU} -c ${PROGRAMMER} -v -v -U flash:w:${TARGET}.hex:i -F

clean:
	rm -f builds/*
