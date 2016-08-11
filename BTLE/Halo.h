



#ifndef __HALO_H__
#define __HALO_H__





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
