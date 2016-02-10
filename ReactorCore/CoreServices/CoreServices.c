//
// Copyright (C) BlockWorks Consulting Ltd - All Rights Reserved.
// Unauthorized copying of this file, via any medium is strictly prohibited.
// Proprietary and confidential.
// Written by Steve Tickle <Steve@BlockWorks.co>, November 2015.
//


#define pr_fmt(fmt) KBUILD_MODNAME ": " fmt

#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/types.h>
#include <linux/timer.h>
#include <linux/watchdog.h>
#include <linux/notifier.h>
#include <linux/reboot.h>
#include <linux/init.h>
#include <linux/jiffies.h>
#include <linux/kernel.h>
#include <linux/io.h>
#include <linux/slab.h>
#include <asm/uaccess.h>
#include <linux/fs.h>
#include <mach/hardware.h>
#include "CoreServices.h"
#include <linux/interrupt.h>
#include <mach/hardware.h>
#include <asm/cacheflush.h>
#include <asm/smp_plat.h>
#include <linux/irq.h>
#include <linux/interrupt.h>
#include <asm/irq.h>
#include <linux/dma-mapping.h>
#include <asm/cputype.h>
#include <linux/mm.h>  /* mmap related stuff */
#include <asm/cacheflush.h>
#include <asm/highmem.h>
#include <linux/delay.h>

#define SUCCESS 0
#define DEVICE_NAME "CoreServices"
#define BUF_LEN 80





//
//
//
CoreServicesBridge*     bridge  = 0;

/*
 * Is the device open right now? Used to prevent
 * concurent access into the same device
 */
static int Device_Open = 0;

/*
 * The message the device will give when asked
 */
static char Message[BUF_LEN];

/*
 * How far did the process reading the message get?
 * Useful if the message is larger than the size of the
 * buffer we get to fill in device_read.
 */
static char* Message_Ptr;




/*
 * This is called whenever a process attempts to open the device file
 */
static int device_open(struct inode* inode, struct file* file)
{
    printk(KERN_INFO "device_open(%p)\n", file);

    /*
     * We don't want to talk to two processes at the same time
     */
    if (Device_Open)
    {
        return -EBUSY;
    }

    Device_Open++;
    /*
     * Initialize the message
     */
    Message_Ptr = Message;
    try_module_get(THIS_MODULE);
    return SUCCESS;
}



//
//
//
static int device_release(struct inode* inode, struct file* file)
{
    /*
     * We're now ready for our next caller
     */
    Device_Open--;
    module_put(THIS_MODULE);
    return SUCCESS;
}



/*
 * This function is called whenever a process which has already opened the
 * device file attempts to read from it.
 */
static ssize_t device_read(struct file* file,   /* see include/linux/fs.h   */
                           char __user* buffer,    /* buffer to be filled with data */
                           size_t length,   /* length of the buffer     */
                           loff_t* offset)
{
    /*
     * Number of bytes actually written to the buffer
     */
    int bytes_read = 0;

    /*
     * If we're at the end of the message, return 0
     * (which signifies end of file)
     */
    if (*Message_Ptr == 0)
    {
        return 0;
    }

    /*
     * Actually put the data into the buffer
     */
    while (length && *Message_Ptr)
    {
        /*
         * Because the buffer is in the user data segment,
         * not the kernel data segment, assignment wouldn't
         * work. Instead, we have to use put_user which
         * copies data from the kernel data segment to the
         * user data segment.
         */
        put_user(*(Message_Ptr++), buffer++);
        length--;
        bytes_read++;
    }

    /*
     * Read functions are supposed to return the number
     * of bytes actually inserted into the buffer
     */
    return bytes_read;
}




/*
 * This function is called when somebody tries to
 * write into our device file.
 */
static ssize_t device_write(struct file* file, const char __user* buffer, size_t length, loff_t* offset)
{
    int i;

    printk(KERN_INFO "device_write(%p,%s,%d)", file, buffer, length);

    for (i = 0; i < length && i < BUF_LEN; i++)
    {
        get_user(Message[i], buffer + i);
    }

    Message_Ptr = Message;
    /*
     * Again, return the number of input characters used
     */
    return i;
}



//
//
//
irqreturn_t MailboxIRQHandler0(int irq, void *dev_id, struct pt_regs *regs)
{
    //
    // Clear the interrupt...
    //
    writel( 0xffffffff, __io_address(ARM_LOCAL_MAILBOX0_CLR0) + 0x00 );
    dsb();

    printk(KERN_INFO "MailboxIRQHandler 0 called.\n");

    return IRQ_HANDLED;
}


//
//
//
irqreturn_t MailboxIRQHandler1(int irq, void *dev_id, struct pt_regs *regs)
{
    //
    // Clear the interrupt...
    //
    writel( 0xffffffff, __io_address(ARM_LOCAL_MAILBOX0_CLR0) + 0x04 );
    dsb();

    printk(KERN_INFO "MailboxIRQHandler 1 called.\n");

    return IRQ_HANDLED;
}


//
//
//
irqreturn_t MailboxIRQHandler2(int irq, void *dev_id, struct pt_regs *regs)
{
    //
    // Clear the interrupt...
    //
    writel( 0xffffffff, __io_address(ARM_LOCAL_MAILBOX0_CLR0) + 0x08 );
    dsb();

    printk(KERN_INFO "MailboxIRQHandler 2 called.\n");

    return IRQ_HANDLED;
}


//
//
//
irqreturn_t MailboxIRQHandler3(int irq, void *dev_id, struct pt_regs *regs)
{
    //
    // Clear the interrupt...
    //
    writel( 0xffffffff, __io_address(ARM_LOCAL_MAILBOX0_CLR0) + 0x0c );
    dsb();

    printk(KERN_INFO "MailboxIRQHandler 3 called.\n");

    return IRQ_HANDLED;
}

//
//
//
void SendDoorBellToCore(uint32_t coreNumber)
{
    uint32_t coreID;

    //
    // Ensure that stores to Normal memory are visible to the
    // other CPUs before issuing the IPI.
    //
    dsb();

    //
    // Cause the doorbell interrupt on the remote core.
    //
    coreID  = read_cpuid_mpidr();
    writel(1<<coreID, __io_address(ARM_LOCAL_MAILBOX0_SET0 + (0x10*coreNumber) ) );
}

//
//
//
void StartCore(uint32_t coreID, uint32_t entrypoint)
{
    uint32_t    physAddress     = 0x4000008c + (coreID<<4);
    uint32_t*   entryPointValue = (uint32_t*)physAddress;

    dsb();

    printk(KERN_INFO "Setting core %d entrypoint value to %08x. phys=%08x virt=%08x\n", coreID, entrypoint, physAddress, (uint32_t)entryPointValue);
    writel( entrypoint, __io_address(physAddress) );
    printk(KERN_INFO "Done.\n");
}



/*
 * This function is called whenever a process tries to do an ioctl on our
 * device file. We get two extra parameters (additional to the inode and file
 * structures, which all device functions get): the number of the ioctl called
 * and the parameter given to the ioctl function.
 *
 * If the ioctl is write or read/write (meaning output is returned to the
 * calling process), the ioctl call returns the output of this function.
 *
 */
long device_ioctl(struct file *filp, unsigned int cmd, unsigned long arg)
{
    printk(KERN_INFO "ioctl cmd=%d, arg=%08lx", cmd, arg);

    /*
     * Switch according to the ioctl called
     */
    switch (cmd)
    {
        case IOCTL_FLUSH_AND_INVALIDATE:
        {
            struct vm_area_struct*  next_vma;
            struct mm_struct*       mm;           

            printk(KERN_DEBUG "process %i (%s) going to sleep\n",current->pid, current->comm);

            mm = current->mm;

            next_vma = find_vma(mm, ALLOY_RAM_BASE);
            
            //flush_cache_range(next_vma, ALLOY_RAM_BASE, ALLOY_RAM_BASE+(ALLOY_DEDICATED_RAM_SIZE));

            break;
        }

        case IOCTL_ALLOCATE_BLOCK:
        {
            AllocateRequest     request;
            void*               block           = NULL;
            dma_addr_t          handle          = 0;
            uint32_t            result;

            //
            //
            //
            result  = copy_from_user(&request, (void *)arg, sizeof(request));

            printk("Request to allocate %d bytes\n", request.size);

            //
            // Allocate an uncached area to be shared among cores.
            //
            block  = dma_alloc_coherent(NULL, request.size, &handle, GFP_KERNEL);
            if(block != NULL)
            {
                printk( KERN_INFO "allocation ok (%08x).\n", virt_to_phys(bridge) );
            }
            else
            {
                printk( KERN_INFO "allocation failed.\n");
            }

            request.virtualAddress     = (uint32_t)block;
            request.physicalAddress    = virt_to_phys(block);

            //
            //
            //
            result  = copy_to_user( (void *)arg, &request, sizeof(request));

            break;
        }

        case IOCTL_SET_MSG:
        {
            *((uint32_t*)0x10000000)    = 3;
            StartCore(3, arg );
            //msleep(100);
            //while( *((uint32_t*)0x10000000) != 0xffffffff);

            *((uint32_t*)0x10000000)    = 2;
            StartCore(2, arg );
            //msleep(100);
            //while( *((uint32_t*)0x10000000) != 0xffffffff);

            *((uint32_t*)0x10000000)    = 1;
            StartCore(1, arg );
            //msleep(100);
            //while( *((uint32_t*)0x10000000) != 0xffffffff);

            break;
            
        }

        case IOCTL_GET_BRIDGE_ADDRESS:
        {
            if(arg != 0)
            {
                uint32_t    physicalAddressOfBridge    = virt_to_phys( bridge );

                uint8_t*    ptr     = (uint8_t*)&physicalAddressOfBridge;
                uint32_t    i       = 0;
                
                for(i=0; i<sizeof(physicalAddressOfBridge); i++)
                {
                    put_user( ptr[i], ((uint8_t*)arg)+i );
                }
            }
            break;
        }

        case IOCTL_SEND_MAIL:
        {
            SendDoorBellToCore( arg );
            printk(KERN_INFO "Sending doorbell to core %d\n", (int)arg);
            break;
        }

        case IOCTL_GET_MSG:
        {
            uint8_t*    ptr     = (uint8_t*)bridge;
            uint32_t    i       = 0;
            
            for(i=0; i<sizeof(CoreServicesBridge); i++)
            {
                put_user( ptr[i], ((uint8_t*)arg)+i );
            }
            break;
        }

        case IOCTL_GET_NTH_BYTE:
            /*
             * This ioctl is both input (arg) and
             * output (the return value of this function)
             */
            return Message[arg];
            break;
    }

    return SUCCESS;
}









void mmap_open(struct vm_area_struct *vma)
{
    printk("mmap_open \n");
}

void mmap_close(struct vm_area_struct *vma)
{
    printk("mmap_close \n");
}

#if 0
int mmap_fault(struct vm_area_struct *vma, struct vm_fault *vmf)
{
    struct page*        page;
    //uint8_t*            baseAddress;

    //baseAddress     = ioremap_nocache( ALLOY_RAM_BASE, ALLOY_DEDICATED_RAM_SIZE );
    //page = virt_to_page(ALLOY_RAM_BASE);
    page    = ALLOY_RAM_BASE >> PAGE_SHIFT;

    //vm_insert_page(vma, vmf->virtual_address , page);

    printk("page = %08x %08x\n",(uint32_t)page, (uint32_t)vmf->pgoff );
    
    /* increment the reference count of this page */
    get_page(page);
    vmf->page = page;

    remap_pfn_range(vma, (uint32_t)vmf->virtual_address, ALLOY_RAM_BASE>>PAGE_SHIFT, ALLOY_DEDICATED_RAM_SIZE, vma->vm_page_prot);

    return 0;
}
#endif



uint8_t*    baseAddress;



//
//
//
struct vm_operations_struct mmap_vm_ops = 
{
    .open =     mmap_open,
    .close =    mmap_close,
    //.fault =    mmap_fault,
};



uint8_t     blaa[8192]  =
{
    0,1,2,3,4,5,6,7,8,9
};


#if 0
#include <soc/bcm2835/raspberrypi-firmware.h>
#include <mach/vc_support.h>

/****** VC MAILBOX FUNCTIONALITY ******/
unsigned int AllocateVcMemory(unsigned int *pHandle, unsigned int size, unsigned int alignment, unsigned int flags)
{
    struct vc_msg
    {
        unsigned int m_msgSize;
        unsigned int m_response;

        struct vc_tag
        {
            unsigned int m_tagId;
            unsigned int m_sendBufferSize;
            union {
                unsigned int m_sendDataSize;
                unsigned int m_recvDataSize;
            };

            struct args
            {
                union {
                    unsigned int m_size;
                    unsigned int m_handle;
                };
                unsigned int m_alignment;
                unsigned int m_flags;
            } m_args;
        } m_tag;

        unsigned int m_endTag;
    } msg;
    int s;

    msg.m_msgSize = sizeof(msg);
    msg.m_response = 0;
    msg.m_endTag = 0;

    //fill in the tag for the allocation command
    msg.m_tag.m_tagId = 0x3000c;
    msg.m_tag.m_sendBufferSize = 12;
    msg.m_tag.m_sendDataSize = 12;

    //fill in our args
    msg.m_tag.m_args.m_size = size;
    msg.m_tag.m_args.m_alignment = alignment;
    msg.m_tag.m_args.m_flags = flags;

    //run the command
    //s = bcm_mailbox_property(&msg, sizeof(msg));

    if (s == 0 && msg.m_response == 0x80000000 && msg.m_tag.m_recvDataSize == 0x80000004)
    {
        *pHandle = msg.m_tag.m_args.m_handle;
        return 0;
    }
    else
    {
        printk(KERN_ERR "failed to allocate vc memory: s=%d response=%08x recv data size=%08x\n",
                s, msg.m_response, msg.m_tag.m_recvDataSize);
        return 1;
    }
}



unsigned int LockVcMemory(unsigned int *pBusAddress, unsigned int handle)
{
    struct vc_msg
    {
        unsigned int m_msgSize;
        unsigned int m_response;

        struct vc_tag
        {
            unsigned int m_tagId;
            unsigned int m_sendBufferSize;
            union {
                unsigned int m_sendDataSize;
                unsigned int m_recvDataSize;
            };

            struct args
            {
                union {
                    unsigned int m_handle;
                    unsigned int m_busAddress;
                };
            } m_args;
        } m_tag;

        unsigned int m_endTag;
    } msg;
    int s;

    msg.m_msgSize = sizeof(msg);
    msg.m_response = 0;
    msg.m_endTag = 0;

    //fill in the tag for the lock command
    msg.m_tag.m_tagId = 0x3000d;
    msg.m_tag.m_sendBufferSize = 4;
    msg.m_tag.m_sendDataSize = 4;

    //pass across the handle
    msg.m_tag.m_args.m_handle = handle;

    //s = bcm_mailbox_property(&msg, sizeof(msg));
    {
        struct rpi_firmware *fw = rpi_firmware_get(NULL);
        int rr= rpi_firmware_property_list(&fw, &msg, sizeof(msg) );        
    }

    if (s == 0 && msg.m_response == 0x80000000 && msg.m_tag.m_recvDataSize == 0x80000004)
    {
        //pick out the bus address
        *pBusAddress = msg.m_tag.m_args.m_busAddress;
        return 0;
    }
    else
    {
        printk(KERN_ERR "failed to lock vc memory: s=%d response=%08x recv data size=%08x\n",
                s, msg.m_response, msg.m_tag.m_recvDataSize);
        return 1;
    }
}

#endif


#if 0
//
//
//
int device_mmap(struct file *filp, struct vm_area_struct *vma)
{
    uint32_t    address;

    printk("mmap call.\n");

    //vma->vm_ops = &mmap_vm_ops;
    vma->vm_flags |= VM_IO;

#if 0
    {
        uint32_t    handle  = 0;
        uint32_t    busAddress  = 0;
        uint32_t    r0      = AllocateVcMemory( &handle, 4096, 4096, MEM_FLAG_L1_NONALLOCATING );
        uint32_t    r1      = LockVcMemory( &busAddress, handle );
        printk("VC: %d %d %08x\n", r0, r1, busAddress);
    }
#endif

    /* assign the file private data to the vm private data */
    //vma->vm_private_data = filp->private_data;
    //mmap_open(vma);
    baseAddress     = ioremap_wc( ALLOY_RAM_BASE, ALLOY_DEDICATED_RAM_SIZE );
    //address         = virt_to_phys( (uint32_t)baseAddress );
    address         = ALLOY_RAM_BASE;
    printk("address = %08x\n",address);
    vma->vm_page_prot   = 0;        // uncached.
    remap_pfn_range(vma, vma->vm_start, address>>PAGE_SHIFT, 4096, vma->vm_page_prot);

    {
        uint32_t    i = 0;

        memset_io(baseAddress, 0x76, 4096);

        for(i=0; i<16; i++)
        {
            baseAddress[i]  = i*2;
            writeb( i*2, baseAddress+i );
            wmb();
        }
    }

    dsb(sy);
    wmb();
    //__cpuc_flush_dcache_area( baseAddress, 4096 );

    baseAddress[128]    = 0xff;
    baseAddress[256]    = 0xff;

    //baseAddress = phys_to_virt(ALLOY_RAM_BASE);
    //memset((uint8_t*)baseAddress, 0xba, 4096 );

    return 0;
}

#endif
/*
 * This structure will hold the functions to be called
 * when a process does something to the device we
 * created. Since a pointer to this structure is kept in
 * the devices table, it can't be local to
 * init_module. NULL is for unimplemented functions.
 */
struct file_operations Fops =
{
    .read           = device_read,
    .write          = device_write,
    .unlocked_ioctl = device_ioctl,
    .open           = device_open,
    .release        = device_release,  /* a.k.a. close */
    //.mmap           = device_mmap,
};


//
//
//
static int __init CoreServicesInit(void)
{
    int         ret_val         = 0;
    int         result          = 0;
    uint32_t    currentSettings = 0;


    struct resource* r = request_mem_region(ALLOY_RAM_BASE, ALLOY_DEDICATED_RAM_SIZE, "AlloyRAM");
    if(r == NULL)
    {
        printk("request_mem_region failed.\n");
    }
    else
    {
        printk("request_mem_region ok.\n");
    }

    r = request_mem_region(0x40000000, 4096, "AlloyPeripherals");
    if(r == NULL)
    {
        printk("request_mem_region failed.\n");
    }
    else
    {
        printk("request_mem_region ok.\n");
    }

    //
    // Allocate an uncached area to be shared among cores.
    //
    //bridge  = dma_alloc_coherent(NULL, ALLOY_DEDICATED_RAM_SIZE, &handle, GFP_KERNEL);
    bridge  = kmalloc(1024*1024*1, GFP_USER );
    if(bridge != NULL)
    {
        printk( KERN_INFO "Bridge allocation ok (%08x).\n", virt_to_phys(bridge) );
    /*
        result  = cacheflush(bridge, ALLOY_DEDICATED_RAM_SIZE, DCACHE);
        if(r == 0)
        {
            printk("cacheflush ok.\n");
        }
        else
        {
            printk("cacheflush failed.\n");
        }
    */
        flush_cache_all();


#if 0
        //
        //
        //
        //uint8_t*    baseAddress     = phys_to_virt( (uint8_t*)ALLOY_RAM_BASE );
        baseAddress     = ioremap_nocache( ALLOY_RAM_BASE, ALLOY_DEDICATED_RAM_SIZE );
        printk("baseAddress = %08x\n", (uint32_t)baseAddress);
        memset(baseAddress, 0xa5, ALLOY_DEDICATED_RAM_SIZE);
        printk("All cleared...\n");
#endif

        //flush_cache_range(page_anon_vma(), ALLOY_RAM_BASE, ALLOY_RAM_BASE+(ALLOY_DEDICATED_RAM_SIZE));

        /*
         * Register the character device (atleast try)
         */
        ret_val = register_chrdev(MAJOR_NUM, DEVICE_NAME, &Fops);

        /*
         * Negative values signify an error
         */
        if (ret_val < 0)
        {
            printk(KERN_ALERT "%s failed with %d\n",
                   "Sorry, registering the character device ", ret_val);
            return ret_val;
        }

        set_irq_flags(IRQ_ARM_LOCAL_MAILBOX2, IRQF_VALID);
        irq_clear_status_flags(IRQ_ARM_LOCAL_MAILBOX2, IRQ_PER_CPU);
        irq_clear_status_flags(IRQ_ARM_LOCAL_MAILBOX2, IRQ_LEVEL);


        irq_modify_status(IRQ_ARM_LOCAL_MAILBOX0,0xffffffff,0x00000000);
        irq_modify_status(IRQ_ARM_LOCAL_MAILBOX1,0xffffffff,0x00000000);
        irq_modify_status(IRQ_ARM_LOCAL_MAILBOX2,0xffffffff,0x00000000);
        irq_modify_status(IRQ_ARM_LOCAL_MAILBOX3,0xffffffff,0x00000000);

    /*
        {
            int i;
            struct irq_desc *desc;

            for_each_irq_desc(i, desc) {
             if (!desc)
               continue;
             printk(KERN_INFO "%d: status_use_accessors=%08x, name=%08x, handle_irq=%08x\n", i, (u32) desc->status_use_accessors, (u32) desc->name, (u32) desc->handle_irq );
             }
         }
    */
        //
        // Register the interrupt handler for mailbox IRQs.
        // 
        set_irq_flags(IRQ_ARM_LOCAL_MAILBOX2, IRQF_VALID);
        result = request_threaded_irq(   IRQ_ARM_LOCAL_MAILBOX0,                 // The interrupt number requested 
                                (irq_handler_t) MailboxIRQHandler0,      // The pointer to the handler function (above)
                                NULL,
                                IRQF_SHARED,           // Interrupt is on rising edge (button press in Fig.1)
                                "MailboxIRQHandler",                    // Used in /proc/interrupts to identify the owner
                                DEVICE_NAME);                                  // The *dev_id for shared interrupt lines, NULL here

        result = request_threaded_irq(   IRQ_ARM_LOCAL_MAILBOX1,                 // The interrupt number requested 
                                (irq_handler_t) MailboxIRQHandler1,      // The pointer to the handler function (above)
                                NULL,
                                IRQF_SHARED,           // Interrupt is on rising edge (button press in Fig.1)
                                "MailboxIRQHandler",                    // Used in /proc/interrupts to identify the owner
                                DEVICE_NAME);                                  // The *dev_id for shared interrupt lines, NULL here

        result = request_threaded_irq(   IRQ_ARM_LOCAL_MAILBOX2,                 // The interrupt number requested 
                                (irq_handler_t) MailboxIRQHandler2,      // The pointer to the handler function (above)
                                NULL,
                                IRQF_SHARED,           // Interrupt is on rising edge (button press in Fig.1)
                                "MailboxIRQHandler",                    // Used in /proc/interrupts to identify the owner
                                DEVICE_NAME);                                  // The *dev_id for shared interrupt lines, NULL here

        result = request_threaded_irq(   IRQ_ARM_LOCAL_MAILBOX3,                 // The interrupt number requested 
                                (irq_handler_t) MailboxIRQHandler3,      // The pointer to the handler function (above)
                                NULL,
                                IRQF_SHARED,           // Interrupt is on rising edge (button press in Fig.1)
                                "MailboxIRQHandler",                    // Used in /proc/interrupts to identify the owner
                                DEVICE_NAME);                                  // The *dev_id for shared interrupt lines, NULL here
        if(result == 0)
        {
            printk(KERN_INFO "Mailbox ISR registered ok.\n");
        }
        else
        {
            printk(KERN_INFO "Mailbox ISR registration failed (%d).\n", result);        
        }

        //
        // Enable the interupt.
        // We're on Core0 and we want to enable the Mailbox 1 interrupt.
        //
        currentSettings     = readl( __io_address(ARM_LOCAL_MAILBOX_INT_CONTROL0) );
        currentSettings |= 0x0000000f;
        writel( currentSettings, __io_address(ARM_LOCAL_MAILBOX_INT_CONTROL0) );



        printk(KERN_INFO "%s The major device number is %d.\n", "Registeration is a success", MAJOR_NUM);
        printk(KERN_INFO "If you want to talk to the device driver,\n");
        printk(KERN_INFO "you'll have to create a device file. \n");
        printk(KERN_INFO "We suggest you use:\n");
        printk(KERN_INFO "mknod %s c %d 0\n", DEVICE_FILE_NAME, MAJOR_NUM);
        printk(KERN_INFO "The device file name is important, because\n");
        printk(KERN_INFO "the ioctl program assumes that's the\n");
        printk(KERN_INFO "file you'll use.\n");
    }
    else
    {
        printk( KERN_INFO "Bridge allocation failed.\n");
    }

    return 0;
}



//
//
//
static void __exit CoreServicesExit(void)
{
    if(bridge != NULL)
    {
        kfree( bridge );
        bridge  = NULL;        
    }

    /*
     * Unregister the device
     */
    unregister_chrdev(MAJOR_NUM, DEVICE_NAME);
}




module_init(CoreServicesInit);
module_exit(CoreServicesExit);

MODULE_AUTHOR("Steve Tickle");
MODULE_DESCRIPTION("Reactor Core Services module.");
MODULE_LICENSE("GPL");
