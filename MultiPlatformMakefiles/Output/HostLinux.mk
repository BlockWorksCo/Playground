
#
# Tools
#
SHELL     		= sh
CPPC      		= g++
CC        	    = gcc
LD        		= gcc
OBJCOPY   		= objcopy
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
%.o: %.c
	@ $(ECHO) Building $(basename $<)
	@ $(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@
	@ $(CC) -MM $(CFLAGS) $(INCLUDES) $< -o $(notdir $(patsubst %.o,%.d,$@) )


