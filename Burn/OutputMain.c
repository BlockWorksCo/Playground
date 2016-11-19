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


void SetupGPIO()
{
	const unsigned long	GPIO_BASE 	= 0x01C20000;

  	int		mem_fd		= 0;
  	void*		regAddrMap 	= MAP_FAILED;

    		if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
		{
     			perror("can't open /dev/mem");
      			exit (1);
    		}

   	/* mmap IO */
  	regAddrMap = mmap(
      		NULL,             		//Any adddress in our space will do
      		8192,       			//Map length
      		PROT_READ|PROT_WRITE,	// Enable reading & writting to mapped memory
      		MAP_PRIVATE,       	//Shared with other processes
      		mem_fd,           	//File to map
      		GPIO_BASE         	//Offset to base address
  		);

  	if (regAddrMap == MAP_FAILED) 
	{
          	perror("mmap error");
          	close(mem_fd);
          	exit (1);
  	}

	printf("gpio mapped to %p\n", regAddrMap);
}



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


