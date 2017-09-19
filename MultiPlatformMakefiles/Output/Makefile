
#
# Make sure we do something sensible if no target is specified.
#
.DEFAULT_GOAL := all
.SUFFIXES: .c .h .o


#########################################################################################################

#
# Define the list of platforms to build.
#
PLATFORMS=ARM HOST

#
# Include the .mk for each platform.
#
PLATFORM_MK_LIST=$(PLATFORMS:%=%.mk)
include $(PLATFORM_MK_LIST)

#
# Include the components.
#
include $(PROJECT_BASE)/Build.mk
include $(PROJECT_BASE)/ComponentOne/Build.mk
include $(PROJECT_BASE)/ComponentTwo/Build.mk

#
# Derive the platform specific objs.
#
REAL_OBJS=$(foreach platform,$(PLATFORMS), $(OBJS:.o=.$(platform).o) )

#
#
#
LIBS		+= 


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
all: Main.ARM.elf Main.HOST.elf

#
# Binary is dependent on the object files.
#
Main.HOST.elf: $(REAL_OBJS)
Main.ARM.elf: $(REAL_OBJS)

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

