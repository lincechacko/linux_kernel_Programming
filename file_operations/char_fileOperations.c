#include<linux/kernel.h>
#include<linux/module.h>
#include<linux/init.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/kdev_t.h>

dev_t device = 0;
static struct class  * device_class;
static struct cdev device_cdev;


/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      etx_open(struct inode *inode, struct file *file);
static int      etx_release(struct inode *inode, struct file *file);
static ssize_t  etx_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  etx_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = etx_read,
    .write      = etx_write,
    .open       = etx_open,
    .release    = etx_release,
};
/*
** This function will be called when we open the Device file
*/
static int etx_open(struct inode *inode, struct file *file)
{
        pr_info("Driver Open Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/
static int etx_release(struct inode *inode, struct file *file)
{
        pr_info("Driver Release Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t etx_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Read Function Called...!!!\n");
        return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t etx_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Driver Write Function Called...!!!\n");
        return len;
}

static int __init init_function(void)
{
	if(alloc_chrdev_region(&device,0,1,"new_deice")<0)
	{
		pr_err("cannot allocate major and minor number\n");
		return -1;
	}
	/*now the major number and minor number has been allocated*/
	pr_info("major and minor number allocation success %d %d\n",MAJOR(device),MINOR(device));
	
	/*we can create the cdev structure*/
	cdev_init(&device_cdev,&fops);

	/*since we created the cdev struct we can add the character device to the system*/
	if(cdev_add(&device_cdev,device,1)<0)
	{
		unregister_chrdev_region(device,1);
		pr_err("adding the device to the system failed\n");
		return(-1);
	}

	/*creating the struct class*/
	if(IS_ERR(device_class = class_create("new_class")))
	{
		unregister_chrdev_region(device,1);
		pr_err("cannot create the struct class\n");
		return(-1);
	}
	pr_info("class creation is success\n");

	/*creating the device*/
	if(IS_ERR(device_create(device_class,NULL,device,NULL,"newDevice")))
	{
		class_destroy(device_class);
		unregister_chrdev_region(device,1);
		pr_err("device creation failed\n");
		return(-1);
	}
	pr_info("device creation is success\n");
	pr_info("kernal module insert successfully\n");
	return (0);
}

static void __exit exit_function(void)
{
	device_destroy(device_class,device);
	class_destroy(device_class);
	cdev_del(&device_cdev);
	unregister_chrdev_region(device,1);
	pr_info("module has been removed successfully\n");
	
}


module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("A TEST PROJECT FOR READ WRITE OF A  CHAR FILE SYSTEM");
