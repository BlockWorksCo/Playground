

NORDICSDK_BASE=../../../NordicSDK


ECHO    = /bin/echo
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
RM      = rm



CFLAGS  += -DSTM32F103C8
CFLAGS  += -DSTM32F10X_MD
CFLAGS  += -DUSE_SX1276_RADIO
CFLAGS  += -DUSE_STDPERIPH_DRIVER
CFLAGS  += -g
CFLAGS  += -mcpu=cortex-m3
CFLAGS  += -mthumb -mabi=aapcs --std=c99
CFLAGS  += -Wall -Werror -O0
CFLAGS  += -mfloat-abi=soft
CFLAGS  += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS  += -fno-builtin --short-enums

LDFLAGS += -L../
LDFLAGS += -mthumb -mabi=aapcs -T../stm32f10x.ld
LDFLAGS += -mcpu=cortex-m3
LDFLAGS += -Wl,--gc-sections
LDFLAGS += --specs=nano.specs -lc -lnosys




VPATH   += ../
VPATH   += ../stm_lib/src 
VPATH   += ../cmsis
VPATH   += ../cmsis_boot
VPATH   += ../cmsis_boot/startup
VPATH   += ../liblora
VPATH   += ../EventQueue



OBJS    += main.o
OBJS    += system_stm32f10x.o
OBJS    += startup_stm32f10x_md.o
OBJS    += stm32f10x_rcc.o
OBJS    += stm32f10x_gpio.o
OBJS    += stm32f10x_spi.o
OBJS    += stm32f10x_tim.o
OBJS    += sx1276-RF.o
OBJS    += sx1276-LoRa.o
OBJS    += sx1276-LoRaMisc.o
OBJS    += sx1276-PhysicalInterface.o
OBJS    += SPIBus.o
OBJS    += delay.o
OBJS    += sysclk.o
OBJS    += EventQueue.o



INCLUDES   += -I../
INCLUDES   += -I../stm_lib/inc
INCLUDES   += -I../cmsis
INCLUDES   += -I../cmsis_boot
INCLUDES   += -I../liblora
INCLUDES   += -I../EventQueue


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



