



#ifndef __HALO_H__
#define __HALO_H__


#define HALO_IDENTITY           (0x00000001)
#define HALO_HEARTBEAT          (0x00000002)


//
//
//
typedef struct __attribute__ ((__packed__))
{
    uint32_t    timestamp;
    uint32_t    type;
    uint32_t    numberOfPayloadBytes;
    //uint8_t     payload[1];

} HaloEvent;




//
//
//
typedef void (*EventNotificationHandler)(HaloEvent*);


#endif
