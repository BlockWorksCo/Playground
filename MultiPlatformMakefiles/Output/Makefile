
#
# Make sure we do something sensible if no target is specified.
#
.DEFAULT_GOAL := all
.SUFFIXES: .c .h .o


#########################################################################################################

#
# define the binaries to output.
#
TARGETS=Main.ARM.elf Main.HOST.elf

#
# Derive the list of platform names from the binary names.
#
PLATFORMS=$(patsubst Main.%.elf,%,$(TARGETS) )

#
# Include the .mk files for each platform.
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
$(foreach platform,$(PLATFORMS),$(eval OBJS_$(platform)=$(OBJS:.o=.$(platform).o)))
$(info OBJS_ARM is $(OBJS_ARM) )
$(info OBJS_HOST is $(OBJS_HOST) )

$(foreach platform,$(PLATFORMS),$(info Main.$(platform).elf:$OBJS_$(platform)))

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
all: $(TARGETS)

#
# Binary is dependent on the object files.
#
$(TARGETS): $(REAL_OBJS)

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

