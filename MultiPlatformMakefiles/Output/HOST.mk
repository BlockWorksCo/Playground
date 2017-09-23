

#
# Tools
#
HOST_CPPC      		= g++
HOST_CC        	    = gcc
HOST_LD        		= gcc
HOST_OBJCOPY   		= objcopy


#
# Build flags.
#
HOST_CFLAGS 		= -g 
HOST_LDFLAGS		= -g 




#
# Compile a file and then generate its dependencies.
#
%.HOST.o: %.c
	@ $(ECHO) \(HOST\) Compiling $(basename $<)
	@ $(HOST_CC) $(HOST_CFLAGS) $(INCLUDES) -c $< -o $@
	@ $(HOST_CC) -MT $@ -MM $(HOST_CFLAGS) $(INCLUDES) $< -o $(notdir $(patsubst %.o,%.d,$@) )



#
# Link the objects into a binary.
#
%.HOST.elf: 
	@ $(ECHO) \(HOST\) Linking $(basename $<)
	@ $(HOST_LD) $(HOST_LDFLAGS) -Xlinker -Map=$@.map -o $@ $^ $(LIBS)
	@ $(SIZE) -d $@




