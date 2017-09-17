
#
# Make sure we do something sensible if no target is specified.
#
.DEFAULT_GOAL := all
.SUFFIXES: .c .h .o


#########################################################################################################


include $(TARGET).mk

include $(PROJECT_BASE)/Build.mk
include $(PROJECT_BASE)/ComponentOne/Build.mk
include $(PROJECT_BASE)/ComponentTwo/Build.mk


#
#
#
LIBS		+= -lgcc -lc


#########################################################################################################


#
# Include the dependency rules for each target iff already generated.
# If they haven't been generated, then we have to build anyway.
#
-include $(notdir $(patsubst %.o,%.d,$(OBJS)) )


#########################################################################################################


#
# Default target.
#
all: Main.elf

#
# Binary is dependent on the object files.
#
Main.elf: $(OBJS)

#
# Remove all the build artifacts.
#
clean:
	@ $(ECHO) Cleaning.
	-@ $(RM) -f *.d
	-@ $(RM) -f *.o
	-@ $(RM) -f *.a
	-@ $(RM) -f *.bin
	-@ $(RM) -f *.hex
	-@ $(RM) -f *.elf
	-@ $(RM) -f *.map

