
# Makefile configuration
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS         = -L../lib -L/opt/ti/msp430_gcc/include/

# Switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: simplified_game_and_watch.elf

# Build the simplified_game_and_watch program
simplified_game_and_watch.elf: simplified_game_and_watch/main.c simplified_game_and_watch/buttons.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lTimer -lLcd

clean:
	rm -f *.o *.elf
