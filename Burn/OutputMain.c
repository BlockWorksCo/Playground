//
// https://github.com/thoughtpolice/enable_arm_pmu.git
//




#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <limits.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#include "armpmu_lib.h"


//
//
//
typedef struct
{
	uint32_t	CFG0;
	uint32_t	CFG1;
	uint32_t	CFG2;
	uint32_t	CFG3;

	uint32_t	DAT;
	uint32_t	DRV0;
	uint32_t	DRV1;

	uint32_t	PUL0;
	uint32_t	PUL1;

} GPIOPort;


//
//
//
uint32_t* SetupGPIO()
{
	const unsigned long	GPIO_BASE 		= 0x01C20800;
	const unsigned long 	PAGE_SIZE 		= 4096;
	const unsigned long	GPIO_BASEPage 		= GPIO_BASE & ~(PAGE_SIZE-1);
	uint32_t 		GPIO_BASEOffsetIntoPage	= GPIO_BASE - GPIO_BASEPage;
  	int			mem_fd			= 0;
  	void*			regAddrMap 		= MAP_FAILED;


	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
	{
		perror("can't open /dev/mem");
		exit (1);
	}

  	regAddrMap = mmap(
      		NULL,          
      		8192,       	
      		PROT_READ|PROT_WRITE,
      		MAP_PRIVATE,     
      		mem_fd,           
		GPIO_BASEPage);

  	if (regAddrMap == MAP_FAILED) 
	{
          	perror("mmap error");
          	close(mem_fd);
          	exit (1);
  	}

	printf("gpio mapped to %p = %08x\n", regAddrMap, (uint32_t)GPIO_BASEPage);

	return (uint32_t*)(regAddrMap + GPIO_BASEOffsetIntoPage);
}



//
//
//
int main()
{
	uint32_t 	start;
	uint32_t	end;
	GPIOPort* 	gpio 	= (GPIOPort*)SetupGPIO();
	GPIOPort*	portA	= &gpio[0];
	GPIOPort*	portC	= &gpio[1];
	GPIOPort*	portD	= &gpio[2];
	GPIOPort*	portE	= &gpio[3];
	GPIOPort*	portF	= &gpio[4];
	GPIOPort*	portG	= &gpio[5];
	GPIOPort*	portL	= &gpio[6];

	printf("BASE = %08x\n",(uint32_t)gpio);
	printf("&CFG1 = %08x\n",(uint32_t)&portA->CFG1);
	printf("CFG1=%08x\n",portA->CFG1);
	portA->CFG1 	&= ~(0xf0000000);
	portA->CFG1 	|= 0x10000000;
	printf("CFG1=%08x\n",portA->CFG1);

	printf("DRV0=%08x\n",portA->DRV0);
	portA->DRV0 	&= ~(0xA0000000);
	portA->DRV0 	|= 0x10000000;
	printf("DRV0=%08x\n",portA->DRV0);

	while(true)
	{
		//portA->DAT 	|= 1<<16;
		portA->DAT 	= 0xffffffff;
		printf("%08x\n", portA->DAT);
		sleep(1);
		//portA->DAT 	&= ~(1<<16);
		portA->DAT 	= 0;
		printf("%08x\n", portA->DAT);
		sleep(1);

#if 0
		start 	= rdtsc32();
		for(uint32_t i=0; i<1000; i++)
		{
			static volatile 	uint32_t i 	= 0;
			i++;
		}
		end 	= rdtsc32();
		
		printf("diff = %d\n", end-start);
#endif
	}

}


