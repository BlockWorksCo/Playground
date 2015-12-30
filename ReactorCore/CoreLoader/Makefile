SRC=main.c loader.c
ASRC=

TARGET=elfloader

CROSS=arm-linux-gnueabi-
CC=$(CROSS)gcc
AS=$(CROSS)gcc
LD=$(CROSS)gcc

CFLAGS=-O0 -ggdb3 -mcpu=cortex-a7 -mthumb \
	-ffunction-sections -fdata-sections \
	-Iarm/CMSIS/include -I.

LDFLAGS=\
  -Wl,--start-group -lgcc -lc -lc -lm -Wl,--end-group \
  -Wl,--gc-sections

OBJS=$(SRC:.c=.o) $(ASRC:.S=.o)
DEPS=$(SRC:.c=.d)

all: $(TARGET)

-include $(DEPS)

%.o: %.c
	@echo " CC $<"
	@$(CC) -MMD $(CFLAGS) -o $@ -c $<

%.o: %.S
	@echo " AS $<"
	@$(AS) $(CFLAGS) -o $@ -c $<

.PHONY: clean all debug app

$(TARGET): $(OBJS)
	@echo " LINK $@"
	@$(LD) -o $@ $(LDFLAGS) $^

app:
	@$(MAKE) -C app clean all list

clean:
	@echo " CLEAN"
	@rm -fR $(OBJS) $(DEPS) $(TARGET)

debug: $(TARGET) app
	@echo " Debuggin..."
	@$(CROSS)gdb $(TARGET) \
		-ex 'target remote :3333' \
		-ex 'monitor reset halt' \
		-ex 'load' \
		-ex 'monitor arm semihosting enable'

run: $(TARGET) app
	@echo " Debuggin..."
	@$(CROSS)gdb $(TARGET) \
		-ex 'target remote :3333' \
		-ex 'monitor reset halt' \
		-ex 'load' \
		-ex 'monitor arm semihosting enable' \
		-ex 'continue'

openocd: app
	@echo " Launch OpenOCD for stm32f4discovery"
	@cd app && openocd -f board/stm32f4discovery.cfg -l oocd.log
