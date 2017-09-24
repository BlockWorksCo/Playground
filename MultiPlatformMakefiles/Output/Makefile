
#
# Make sure we do something sensible if no target is specified.
#
.DEFAULT_GOAL := all
.SUFFIXES: .c .h .o

####################################################################################################

#
# Common tools for all platforms.
#
SHELL           = sh
RM              = rm
MKDIR           = mkdir
SIZE            = size
HEX2BIN         = hex2bin
ECHO            = echo 
COLOUR_RED      = "\033[0;31m"
COLOUR_GREEN    = "\033[0;32m"
COLOUR_RESET    = "\033[0m"


####################################################################################################

#
# Include the components.
# These files simply define the .o files that are required for the component and
# the path to find them in and their includes.
#
include $(PROJECT_BASE)/Build.mk
include $(PROJECT_BASE)/ComponentOne/Build.mk
include $(PROJECT_BASE)/ComponentTwo/Build.mk

#
# Extra libraries that need to be built in.
#
LIBS		+= 

#
# define the top-level target binaries.
#
TARGETS=Main.ARM.elf Main.HOST.elf

#
# Default target(s).
#
all: $(TARGETS)

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



####################################################################################################
# ** Beware: Macro-magic **
# The names here are of the form Main.<platform>.elf . the following commands derive
# the lists of objects and rule(s) requried to build the targets with the defined list of
# object from the component list above.
#
# The intention is that foreach target in TARGETS, we derive commands of the form:
# "PLATFORM=ARM HOST"
# "include ARM.mk HOST.mk"
# "OBJS_ARM=one.ARM.o two.ARM.o three.ARM.o"
# "OBJS_HOST=one.HOST.o two.HOST.o three.HOST.o"
# "Main.ARM.elf:$(OBJS_ARM)"
# "Main.HOST.elf:$(OBJS_HOST)"
# 
# Also, foreach object foreach platform, dependency files (.d) are created and included
# # so we can manage dependencies correctly.
#

#
# Derive the list of platform names from the binary names.
# i.e. we end up with a command of the form "PLATFORMS=ARM HOST MIPS".
#
PLATFORMS=$(patsubst Main.%.elf,%,$(TARGETS) )

#
# Include the .mk files for each platform.
# the <platform>.mk files contain the toolchain details for each platform.
# From the above list, we end up with a command of the form "include ARM.mk HOST.mk MIPS.mk"
#
PLATFORM_MK_LIST=$(PLATFORMS:%=%.mk)
include $(PLATFORM_MK_LIST)

#
# Derive the platform specific objs from the generic OBJS and assign the list to a variable with 
# a name computed from the platform.
# i.e. for each platform, we generate the a command such as "OBJS_ARM=$(OBJS:.o=ARM.o)".
#
$(foreach platform,$(PLATFORMS),$(eval OBJS_$(platform)=$(OBJS:.o=.$(platform).o)))

#
# Generate a target for each platform that depends on the objects for that platform.
# NOTE: This generates a target of the form "Main.ARM.elf: $(OBJS_ARM)"
#
$(foreach platform,$(PLATFORMS),$(eval Main.$(platform).elf:$$(OBJS_$(platform))))

#
# Generate include commands for all the targets for all the platforms.
# e.g. for each platform the following dependency includes could be generated:
# "include Main.ARM.d ComponentOne.ARM.d ComponentTwo.ARM.d"
#
$(foreach platform,$(PLATFORMS),$(eval -include $(patsubst %.$(platform).o,%.$(platform).d,$(OBJS_$(platform)))))

