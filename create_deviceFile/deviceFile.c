#include <linux/kernel.h>
#include <linux/module.h>
#include <linux/init.h>
#include <linux/fs.h>
#include <linux/device.h>
#include <linux/kdev_t.h>
#include <linux/err.h>


dev_t device = 0; /*used for getting the major and the minor number*/
static struct class * device_class; 

static int __init init_function(void)
{
	if(alloc_chrdev_region(&device,0,1,"new_device")<0)
	{
		/*dynamic allocation of the  major and minor number failed*/
		pr_err("cannot allocate major and minor number \n");
		return -1;
	}
	/*now we got our major and minor number*/
	pr_info("allocated major no is %d and minor number is %d\n",MAJOR(device),MINOR(device));

	/*now we can create the struct class which will be used for creating the device*/
	device_class = class_create("new_class");

	/*if the struct class creation is failed then remove the allocated major and minor number*/
	if(IS_ERR(device_class))
	{
		pr_err("class creation failed\n");
		unregister_chrdev_region(device,1);
		return(-1);
	}

	/*if the struct class creation is success then create the device*/
	if(IS_ERR(device_create(device_class,NULL,device,NULL,"new_device")))
	{
		/*if the device creation failed then removes the class created previously and the major and minor number*/
		class_destroy(device_class);
		unregister_chrdev_region(device,1);
		pr_err("device creation failed\n");
		return -1;
	}
	pr_info("device creation successfull\n");
	pr_info("KERNAL MODULE INSERTED SUCCESSFULLY \n");
	return (0);

}

/*function to remove the kernal module*/
static void __exit exit_function(void)
{
	device_destroy(device_class,device);
	class_destroy(device_class);
	unregister_chrdev_region(device,1);
	pr_info("Kernal module removed successfully\n");

}

module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("KERNAL MODULE FOR CREATING DEVICE AFTER CREATING THE MAJOR AND MINOR NUMBER");
