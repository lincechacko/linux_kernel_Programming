#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>

dev_t device = MKDEV(235,0);

static int __init init_function(void)
{
	register_chrdev_region(device,1,"new_device");
	printk(KERN_INFO "Major Number is %d  Minor Number is %d\n",MAJOR(device),MINOR(device));
	printk(KERN_INFO "KERNAL MODULE INSERT SUCCESSFULLY\n");
	return 0;
}

static  void __exit exit_function(void)
{
	unregister_chrdev_region(device,1);
	printk(KERN_INFO "Kernal device removed successfully\n");
}


module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("STATIC ASSIGNMENT OF MAJOR AND MINOR NUMBER");
