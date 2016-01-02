//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, November 2015.
//


#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/moduleparam.h>
#include <linux/workqueue.h>
#include <linux/time.h>
#include <asm/mutex.h>
#include <linux/cpu.h> 
#include <asm/io.h>
#include <mach/hardware.h>
#include <asm/cputype.h>
//#include <asm/smp.h>
//#include <asm/smp_plat.h>

#include <linux/slab.h>



uint8_t*            stuff                   = NULL;
uint32_t            stuffPhysicalAddress    = 0;

__naked void EntryPoint(void)
{
    //static volatile uint32_t*  sharedLocation    = (uint32_t*)(0x400000a0);

    while(1)
    {
        //(*sharedLocation)++;
    }
}



//
// https://www.raspberrypi.org/forums/viewtopic.php?f=72&t=98904&start=25
// http://pastebin.com/rgGgBuTN
//
void BootCore(uint32_t cpu)
{
    void*       mbox_set            = __io_address(0x4000008C + (0x10*cpu) );
    uint32_t    EntryPointAddress   = (uint32_t)__virt_to_phys((void *)EntryPoint);

    dsb();
    printk("EntryPointAddress = %08x\n", EntryPointAddress);
    //EntryPointAddress   = 0x01000000;
    writel(EntryPointAddress, mbox_set);
}



//
// Make PMU accessible from user space when module is loaded
//
static int __init ModuleInit(void)
{
    stuff   = (uint8_t*)kmalloc( 1024*1024, __GFP_DMA );
    if( stuff != NULL )
    {
        uint32_t    online      = num_online_cpus();
        uint32_t    possible    = num_possible_cpus();
        uint32_t    present     = num_present_cpus();

        printk (KERN_INFO "Online Cpus   : %d\n",     online   );
        printk (KERN_INFO "Possible Cpus : %d\n",     possible );
        printk (KERN_INFO "Present Cpus  : %d\n",     present  );

        //on_each_cpu(EnablePMUAccessFromUserSpace, NULL, 1);

        //printk ("User mode cycle counter access enabled.\n");

        stuffPhysicalAddress   = virt_to_phys(stuff);

        printk("stuff = %08x stuffPhysicalAddress = %08x\n", (uint32_t)stuff, (uint32_t)stuffPhysicalAddress );

        BootCore(1);
    }
    else
    {
        printk("kmalloc failed.");
    }


    return 0;
}

//
// Disable PMU access from user space when module is unloaded
//
static void __exit ModuleExit(void)
{
    if(stuff != NULL)
    {
        kfree(stuff);
    }
    //on_each_cpu(DisablePMUAccessFromUserSpace, NULL, 1);

    printk ("User mode cycle counter access disabled.\n");
}



 
module_init(ModuleInit);
module_exit(ModuleExit);


MODULE_LICENSE("BlockWorks");
MODULE_AUTHOR("Steve Tickle");
MODULE_DESCRIPTION("Trigger a boot of another core.");
