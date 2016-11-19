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

	printf("gpio mapped to %p\n", regAddrMap);

	return (uint32_t*)(regAddrMap + GPIO_BASEOffsetIntoPage);
}



//
//
//
int main()
{
	uint32_t 	start;
	uint32_t	end;

	SetupGPIO();

	while(true)
	{
		start 	= rdtsc32();
		for(uint32_t i=0; i<1000; i++)
		{
			static volatile 	uint32_t i 	= 0;
			i++;
		}
		end 	= rdtsc32();
		
		printf("diff = %d\n", end-start);
	}

}


