
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
CPPC      		= arm-linux-gnueabi-g++
CC        	    = arm-linux-gnueabi-gcc
LD        		= arm-linux-gnueabi-gcc
AS        		= arm-linux-gnueabi-as
OBJCOPY   		= arm-linux-gnueabi-objcopy
RM        		= rm
MKDIR     		= mkdir
SIZE      		= size
HEX2BIN   		= hex2bin
HEXSPLIT  		= perl $(CORE_LIBRARY)/Tools/HexSplit.pl
ECHO      		= echo
ECHO_RED  		= echo -e "\033[0;31m"
ECHO_GREEN		= echo -e "\033[0;32m"


######################################################################################################

CFLAGS	+= -g -Ofast


######################################################################################################


INCLUDES	+=	-I$(BLOCKWORKS_BASE)/Platform/$(PLATFORM) \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Device/ST/STM32F4xx/Include \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/inc \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/CMSIS/Include \
        	  	-I$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Project/STM32F4xx_StdPeriph_Examples/CortexM/MPU


VPATH		+=	$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM) \
     		  	$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)/CMSIS/STM32F4xx_DSP_StdPeriph_Lib_V1.1.0/Libraries/STM32F4xx_StdPeriph_Driver/src

OBJS		+=	Main.o



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
	-@ $(RM) -f *.d


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




