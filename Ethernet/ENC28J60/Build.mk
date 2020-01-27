
#
# Source directories
#
#VPATH		+= $(SOURCE_LIBRARY)/Implementations/ENC28J60
VPATH		+= ../ENC28J60



#
# Include directories.
#
INCLUDES	+= -I$(SOURCE_LIBRARY)/Implementations/ENC28J60
INCLUDES	+= -I$(SOURCE_LIBRARY)/Interfaces/Assertions
INCLUDES	+= -I$(SOURCE_LIBRARY)/Interfaces/Ethernet
INCLUDES	+= -I../ENC28J60


#
# Objects to build.
#
OBJS    += ENC28J60.o


    		  		

#
# Libraries.
#
LIBS		+=

