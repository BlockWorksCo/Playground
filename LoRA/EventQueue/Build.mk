
#
# Source directories
#
VPATH		+=	$(SOURCE_LIBRARY)/Implementations/EventQueue


#
# Include directories.
#
INCLUDES	+=	-I$(SOURCE_LIBRARY)/Implementations/EventQueue \
        	  	-I$(SOURCE_LIBRARY)/Interfaces/EventQueue \
        	  	-I$(SOURCE_LIBRARY)/Interfaces/Queue \
        	  	-I$(SOURCE_LIBRARY)/Interfaces/Common \


#
# Objects to build.
#
OBJS		+=	EventQueue.o
    		  		

#
# Libraries.
#
LIBS		+=

