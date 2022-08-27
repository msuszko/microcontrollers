TARGET=monitor

CC=avr-gcc
PORT=/dev/cuaU0
PROGRAMMER=stk500v2

MCU=attiny45
INC=

CFLAGS=-mmcu=$(MCU) -Wall -I. $(INC) -g -Os
CFLAGS+= -mint8


.SUFFIXES: .elf .bin .c .o
OBJS= $(TARGET).o

.c.o:
	$(CC) $(CFLAGS) -c ${.IMPSRC} -o ${.TARGET}

$(TARGET).elf: $(OBJS)
	$(CC) $(CFLAGS) -Wl,-Map,$(TARGET).map -o ${.TARGET} ${.ALLSRC}

$(TARGET).bin: $(TARGET).elf
	avr-objcopy -j .text -j .data -O binary ${.ALLSRC} ${.TARGET}


all: $(TARGET).bin

flash: $(TARGET).bin
	avrdude -p $(MCU) -P $(PORT) -c $(PROGRAMMER) -U flash:w:$(TARGET).bin:r

clean:
	rm -f *.elf *.o *.bin *.map


.PHONY: clean
