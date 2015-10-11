
#
#
#
BLOCKWORKS_PLATFORM_BASE	= $(BLOCKWORKS_BASE)/Platform


#
#
#


#
# Make sure we do something sensible if no target is specified.
#
.DEFAULT_GOAL := all
.SUFFIXES: .c .h .o .cpp
SHELL		= sh


#########################################################################################################

#
# Tools
#
CPPC      		= arm-none-eabi-g++
CC        	    = arm-none-eabi-gcc
LD        		= arm-none-eabi-gcc
AS        		= arm-none-eabi-as
OBJCOPY   		= arm-none-eabi-objcopy
RM        		= rm
MKDIR     		= mkdir
SIZE      		= size
HEX2BIN   		= hex2bin
HEXSPLIT  		= perl $(CORE_LIBRARY)/Tools/HexSplit.pl
ECHO      		= echo
ECHO_RED  		= echo -e "\033[0;31m"
ECHO_GREEN		= echo -e "\033[0;32m"


######################################################################################################

CFLAGS	+= -g -Ofast -ftree-loop-optimize -funroll-loops -Wall -T$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/stm32_ram.ld -mlittle-endian -mthumb -mthumb-interwork -mcpu=cortex-m4 -fsingle-precision-constant -Wdouble-promotion -mfpu=fpv4-sp-d16 -mfloat-abi=hard -nostdlib -DSTM32F40XX -DUSE_STDPERIPH_DRIVER


######################################################################################################


INCLUDES	+=	-I$(BLOCKWORKS_BASE)/Platform/$(PLATFORM) \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Include \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Project/STM32F4xx_StdPeriph_Examples/CortexM/MPU


VPATH		+=	$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM) \
     		  	$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src

OBJS		+=	system_stm32f4xx.o \
    		  	startup_stm32f4xx.o \
    		  	syscalls.o \
    		  	Support.o \
    		  	stm32f4xx_usart.o \
    		  	stm32f4xx_rcc.o \
    		  	stm32f4xx_gpio.o \
    		  	stm32f4xx_dac.o \
    		  	stm32f4xx_tim.o \
    		  	misc.o \
    		  	Main.o



#########################################################################################################


#
# Include the dependency rules for each target iff already generated.
# If they haven't been generated, then we have to build anyway.
#
-include $(notdir $(patsubst %.o,%.d,$(OBJS)) )


######################################################################################################


#
#
#
clean:
	-@ $(RM) -f Main.elf
	-@ $(RM) -f *.o


#
#
#
all: Main.elf

#
#
#
Main.elf: $(OBJS)
	@ $(LD) $(CFLAGS) $(INCLUDES) -o $@ $^


######################################################################################################


#
# Compile a file and then generate its dependencies.
#
%.o: %.c
	@ echo Building $(basename $(notdir $<) )
	@ $(CC) $(CFLAGS) -std=c99 $(INCLUDES) -c $< -o $@
	@ $(CC) -MM $(CFLAGS) -std=c99 $(INCLUDES) $< -o $(notdir $(patsubst %.o,%.d,$@) )


#
# Compile a file and then generate its dependencies.
#
%.o: %.cpp
	@ echo Building $(basename $(notdir $<) )
	@ $(CC) $(CFLAGS) -std=c++11 $(INCLUDES) -c $< -o $@
	@ $(CC) -MM $(CFLAGS) -std=c++11 -fno-rtti -fno-exceptions $(INCLUDES) $< -o $(notdir $(patsubst %.o,%.d,$@) )

#
# Compile a file and then generate its dependencies.
#
%.o: %.s
	@ echo Assembling $(basename $(notdir $<) )
	@ $(AS) -c $< -o $@




