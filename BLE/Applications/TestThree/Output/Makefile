

NORDICSDK_BASE=../../../NordicSDK


ECHO    = /bin/echo
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
RM      = rm



CFLAGS  += -g
CFLAGS  += -DBOARD_PCA10001
CFLAGS  += -g
CFLAGS  += -DSOFTDEVICE_PRESENT
CFLAGS  += -DNRF51
CFLAGS  += -mcpu=cortex-m0
CFLAGS  += -mthumb -mabi=aapcs --std=gnu99
CFLAGS  += -Wall -Werror -O0
CFLAGS  += -mfloat-abi=soft
CFLAGS  += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS  += -fno-builtin --short-enums

AFLAGS  += -DNRF51


LDFLAGS += -L../
LDFLAGS += -mthumb -mabi=aapcs -T../nrf51_xxaa.ld
LDFLAGS += -mcpu=cortex-m0
LDFLAGS += -Wl,--gc-sections
LDFLAGS += --specs=nano.specs -lc -lnosys




VPATH   += ../
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/hal
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/delay
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/common
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/gpiote
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/uart
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/pstorage
VPATH   += $(NORDICSDK_BASE)/components/toolchain
VPATH   += $(NORDICSDK_BASE)/components/toolchain/gcc



OBJS    += main.o
OBJS    += system_nrf51.o
OBJS    += gcc_startup_nrf51.o



INCLUDES   += -I../
INCLUDES   += -I$(NORDICSDK_BASE)/components/device
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/delay
INCLUDES   += -I$(NORDICSDK_BASE)/components/libraries/util
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/hal
INCLUDES   += -I$(NORDICSDK_BASE)/components/toolchain/gcc
INCLUDES   += -I$(NORDICSDK_BASE)/components/toolchain
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/common


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



