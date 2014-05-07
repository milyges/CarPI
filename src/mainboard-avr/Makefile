.SUFFIXES: .c

TARGET=carpi
SOURCES=main.c power.c i2c.c pilot.c
MCU=atmega8
F_CPU=8000000UL
AVRDUDE_PROGRAMMER=stk200
AVRDUDE_PORT=/dev/parport0
VERSION=0.1

CC=avr-gcc
CFLAGS=-Iinclude -Wall -Os -pipe -mmcu=$(MCU) -DF_CPU=$(F_CPU) -funsigned-char -funsigned-bitfields \
       -fpack-struct -fshort-enums -Wstrict-prototypes -DFIRMWARE_VERSION=\"$(VERSION)\"

ASFLAGS=$(CFLAGS) -D__ASM__

LD=avr-gcc
LDFLAGS=-mmcu=$(MCU) -Os
LDADD=

OBJCOPY=avr-objcopy
SIZE=avr-size

OBJECTS:=$(SOURCES:.c=.o)
OBJECTS:=$(OBJECTS:.S=.o)

all: $(TARGET).hex $(TARGET).eep

clean:
	@echo " CLEAN   $(OBJECTS) $(TARGET).elf $(TARGET).hex $(TARGET).eep"
	@rm -f $(OBJECTS) $(TARGET).elf $(TARGET).hex $(TARGET).eep

install: program

program: $(TARGET).hex
	@echo " INSTALL $(TARGET).hex"
	avrdude -c $(AVRDUDE_PROGRAMMER) -p $(MCU) -U flash:w:$(TARGET).hex

$(TARGET).elf: $(OBJECTS)
	@echo " LD      $@"
	@$(LD) $(LDFLAGS) -o $@ $(OBJECTS) $(LDADD)
	@echo " SIZE    $@"
	@avr-size $@

$(TARGET).hex: $(TARGET).elf
	@echo " OBJCOPY $@"
	@$(OBJCOPY) -O ihex -R .eeprom -R .fuse -R .lock $< $@

$(TARGET).eep: $(TARGET).elf
	@echo " OBJCOPY $@"
	@$(OBJCOPY)  -j .eeprom --set-section-flags=.eeprom="alloc,load" \
	--change-section-lma .eeprom=0 --no-change-warnings -O ihex $< $@


.c.o:
	@echo " CC      $@"
	@$(CC) $(CFLAGS) -c -o $@ $<

.S.o:
	@echo " AS      $@"
	$(CC) $(ASFLAGS) -c -o $@ $<
