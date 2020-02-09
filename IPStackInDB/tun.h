

#ifndef __TUN_H__
#define __TUN_H__


extern int tun_fd;

int tun_alloc(char* dev, int flags);

#endif

