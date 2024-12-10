
# Makefile configuration
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS         = -L../lib -L/opt/ti/msp430_gcc/include/

# Compiler and assembler
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: mr_game_watch.elf

# Build the mr_game_watch program
mr_game_watch.elf: main.o buttons.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lTimer -lLcd

main.o: mr_game_watch/main.c
	$(CC) $(CFLAGS) -c -o $@ $<

buttons.o: mr_game_watch/buttons.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf
