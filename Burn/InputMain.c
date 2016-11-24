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
	volatile uint32_t	CFG0;
	volatile uint32_t	CFG1;
	volatile uint32_t	CFG2;
	volatile uint32_t	CFG3;

	volatile uint32_t	DAT;
	volatile uint32_t	DRV0;
	volatile uint32_t	DRV1;

	volatile uint32_t	PUL0;
	volatile uint32_t	PUL1;

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
			PROT_READ|PROT_WRITE|PROT_EXEC,// Enable reading & writting to mapped memory
			MAP_SHARED,       //Shared with other processes
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
typedef struct
{
	volatile uint32_t	reserved0;

	volatile uint32_t	CTL;			// 0x04 Global Control
	volatile uint32_t	INTCTL;			// 0x08 Transfer Conrol

	volatile uint32_t	reserved1;		// 0x0c

	volatile uint32_t	IER;			// 0x10 Interrupt control
	volatile uint32_t	INT_STA;		// 0x14 Interrupt status
	volatile uint32_t	FCR;			// 0x18 FIFO control
	volatile uint32_t	FSR;			// 0x1c FIFO status
	volatile uint32_t	WAIT;			// 0x20 wait clock counter
	volatile uint32_t	CCTL;			// 0x24 Clock rate control

	volatile uint32_t	reserved2;		// 0x28
	volatile uint32_t	reserved3;		// 0x2c

	volatile uint32_t	BC;				// 0x30 burst counter
	volatile uint32_t	TC;				// 0x3c transmit counter
	volatile uint32_t	BCC;			//  0x38 Burst control

	volatile uint8_t	reserved4[0x1c0];// 0x40

	volatile uint32_t	TXD;			// 0x200

	volatile uint8_t	reserved5[0xfc];// 0x204

	volatile uint32_t	RXD;			// 0x300

} SPIPort;


//
//
//
uint32_t* SetupSPI()
{
	//const unsigned long	BASE 					= 0x01C68000;	// SPI0
	const unsigned long	BASE 					= 0x01C69000; 	// SPI1
	const unsigned long PAGE_SIZE 				= 4096;
	const unsigned long	BASEPage 				= BASE & ~(PAGE_SIZE-1);
	uint32_t 			BASEOffsetIntoPage		= BASE - BASEPage;
  	int					mem_fd					= 0;
  	void*				regAddrMap 				= MAP_FAILED;


	if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
	{
		perror("can't open /dev/mem");
		exit (1);
	}

  	regAddrMap = mmap(
      		NULL,          
      		8192,       	
			PROT_READ|PROT_WRITE|PROT_EXEC,// Enable reading & writting to mapped memory
			MAP_SHARED,       //Shared with other processes
      		mem_fd,           
			BASEPage);

  	if (regAddrMap == MAP_FAILED) 
	{
          	perror("mmap error");
          	close(mem_fd);
          	exit (1);
  	}

	printf("mapped to %p = %08x\n", regAddrMap, (uint32_t)BASEPage);

	return (uint32_t*)(regAddrMap + BASEOffsetIntoPage);
}





volatile GPIOPort* 	gpio 	= 0;
volatile SPIPort* 	spi 	= 0;
volatile GPIOPort*	portA	= 0;
volatile GPIOPort*	portC	= 0;
volatile GPIOPort*	portD	= 0;
volatile GPIOPort*	portE	= 0;
volatile GPIOPort*	portF	= 0;
volatile GPIOPort*	portG	= 0;
volatile GPIOPort*	portL	= 0;


//
//
//
int main()
{
	uint32_t 			start;
	uint32_t			end;

	gpio 	= (GPIOPort*)SetupGPIO();
	spi 	= (SPIPort*)SetupSPI();

	portA	= &gpio[0];
	portC	= &gpio[1];
	portD	= &gpio[2];
	portE	= &gpio[3];
	portF	= &gpio[4];
	portG	= &gpio[5];
	portL	= &gpio[6];

	portA->CFG1 	&= ~0xfff00000;
	portA->CFG1 	|=  0x22200000;

	portA->CFG2 	&= ~0x0000000f;
	portA->CFG2 	|=  0x00000002;

	spi->CTL 	|= 0x80000000;
	while( (spi->CTL&0x80000000)  != 0);

	spi->CTL 	|= 0x00000003;

	printf("CTL=%08x\n", spi->CTL);
	printf("INTCTL=%08x\n", spi->INTCTL);
	printf("IER=%08x\n", spi->IER);
	printf("INT_STA=%08x\n", spi->INT_STA);
	printf("FCR=%08x\n", spi->FCR);
	printf("FSR=%08x\n", spi->FSR);
	printf("CCTL=%08x\n", spi->CCTL);

	uint32_t 	i 	= 0;
	while(true)
	{
		printf("FSR=%08x\n", spi->FSR);
		printf("INT_STA=%08x\n", spi->INT_STA);
		spi->TXD 	= i;
		i++;
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


