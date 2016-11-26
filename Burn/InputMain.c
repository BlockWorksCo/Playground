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
#include <stddef.h>
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

    volatile uint8_t	reserved4[0x1c4];// 0x40

    volatile uint32_t	TXD;			// 0x200

    volatile uint8_t	reserved5[0xfc];// 0x204

    volatile uint32_t	RXD;			// 0x300

} SPIPort;


//
//
//
SPIPort* SetupSPI()
{
    const uint32_t	    BASE 					= 0x01C68000;	// SPI0
    const uint32_t  	PAGE_SIZE 				= 4096;
    const uint32_t		BASEPage 				= BASE & ~(PAGE_SIZE-1);
    uint32_t 			BASEOffsetIntoPage		= BASE - BASEPage;
    int					mem_fd					= 0;
    void*				regAddrMap 				= MAP_FAILED;


    printf("BASE 						= %08x\n", BASE);
    printf("BASEPage 					= %08x\n", BASEPage);
    printf("BASEOffsetIntoPage 			= %08x\n", BASEOffsetIntoPage);
    printf("BASE+BASEOffsetIntoPage 	= %08x\n", BASEPage+BASEOffsetIntoPage);

    if ((mem_fd = open("/dev/mem", O_RDWR|O_SYNC) ) < 0) 
    {
        perror("can't open /dev/mem");
        exit (1);
    }

      regAddrMap = mmap(
                        NULL,          
                        BASEOffsetIntoPage+(PAGE_SIZE*2),       	
                        PROT_READ|PROT_WRITE|PROT_EXEC,// Enable reading & writting to mapped memory
                        MAP_PRIVATE,       //Shared with other processes
                        mem_fd,           
                        BASEPage);

        if (regAddrMap == MAP_FAILED) 
        {
              perror("mmap error");
              close(mem_fd);
              exit (1);
        }

    printf("spi mapped to %p = %08x (+%08x)\n", regAddrMap, (uint32_t)BASEPage, BASEOffsetIntoPage);
    printf("TXD = %x\n", offsetof(SPIPort, TXD) );
    printf("RXD = %x\n", offsetof(SPIPort, RXD) );
    printf("BCC = %x\n", offsetof(SPIPort, BCC) );

    return (SPIPort*)(regAddrMap + BASEOffsetIntoPage);
}





volatile GPIOPort* 	gpio 	= 0;
volatile SPIPort* 	spi 	= 0;
volatile SPIPort*   spi0    = 0;
volatile SPIPort*   spi1    = 0;
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
    portA	= &gpio[0];
    portC	= &gpio[1];
    portD	= &gpio[2];
    portE	= &gpio[3];
    portF	= &gpio[4];
    portG	= &gpio[5];
    portL	= &gpio[6];

    spi 	= (SPIPort*)SetupSPI();
    spi0    = &spi[0];
    spi1    = &spi[1];


    //
    // Pin setup.
    //
    portA->CFG1 	&= ~0xfff00000;
    portA->CFG1 	|=  0x22200000;

    portA->CFG2 	&= ~0x0000000f;
    portA->CFG2 	|=  0x00000002;

    //
    // spiX port setup.
    //
    spi1->CTL   = 0x81;
    spi0->CTL   = 0x83;
    volatile SPIPort* spiX    = spi1;
    spiX->CTL 	= 0x00000081;
    spiX->INTCTL = 0x000001c4;
    spiX->IER 	= 0x00000000;
    spiX->INT_STA= 0x00000032;
    spiX->FCR 	= 0x00100010;
    spiX->FSR 	= 0x00000000;
    spiX->WAIT 	= 0x00000000;
    spiX->CCTL 	= 0x00001004;
    spiX->BC 	= 0x00000000;
    spiX->TC 	= 0x00000000;
    spiX->BCC 	= 0x00000000;
    msync( (void*)spiX, 4096, MS_SYNC|MS_INVALIDATE);


    printf("CTL=%08x\n", spiX->CTL);
    printf("INTCTL=%08x\n", spiX->INTCTL);
    printf("IER=%08x\n", spiX->IER);
    printf("INT_STA=%08x\n", spiX->INT_STA);
    printf("FCR=%08x\n", spiX->FCR);
    printf("FSR=%08x\n", spiX->FSR);
    printf("WAIT=%08x\n", spiX->WAIT);
    printf("CCTL=%08x\n", spiX->CCTL);
    printf("BC=%08x\n", spiX->BC);
    printf("TC=%08x\n", spiX->TC);
    printf("BCC=%08x\n", spiX->BCC);

    uint32_t 	i 	= 0;
    while(true)
    {
        printf("FSR=%08x\n", spiX->FSR);
        printf("INT_STA=%08x\n", spiX->INT_STA);
        spiX->TXD 	= i;
        i++;
        msync( (void*)spi, 8192, MS_SYNC );
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


