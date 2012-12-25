# This is a more specific and less cluttered Makefile. For a template Makefile,
# check http://sourceforge.net/apps/mediawiki/mspgcc/index.php?title=Example:Makefile

# Output will be main.elf
# Our microcontroller is MSP430G2553

OUTPUT		= main.hex
MCU			= msp430g2553

# Declare our CFLAGS.
CFLAGS 		= -mmcu=$(MCU) -g -Os -Wall -Wunused -I.

# Declare compiler variables

CC 			= msp430-gcc
LD			= msp430-ld
AR			= msp430-ar
OBJCOPY		= msp430-objcopy

# Dump all build intermittants in build directory

WORKDIR 	= ./build

# Declare your source file here:
SRC			= main.c

all: $(OUTPUT)

main.o:$(SRC)
	$(CC) $(CFLAGS) -o main.o -c $(SRC)
	rm -rf $(WORKDIR)
	mkdir $(WORKDIR)
	mv main.o $(WORKDIR)/main.o

main.elf:main.o
	$(CC) $(CFLAGS) -o $(WORKDIR)/main.elf $(WORKDIR)/main.o

$(OUTPUT):main.elf
	$(OBJCOPY) -O ihex $(WORKDIR)/main.elf $(WORKDIR)/$(OUTPUT)

.PHONY: clean
clean:
	rm -rf *.o *.elf *.hex build/*

.PHONY: flash
flash:main.elf
	mspdebug rf2500	"prog $(WORKDIR)/main.elf"
.PHONY: serial
serial:
	mspdebug rf2500 exit
	stty 9600 -F /dev/ttyACM0
	minicom -b 9600 -D /dev/ttyACM0
	
