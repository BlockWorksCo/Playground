

NORDICSDK_BASE=../../../NordicSDK


ECHO    = /bin/echo
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
RM      = rm



CFLAGS  += -g
CFLAGS  += -mcpu=cortex-m3
CFLAGS  += -mthumb -mabi=aapcs --std=c99
CFLAGS  += -Wall -Werror -O0
CFLAGS  += -mfloat-abi=soft
CFLAGS  += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS  += -fno-builtin --short-enums

LDFLAGS += -L../
LDFLAGS += -mthumb -mabi=aapcs -T../nrf51_xxaa.ld
LDFLAGS += -mcpu=cortex-m3
LDFLAGS += -Wl,--gc-sections
LDFLAGS += --specs=nano.specs -lc -lnosys




VPATH   += ../



OBJS    += main.o
OBJS    += system_nrf51.o
OBJS    += gcc_startup_nrf51.o



INCLUDES   += -I../


all: app.elf

clean:
	@$(ECHO) Removing object files.
	@-$(RM) *.o
	@$(ECHO) Removing ELF files.
	@-$(RM) *.elf

app.elf: $(OBJS)
	@$(ECHO) Linking $@.
	@$(LD) $(LDFLAGS) -o $@ $^





.c.o:
	@$(ECHO) Compiling $^
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $^


.s.o:
	@$(ECHO) Assembling $^
	@$(AS) $(AFLAGS) -o $@ -c $<



