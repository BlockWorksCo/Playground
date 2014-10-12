


#
#
#
BLOCKWORKS_PLATFORM=$(BLOCKWORKS_PLATFORM_BASE)/$(PLATFORM)


#
#
#
INCLUDES=-I./$(PLATFORM)
INCLUDES+=-I$(BLOCKWORKS_PLATFORM)/Common
INCLUDES+=-I$(BLOCKWORKS_PLATFORM)
INCLUDES+=-I./usb_conf
INCLUDES+=-I./usb_lib/cdc
INCLUDES+=-I./usb_lib/core
INCLUDES+=-I./usb_lib/otg

#
#
#
VPATH+=./usb_conf
VPATH+=./usb_lib/cdc
VPATH+=./usb_lib/core
VPATH+=./usb_lib/otg


#
#
#
OBJS=\
main.o \
usb_bsp.o \
usbd_desc.o \
usbd_usr.o \
usbd_cdc_core.o \
usbd_cdc_vcp.o \
usbd_core.o \
usbd_ioreq.o \
usbd_req.o \
usb_core.o \
usb_dcd.o \
usb_dcd_int.o \
PlatformMain.o


include $(BLOCKWORKS_PLATFORM)/Makefile





