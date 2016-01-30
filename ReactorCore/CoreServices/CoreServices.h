/*
 *  chardev.h - the header file with the ioctl definitions.
 *
 *  The declarations here have to be in a header file, because
 *  they need to be known both to the kernel module
 *  (in chardev.c) and the process calling ioctl (ioctl.c)
 */

#ifndef __CORESERVICES_H__
#define __CORESERVICES_H__



#include <linux/ioctl.h>

/* 
 * The major device number. We can't rely on dynamic 
 * registration any more, because ioctls need to know 
 * it. 
 */
#define MAJOR_NUM 100

/* 
 * Set the message of the device driver 
 */
#define IOCTL_SET_MSG _IOR(MAJOR_NUM, 0, char *)
/*
 * _IOR means that we're creating an ioctl command 
 * number for passing information from a user process
 * to the kernel module. 
 *
 * The first arguments, MAJOR_NUM, is the major device 
 * number we're using.
 *
 * The second argument is the number of the command 
 * (there could be several with different meanings).
 *
 * The third argument is the type we want to get from 
 * the process to the kernel.
 */

/* 
 * Get the message of the device driver 
 */
#define IOCTL_GET_MSG _IOR(MAJOR_NUM, 1, char *)
/* 
 * This IOCTL is used for output, to get the message 
 * of the device driver. However, we still need the 
 * buffer to place the message in to be input, 
 * as it is allocated by the process.
 */

/* 
 * Get the n'th byte of the message 
 */
#define IOCTL_GET_NTH_BYTE _IOWR(MAJOR_NUM, 2, int)

#define IOCTL_GET_BRIDGE_ADDRESS _IOWR(MAJOR_NUM, 3, int)

#define IOCTL_SEND_MAIL _IOWR(MAJOR_NUM, 4, int)

#define IOCTL_ALLOCATE_BLOCK _IOWR(MAJOR_NUM, 5, int)

#define IOCTL_FLUSH_AND_INVALIDATE _IOWR(MAJOR_NUM, 6, int)



/* 
 * The IOCTL is used for both input and output. It 
 * receives from the user a number, n, and returns 
 * Message[n]. 
 */

/* 
 * The name of the device file 
 */
#define DEVICE_FILE_NAME "/dev/ReactorCoreServices"



typedef struct
{
    volatile uint32_t    heartBeats[4];

} CoreServicesBridge;


//
//
//
typedef struct
{
    uint32_t    size;
    uint32_t    virtualAddress;
    uint32_t    physicalAddress;  

} AllocateRequest;






#define UNCACHED_AREA                   (0x00000000)
#define ALLOY_DEDICATED_RAM_SIZE        (1024*1024*706)
#define ALLOY_RAM_BASE                  UNCACHED_AREA+(256*1024*1024)


#endif



