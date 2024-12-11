
# Makefile configuration
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS         = -L../lib -L/opt/ti/msp430_gcc/include/

# Switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc

all: game.elf

# Build the game program
game.elf: game.c
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lTimer -lLcd

clean:
	rm -f *.o *.elf
