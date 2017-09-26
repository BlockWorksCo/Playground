

#
# Tools
#
TEST_CPPC      		= g++
TEST_CC        	    = gcc
TEST_LD        		= gcc
TEST_OBJCOPY   		= objcopy


#
# Build flags.
#
TEST_CFLAGS 		= -g 
TEST_LDFLAGS		= -g 




#
# Compile a file and then generate its dependencies.
#
%.TEST.o: %.c
	@ $(ECHO) $(COLOUR_GREEN)\(TEST\) Compiling $(basename $<) $(COLOUR_RESET)
	@ $(TEST_CC) $(TEST_CFLAGS) $(INCLUDES) -c $< -o $@
	@ $(TEST_CC) -MT $@ -MM $(TEST_CFLAGS) $(INCLUDES) $< -o $(notdir $(patsubst %.o,%.d,$@) )



#
# Link the objects into a binary.
#
%.TEST.elf: 
	@ $(ECHO) $(COLOUR_GREEN)\(TEST\) Linking $(basename $<) $(COLOUR_RESET)
	@ $(TEST_LD) $(TEST_LDFLAGS) -Xlinker -Map=$@.map -o $@ $^ $(LIBS)
	@ $(SIZE) -d $@




