#include <linux/kernel.h>
#include <linux/init.h>
#include <linux/module.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/cdev.h>
#include <linux/device.h>
#include <linux/slab.h>                 //kmalloc()
#include <linux/uaccess.h>              //copy_to/from_user()
#include <linux/kthread.h>
#include <linux/wait.h>                 // Required for the wait queues
#include <linux/err.h>


uint32_t read_count = 0;
static struct task_struct *wait_thread;

dev_t device = 0;
static struct class *device_class;
static struct cdev device_cdev;
wait_queue_head_t wait_queue_new;
int wait_queue_flag = 0;

static int device_open(struct inode *newNode , struct file * newFile);
static int device_close(struct inode *newNode , struct file *newFile);
static ssize_t device_read(struct file * newFile , char __user *buffer , size_t length , loff_t *offt);
static ssize_t device_write(struct file *newFile , const char __user *buffer , size_t length , loff_t *offt);


static struct file_operations fops=
{
	.owner = THIS_MODULE,
	.open = device_open,
	.release = device_close,
	.read = device_read,
	.write = device_write
};


/*
** Thread function
*/
static int wait_function(void *unused)
{

        while(1) {
                pr_info("Waiting For Event...\n");
                wait_event_interruptible(wait_queue_new, wait_queue_flag != 0 );
                if(wait_queue_flag == 2) {
                        pr_info("Event Came From Exit Function\n");
                        return 0;
                }
                pr_info("Event Came From Read Function - %d\n", ++read_count);
                wait_queue_flag = 0;
        }
        return 0;
}




static int device_open(struct inode *newNode , struct file * newFile)
{
	pr_info("device open called\n");
	return 0;
}

static int device_close(struct inode *newNode , struct file *newFile)
{
	pr_info("device close called\n");
	return 0;
}

static ssize_t device_read(struct file * newFile , char __user *buffer , size_t length , loff_t * offt)
{
	pr_info("Read Function\n");
        wait_queue_flag = 1;
        wake_up_interruptible(&wait_queue_new);
        return 0;
}

static ssize_t device_write(struct file *newFile , const char __user *buffer , size_t length , loff_t *offt)
{
	pr_info("device write called\n");
	return length;
}

static int __init init_function(void)
{
	if(alloc_chrdev_region(&device ,0, 1 , "chr_device")<0)
	{
		pr_err("error in assigning major and minor number\n");
		return -1;
	}
	pr_info("major and minor number creation success\n");

	/*initializing the character device with the file operation structure*/
	cdev_init(&device_cdev ,  &fops);
	pr_info("cdev init success\n");

	if(cdev_add(&device_cdev , device , 1)<0)
	{
		pr_err("adding the device to the system failed\n");
		unregister_chrdev_region(device , 1);
		return -1;
	}
	pr_info("adding the character driver to the system is success\n");

	if(IS_ERR(device_class = class_create("New device class")))
	{
		pr_err("struct class creation failed\n");
		unregister_chrdev_region(device , 1);
		return -1;
	}
	pr_info("struct class creation success\n");

	if(IS_ERR(device_create(device_class , NULL , device , NULL ,"new_device")))
	{
		pr_err("creating the device has been failed\n");
		class_destroy(device_class);
		unregister_chrdev_region(device , 1);
		return -1;
	}

	        //Create the kernel thread with name 'mythread'
        wait_thread = kthread_create(wait_function, NULL, "WaitThread");
        if (wait_thread) {
                pr_info("Thread Created successfully\n");
                wake_up_process(wait_thread);
        } else
                pr_info("Thread creation failed\n");

	 pr_info("module has been added succssfully\n");

	return 0;
}

static void __exit exit_function(void)
{
	wait_queue_flag = 2;
        wake_up_interruptible(&wait_queue_new);
	device_destroy(device_class , device);
	class_destroy(device_class);
	cdev_del(&device_cdev);
	unregister_chrdev_region(device , 1);
}


module_init(init_function);
module_exit(exit_function);

MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("SAMPLE MODULE FOR THE WAITQUEUE");
