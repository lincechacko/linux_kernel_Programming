#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/kdev_t.h>
#include<linux/err.h>
#include<linux/device.h>
#include<linux/cdev.h>
#include<linux/uaccess.h>
#include<linux/slab.h>
#include<linux/proc_fs.h>
#include<linux/ioctl.h>


dev_t device = 0;
static struct cdev device_cdev;
static struct class * device_class;

static int __init init_function(void)
{
	if(alloc_chrdev_region(&device , 1 , "new_Device") < 0)
	{
		//error
	}

	cdev_init(&device_cdev , &fops);

	cdev_add(&device_cdev , device , 1);


	device_class = class_create("new_class");

	device_create(device_class , NULL , device , NULL , "new_device");
}


static void __exit exit_function(void)
{
}


module_init(init_function);
module_exit(exit_function);

MOUDLE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("PROCFS EXAMPLE");
