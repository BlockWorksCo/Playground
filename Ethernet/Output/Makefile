

SOURCE_LIBRARY=../../../ProjectLayout/SourceLibrary


ECHO    = /bin/echo
CC      = arm-none-eabi-gcc
LD      = arm-none-eabi-gcc
AS      = arm-none-eabi-gcc
RM      = rm



include ../Build.mk
include $(SOURCE_LIBRARY)/Implementations/BluePillBoardSupport/Build.mk
include $(SOURCE_LIBRARY)/Implementations/ErrorHandling/Build.mk
include $(SOURCE_LIBRARY)/Implementations/EventQueue/Build.mk
include $(SOURCE_LIBRARY)/Implementations/NullDebugText/Build.mk
include $(SOURCE_LIBRARY)/Implementations/BluePillSPIMaster/Build.mk
include $(SOURCE_LIBRARY)/Implementations/TimedEvents/Build.mk



all: app.elf

clean:
	@$(ECHO) Removing object files.
	@-$(RM) *.o
	@$(ECHO) Removing ELF files.
	@-$(RM) *.elf

app.elf: $(OBJS)
	@$(ECHO) Linking $@.
	@$(LD) $(LDFLAGS) -o $@ $^





.c.o:
	@$(ECHO) Compiling $(notdir $^)
	@$(CC) $(CFLAGS) $(INCLUDES) -o $@ -c $^


.s.o:
	@$(ECHO) Assembling $(notdir $^)
	@$(AS) $(AFLAGS) -o $@ -c $<



