


CC 			= arm-none-eabi-gcc
LD 			= arm-none-eabi-gcc
AS 			= arm-none-eabi-gcc
RM 			= rm



DEFINES 	+= -DBOARD_PCA10001
DEFINES 	+= -DSOFTDEVICE_PRESENT
DEFINES 	+= -DNRF51
DEFINES 	+= -DS110
DEFINES 	+= -DBLE_STACK_SUPPORT_REQD
DEFINES 	+= -DSWI_DISABLE0

CFLAGS 		+= -mcpu=cortex-m0
CFLAGS 		+= -mthumb
CFLAGS 		+= -mabi=aapcs
CFLAGS 		+= --std=gnu99
CFLAGS 		+= -Wall
CFLAGS 		+= -Werror
CFLAGS 		+= -O3
CFLAGS 		+= -g3
CFLAGS 		+= -mfloat-abi=soft

INCLUDES 	+= -I./NRF51SDK/components/ble/common
INCLUDES 	+= -I./NRF51SDK/components/ble/ble_advertising
INCLUDES 	+= -I./NRF51SDK/components/ble/ble_services/ble_nus
INCLUDES 	+= -I./NRF51SDK/components/softdevice/s110/headers
INCLUDES	+= -I./NRF51SDK/components/libraries/util
INCLUDES	+= -I./NRF51SDK/components/libraries/uart
INCLUDES	+= -I./NRF51SDK/components/libraries/fifo
INCLUDES	+= -I./NRF51SDK/components/libraries/trace
INCLUDES 	+= -I./NRF51SDK/components/device
INCLUDES 	+= -I./NRF51SDK/components/softdevice/common/softdevice_handler
INCLUDES 	+= -I./NRF51SDK/components/toolchain/gcc
INCLUDES 	+= -I./NRF51SDK/components/toolchain
INCLUDES 	+= -I./NRF51SDK/examples/bsp
INCLUDES 	+= -I./NRF51SDK/components/drivers_nrf/hal
INCLUDES 	+= -I./NRF51SDK/components/libraries/button
INCLUDES 	+= -I./NRF51SDK/components/libraries/timer
INCLUDES 	+= -I./NRF51SDK/components/drivers_nrf/delay
INCLUDES 	+= -I./NRF51SDK/components/drivers_nrf/gpiote
INCLUDES 	+= -I./NRF51SDK/components/drivers_nrf/config
INCLUDES 	+= -I./NRF51SDK/components/drivers_nrf/common
INCLUDES 	+= -I./NRF51SDK/components/drivers_nrf/uart
INCLUDES 	+= -I./NRF51SDK/components/drivers_nrf/pstorage
INCLUDES 	+= -I./NRF51SDK/examples/ble_peripheral/ble_app_uart/config

LIBDIRS 	+= -L./NRF51SDK/components/toolchain/gcc

LDFLAGS 	+= -mthumb
LDFLAGS 	+= -mabi=aapcs
LDFLAGS 	+= -mcpu=cortex-m0
LDFLAGS 	+= --specs=nano.specs
LDFLAGS		+= -lnosys
LDFLAGS		+= -T Main.ld
LDFLAGS 	+= -Wl,--no-wchar-size-warning

LIBS 		+= -lc

OBJECTS 	+= Main.o
OBJECTS 	+= softdevice_handler.o
OBJECTS 	+= app_timer.o
OBJECTS 	+= app_button.o
OBJECTS 	+= bsp.o
OBJECTS 	+= ble_advdata.o
OBJECTS		+= nrf_drv_gpiote.o
OBJECTS 	+= nrf_drv_common.o
OBJECTS 	+= app_error.o
OBJECTS 	+= gcc_startup_nrf51.o
OBJECTS 	+= system_nrf51.o
OBJECTS 	+= app_fifo.o
OBJECTS 	+= app_trace.o
OBJECTS 	+= retarget.o
OBJECTS 	+= app_uart_fifo.o
OBJECTS 	+= nrf_delay.o
OBJECTS 	+= nrf_drv_uart.o
OBJECTS 	+= pstorage.o
OBJECTS 	+= bsp_btn_ble.o
OBJECTS 	+= ble_advdata.o
OBJECTS 	+= ble_advertising.o
OBJECTS 	+= ble_conn_params.o
OBJECTS 	+= ble_nus.o
OBJECTS 	+= ble_srv_common.o



VPATH 		+= ./NRF51SDK/components/softdevice/common/softdevice_handler
VPATH 		+= ./NRF51SDK/components/libraries/timer
VPATH 		+= ./NRF51SDK/components/libraries/button
VPATH 		+= ./NRF51SDK/components/libraries/uart
VPATH 		+= ./NRF51SDK/components/libraries/fifo
VPATH 		+= ./NRF51SDK/components/libraries/trace
VPATH 		+= ./NRF51SDK/examples/bsp
VPATH 		+= ./NRF51SDK/components/ble/common
VPATH 		+= ./NRF51SDK/components/ble/ble_advertising
VPATH 		+= ./NRF51SDK/components/ble/ble_services/ble_nus
VPATH 		+= ./NRF51SDK/components/drivers_nrf/gpiote
VPATH 		+= ./NRF51SDK/components/drivers_nrf/common
VPATH 		+= ./NRF51SDK/components/libraries/util
VPATH 		+= ./NRF51SDK/components/toolchain/gcc
VPATH 		+= ./NRF51SDK/components/toolchain
VPATH 		+= ./NRF51SDK/components/drivers_nrf/delay
VPATH 		+= ./NRF51SDK/components/drivers_nrf/uart
VPATH 		+= ./NRF51SDK/components/drivers_nrf/pstorage

clean:
	- $(RM) *.o
	- $(RM) *.elf

all: Main.elf




Main.elf: $(OBJECTS)
	$(LD) $(LDFLAGS) $(LIBDIRS) -o $@ $^


%.o : %.c
	$(CC) $(CFLAGS) $(DEFINES) $(INCLUDES) -c $< -o $@

%.o : %.s
	$(AS) -x assembler-with-cpp $(DEFINES) $(INCLUDES) -c $< -o $@
