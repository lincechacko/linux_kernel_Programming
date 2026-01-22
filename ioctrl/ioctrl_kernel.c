#include<linux/kernel.h>
#include<linux/init.h>
#include<linux/module.h>
#include<linux/fs.h>
#include<linux/err.h>
#include<linux/kdev_t.h>
#include<linux/cdev.h>
#include<linux/device.h>
#include<linux/slab.h>
#include<linux/uaccess.h>
#include<linux/ioctl.h>


/*defining the IOCTL Control command*/
#define WR_VALUE _IOW('a','a',int32_t *)
#define RD_VALUE _IOR('a','b',int32_t *)

int32_t value = 0;

dev_t device;
static struct class * device_class;
static struct cdev device_cdev;
static struct file_operartions fops;

/*
** Function Prototypes
*/
static int      __init etx_driver_init(void);
static void     __exit etx_driver_exit(void);
static int      device_open(struct inode *inode, struct file *file);
static int      device_release(struct inode *inode, struct file *file);
static ssize_t  device_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  device_write(struct file *filp, const char *buf, size_t len, loff_t * off);
static long     device_ioctl(struct file *file, unsigned int cmd, unsigned long arg);
/*
** File operation sturcture
*/
static struct file_operations fops =
{
        .owner          = THIS_MODULE,
        .read           = device_read,
        .write          = device_write,
        .open           = device_open,
        .unlocked_ioctl = device_ioctl,
        .release        = device_release,
};
/*
** This function will be called when we open the Device file
*/
static int device_open(struct inode *inode, struct file *file)
{
        pr_info("Device File Opened...!!!\n");
        return 0;
}
/*
** This function will be called when we close the Device file
*/
static int device_release(struct inode *inode, struct file *file)
{
        pr_info("Device File Closed...!!!\n");
        return 0;
}
/*
** This function will be called when we read the Device file
*/
static ssize_t device_read(struct file *filp, char __user *buf, size_t len, loff_t *off)
{
        pr_info("Read Function\n");
        return 0;
}
/*
** This function will be called when we write the Device file
*/
static ssize_t device_write(struct file *filp, const char __user *buf, size_t len, loff_t *off)
{
        pr_info("Write function\n");
        return len;
}


static long device_ioctl(struct file * file , unsigned int cmd , unsigned long arg)
{
	         switch(cmd) {
                case WR_VALUE:
                        if( copy_from_user(&value ,(int32_t*) arg, sizeof(value)) )
                        {
                                pr_err("Data Write : Err!\n");
                        }
                        pr_info("Value = %d\n", value);
                        break;
                case RD_VALUE:
                        if( copy_to_user((int32_t*) arg, &value, sizeof(value)) )
                        {
                                pr_err("Data Read : Err!\n");
                        }
                        break;
                default:
                        pr_info("Default\n");
                        break;
        }
        return 0;
}

static int __inti init_function(void)
{
	if(alloc_chrdev_region(&device,0,1,"chr_dev")<0)
	{
		pr_err("allocation of major and minor number has been failed\n");
		return(-1);
	}
	pr_info("allocation of major and minor number is success\n");

	/*after the successfull allocation of the major and the minor number lets init the cdev structure*/

	cdev_init(&device_cdev , &fops);

	/*adding the character device to the system*/
	if(cdev_add(&device_cdev , device , 1) < 0)
	{
		unregister_chrdev_region(device , 1);
		pr_err("cannot add the device to the system\n");
		return(-1);
	}

	/*creating the struct class*/
	if(IS_ERR(device_class = class_create("device_class")))
	{
		unregister_chrdev_region(device , 1);
		pr_err("class creation has been failed\n");
		return(-1);
	}

	/*creating the device*/

	if(IS_ERR(device_create(device_class , NULL,device ,  NULL , "new_device"))) 
	{
		class_destroy(device_class);
		unregister_chrdev_region(device , 1);
		pr_err("device creation failed\n");
		return(-1);
	}
	pr_info("kernal module added successfully\n");
	return (0);
}

static void __exit exit_function(void)
{
}


module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("SIMPLE DRIVER TO MAKE THE USE OF IOCTL");

