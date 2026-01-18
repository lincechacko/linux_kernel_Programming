#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/fs.h>

dev_t device = 0;

/*init function*/
static int __init init_function(void)
{
	if(alloc_chrdev_region(&device,0,1,"new_device")<0)
	{
		printk(KERN_INFO "cannot allocate major and minor number\n");
	}
	printk(KERN_INFO "Allocated major number is %d and the minor number is %d\n",MAJOR(device),MINOR(device));
	printk(KERN_INFO "kernel module added successfully\n");
	return 0;
}


static void __exit exit_function(void)
{
	unregister_chrdev_region(device,1);
	printk(KERN_INFO "module has been removed successfully\n");
}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("DYNAMIC ALLOCATION OF THE MAJOR AND MINOR NUMBER");
