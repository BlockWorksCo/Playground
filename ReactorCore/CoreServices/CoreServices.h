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





#define IOCTL_START_CORE    _IOWR(MAJOR_NUM, 0, char *)
#define IOCTL_SEND_MAIL     _IOWR(MAJOR_NUM, 1, int)




/* 
 * The IOCTL is used for both input and output. It 
 * receives from the user a number, n, and returns 
 * Message[n]. 
 */

/* 
 * The name of the device file 
 */
#define DEVICE_FILE_NAME "/dev/ReactorCoreServices"


#define NUMBER_OF_CORES         (4)

#define CORE_MESSAGE_NONE       ((uint32_t)-1)
#define CORE_MESSAGE_RESET      (0)
#define CORE_MESSAGE_TEST       (1)

//
//
//
typedef struct
{
    uint32_t    type;
    uint32_t    payload;

} CoreMessage;

//
//
//
typedef struct
{
    volatile CoreMessage    coreMessages[NUMBER_OF_CORES][NUMBER_OF_CORES];
    volatile uint32_t       heartBeats[NUMBER_OF_CORES];
    volatile uint32_t       messageCounts[NUMBER_OF_CORES];

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


//
//
//
typedef struct
{
    uint32_t    coreID;
    uint32_t    startPoint;
    
} CoreStartData;



#define UNCACHED_AREA                   (0x00000000)
#define ALLOY_DEDICATED_RAM_SIZE        (1024*1024*706)
#define ALLOY_RAM_BASE                  (UNCACHED_AREA+(256*1024*1024))


#endif



