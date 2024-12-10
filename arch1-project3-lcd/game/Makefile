
# Makefile configuration
CPU             = msp430g2553
CFLAGS          = -mmcu=${CPU} -Os -I../h
LDFLAGS         = -L../lib -L/opt/ti/msp430_gcc/include/

# Switch the compiler (for the internal make rules)
CC              = msp430-elf-gcc
AS              = msp430-elf-gcc -mmcu=${CPU} -c

all: mr_game_watch.elf

# Compile and link the mr_game_watch program
mr_game_watch.elf: main.o buttons.o
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -lTimer -lLcd

main.o: main.c
	$(CC) $(CFLAGS) -c -o $@ $<

buttons.o: buttons.c
	$(CC) $(CFLAGS) -c -o $@ $<

clean:
	rm -f *.o *.elf
