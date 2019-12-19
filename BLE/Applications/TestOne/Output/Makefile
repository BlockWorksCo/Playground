

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
CFLAGS  += -DS110
CFLAGS  += -DBLE_STACK_SUPPORT_REQD
CFLAGS  += -DSWI_DISABLE0
CFLAGS  += -mcpu=cortex-m0
CFLAGS  += -mthumb -mabi=aapcs --std=gnu99
CFLAGS  += -Wall -Werror -O0
CFLAGS  += -mfloat-abi=soft
CFLAGS  += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS  += -fno-builtin --short-enums

AFLAGS  += -DSOFTDEVICE_PRESENT
AFLAGS  += -DNRF51
AFLAGS  += -DS110
AFLAGS  += -DBLE_STACK_SUPPORT_REQD
AFLAGS  += -DSWI_DISABLE0


LDFLAGS += -L../
LDFLAGS += -mthumb -mabi=aapcs -T../nrf51_xxaa.ld
LDFLAGS += -mcpu=cortex-m0
LDFLAGS += -Wl,--gc-sections
LDFLAGS += --specs=nano.specs -lc -lnosys




VPATH   += ../
VPATH   += $(NORDICSDK_BASE)/components/libraries/button
VPATH   += $(NORDICSDK_BASE)/components/libraries/util
VPATH   += $(NORDICSDK_BASE)/components/libraries/timer
VPATH   += $(NORDICSDK_BASE)/components/libraries/trace
VPATH   += $(NORDICSDK_BASE)/components/libraries/util
VPATH   += $(NORDICSDK_BASE)/components/libraries/uart
VPATH   += $(NORDICSDK_BASE)/components/libraries/sensorsim
VPATH   += $(NORDICSDK_BASE)/components/libraries/uart
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/hal
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/delay
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/common
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/gpiote
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/uart
VPATH   += $(NORDICSDK_BASE)/components/drivers_nrf/pstorage
VPATH   += $(NORDICSDK_BASE)/examples/bsp
VPATH   += $(NORDICSDK_BASE)/examples/bsp
VPATH   += main.o
VPATH   += $(NORDICSDK_BASE)/components/ble/common
VPATH   += $(NORDICSDK_BASE)/components/ble/ble_services/ble_bas
VPATH   += $(NORDICSDK_BASE)/components/ble/common
VPATH   += $(NORDICSDK_BASE)/components/ble/ble_services/ble_ias
VPATH   += $(NORDICSDK_BASE)/components/ble/ble_services/ble_ias_c
VPATH   += $(NORDICSDK_BASE)/components/ble/ble_services/ble_lls
VPATH   += $(NORDICSDK_BASE)/components/ble/common
VPATH   += $(NORDICSDK_BASE)/components/ble/ble_services/ble_tps
VPATH   += $(NORDICSDK_BASE)/components/ble/device_manager
VPATH   += $(NORDICSDK_BASE)/components/toolchain
VPATH   += $(NORDICSDK_BASE)/components/softdevice/common/softdevice_handler
VPATH   += $(NORDICSDK_BASE)/components/toolchain/gcc



OBJS    += app_button.o
OBJS    += app_error.o
OBJS    += app_timer.o
OBJS    += app_trace.o
OBJS    += nrf_assert.o
OBJS    += retarget.o
OBJS    += sensorsim.o
OBJS    += app_uart.o
OBJS    += nrf_adc.o
OBJS    += nrf_delay.o
OBJS    += nrf_drv_common.o
OBJS    += nrf_drv_gpiote.o
OBJS    += nrf_drv_uart.o
OBJS    += pstorage.o
OBJS    += bsp.o
OBJS    += bsp_btn_ble.o
OBJS    += main.o
OBJS    += ble_advdata.o
OBJS    += ble_bas.o
OBJS    += ble_conn_params.o
OBJS    += ble_ias.o
OBJS    += ble_ias_c.o
OBJS    += ble_lls.o
OBJS    += ble_srv_common.o
OBJS    += ble_tps.o
OBJS    += device_manager_peripheral.o
OBJS    += system_nrf51.o
OBJS    += softdevice_handler.o
OBJS    += gcc_startup_nrf51.o



INCLUDES   += -I../
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/config
INCLUDES   += -I$(NORDICSDK_BASE)/examples/bsp
INCLUDES   += -I$(NORDICSDK_BASE)/components/ble/ble_services/ble_ias_c
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/delay
INCLUDES   += -I$(NORDICSDK_BASE)/components/libraries/util
INCLUDES   += -I$(NORDICSDK_BASE)/components/ble/device_manager
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/uart
INCLUDES   += -I$(NORDICSDK_BASE)/components/ble/common
INCLUDES   += -I$(NORDICSDK_BASE)/components/libraries/sensorsim
INCLUDES   += -I$(NORDICSDK_BASE)/components/ble/ble_services/ble_lls
INCLUDES   += -I$(NORDICSDK_BASE)/components/ble/ble_services/ble_tps
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/pstorage
INCLUDES   += -I$(NORDICSDK_BASE)/components/libraries/uart
INCLUDES   += -I$(NORDICSDK_BASE)/components/device
INCLUDES   += -I$(NORDICSDK_BASE)/components/libraries/button
INCLUDES   += -I$(NORDICSDK_BASE)/components/libraries/timer
INCLUDES   += -I$(NORDICSDK_BASE)/components/softdevice/s110/headers
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/gpiote
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/hal
INCLUDES   += -I$(NORDICSDK_BASE)/components/toolchain/gcc
INCLUDES   += -I$(NORDICSDK_BASE)/components/toolchain
INCLUDES   += -I$(NORDICSDK_BASE)/components/drivers_nrf/common
INCLUDES   += -I$(NORDICSDK_BASE)/components/ble/ble_services/ble_ias
INCLUDES   += -I$(NORDICSDK_BASE)/components/libraries/trace
INCLUDES   += -I$(NORDICSDK_BASE)/components/ble/ble_services/ble_bas
INCLUDES   += -I$(NORDICSDK_BASE)/components/softdevice/common/softdevice_handler


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



