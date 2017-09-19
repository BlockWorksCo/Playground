
#
# Tools
#
SHELL     		= sh
CPPC      		= arm-linux-gnueabihf-g++
CC        	    = arm-linux-gnueabihf-gcc
LD        		= arm-linux-gnueabihf-gcc
OBJCOPY   		= arm-linux-gnueabihf-objcopy
RM        		= rm
MKDIR     		= mkdir
SIZE      		= size
HEX2BIN   		= hex2bin
ECHO      		= echo
ECHO_RED  		= echo -e "\033[0;31m"
ECHO_GREEN		= echo -e "\033[0;32m"


#
# Build flags.
#
CFLAGS 			= -g 
LDFLAGS			= -g 




#
# Compile a file and then generate its dependencies.
#
%.ARM.o: %.c
	@ $(ECHO) \(ARM\) Compiling $(basename $<)
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@ $(CC) -MM $(CFLAGS) $(INCLUDES) $< -o $(notdir $(patsubst %.o,%.d,$@) )


#
# Link the objects into a binary.
#
%.ARM.elf: 
	@ $(ECHO) \(ARM\) Linking $(basename $<)
	@ $(LD) $(LDFLAGS) -Xlinker -Map=$@.map -o $@ $^ $(LIBS)
	@ $(SIZE) -d $@




