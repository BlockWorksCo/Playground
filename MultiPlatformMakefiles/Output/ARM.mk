
#
# Tools
#
ARM_CPPC      		= arm-linux-gnueabihf-g++
ARM_CC        	    = arm-linux-gnueabihf-gcc
ARM_LD        		= arm-linux-gnueabihf-gcc
ARM_OBJCOPY   		= arm-linux-gnueabihf-objcopy


#
# Build flags.
#
ARM_CFLAGS 			= -g 
ARM_LDFLAGS			= -g 




#
# Compile a file and then generate its dependencies.
#
%.ARM.o: %.c
	@ $(ECHO) \(ARM\) Compiling $(basename $<)
	@ $(ARM_CC) $(ARM_CFLAGS) $(INCLUDES) -c $< -o $@
	@ $(ARM_CC) -MT $@ -MM $(ARM_CFLAGS) $(INCLUDES) $< -o $(notdir $(patsubst %.o,%.d,$@) )


#
# Link the objects into a binary.
#
%.ARM.elf: 
	@ $(ECHO) \(ARM\) Linking $(basename $<)
	@ $(ARM_LD) $(ARM_LDFLAGS) -Xlinker -Map=$@.map -o $@ $^ $(LIBS)
	@ $(SIZE) -d $@




