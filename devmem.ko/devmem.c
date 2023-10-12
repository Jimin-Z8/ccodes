#include <linux/module.h>
#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/version.h>
#include <linux/proc_fs.h>
#include <linux/seq_file.h>
#include <linux/delay.h>
#include <linux/uaccess.h>
#include <asm/io.h>     // ioremap()
#include <linux/ioport.h>   // request_mem_region

unsigned int *p=NULL;
static __init int my_module_init(void)
{
    int i;
    // struct resource *request_mem_region(unsigned long start, unsigned long len, char *name);
    if (request_mem_region(0x200000af0000, 0x10000, "XXX"))
    {
        printk("mmio already reserved\n");
        return -EBUSY;
    }
    //p = ioremap(0x200000000000, 0x1000000);
    p = ioremap(0x200000af0000, 0x10000);
    // 0x200000000000 is pcie device P2SB 64-bit BAR value
    // 0xaf0000 is Lewisburg pch gpio community 0. PCH datasheet table 36-10
    printk("ioremap 0x200000af0000\n");
    printk("%p\n", p);
    for (i=0; i<16; i++)
    {
        printk("%03x: %08x\n", i*4, p[i]);
    }
    for (i=0x100; i<0x100+16; i++)
    {
        printk("%03x: %08x\n", i*4, p[i]);
    }
    return 0;
}
module_init(my_module_init);

static __exit void my_module_exit(void)
{
    iounmap(p);
    release_mem_region(0x200000af0000, 0x10000);
    printk("test.ko exit\n");
}
module_exit(my_module_exit);


MODULE_AUTHOR("Jimin Li <Jimin.Li@intel.com>");
MODULE_DESCRIPTION("my module exmaple");
MODULE_LICENSE("GPL v2");
