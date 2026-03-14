#include <linux/module.h>
#include <linux/init.h>
#include <linux/slab.h>
#include <linux/i2c.h>
#include <linux/delay.h>
#include <linux/kernel.h>
#include <linux/kdev_t.h>
#include <linux/fs.h>
#include <linux/err.h>
#include <linux/cdev.h>
#include <linux/device.h>

#define I2C_BUS_AVAILABLE            2                    /*i2c bus number*/
#define I2C_DEVICE_NAME              "BMP180"             /*device name*/
#define I2C_DEVICE_ADDRESS           0X77                 /*address of the device*/

static int      bmp180_open(struct inode *inode, struct file *file);
static int      bmp180_release(struct inode *inode, struct file *file);
static ssize_t  bmp180_read(struct file *filp, char __user *buf, size_t len,loff_t * off);
static ssize_t  bmp180_write(struct file *filp, const char *buf, size_t len, loff_t * off);

dev_t bmp180_dev = 0;
static struct class *bmp180_class;
static struct cdev bmp180_cdev;
static struct i2c_adapter *bmp180_i2c_adapter     = NULL;    /* I2C Adapter Structure*/
static struct i2c_client  *i2c_client_bmp180 = NULL;  /* I2C Cient Structure (In our case it is BMP180)*/
static struct file_operations fops =
{
    .owner      = THIS_MODULE,
    .read       = bmp180_read,
    .write      = bmp180_write,
    .open       = bmp180_open,
    .release    = bmp180_release,
};

static int bmp180_open(struct inode *inode, struct file *file)
{
	return 0;
}

static int  bmp180_release(struct inode *inode, struct file *file)
{
	return 0;
}

static ssize_t bmp180_read(struct file *filp ,  char __user *read_buffer, size_t len , loff_t *off)
{
	i2c_master_recv(i2c_client_bmp180 , read_buffer , len);
	return 0;
}

static ssize_t bmp180_write(struct file *filp , const char __user *write_buffer , size_t len , loff_t *off )
{
	i2c_master_send(i2c_client_bmp180 , write_buffer, len);
	return 0;
}

/*
 * create the structure i2c_device_id and initialize the necessary members
 * */
static const struct i2c_device_id bmp180_id [] = 
{
	{I2C_DEVICE_NAME , 0},
	{}
};

MODULE_DEVICE_TABLE(i2c , bmp180_id);

/*
* This function getting called when the slave has been found
* Note : This will be called only once when we load the driver.
*/
static int bmp180_probe(struct i2c_client *client)
{
    uint8_t data = 0;
    data = i2c_smbus_read_byte_data(i2c_client_bmp180, 0xD0);
    if(data == 0x55)
    {
    	pr_info("bmp180 Probed!!!\n");
    }
    return 0;
}

/*
* This function getting called when the slave has been removed
* Note : This will be called only once when we unload the driver.
*/
static void bmp180_remove(struct i2c_client *client)
{
   pr_info("bmp180 Removed!!!\n");
}

/*
 *i2c driver structure that has to be added to the linux
 */
static struct i2c_driver bmp180_driver = {
        .driver = {
            .name   = I2C_DEVICE_NAME,
            .owner  = THIS_MODULE,
        },
        .probe          = bmp180_probe,
        .remove         = bmp180_remove,
        .id_table       = bmp180_id,
};


/*
 * @brief : To initialize the board info
 * */
static struct i2c_board_info bmp180_i2c_board_info = 
{
	I2C_BOARD_INFO(I2C_DEVICE_NAME , I2C_DEVICE_ADDRESS)
};

/*
 * @brief init function to load the driver
 * */
static int i2c_driver_init(void)
{
	int return_status = -1;
	/*API to get the adapter structure of this I2C bus.*/
	bmp180_i2c_adapter = i2c_get_adapter(I2C_BUS_AVAILABLE);

	if(bmp180_i2c_adapter != NULL)
	{
		/*API to get the adapter structure of the i2c bus*/
		i2c_client_bmp180 = i2c_new_client_device(bmp180_i2c_adapter , &bmp180_i2c_board_info);
		
		if(i2c_client_bmp180 != NULL)
		{
			i2c_add_driver(&bmp180_driver);
			return_status = 0;
		}
		
		i2c_put_adapter(bmp180_i2c_adapter);
	}
        /*Allocating Major number*/
        if((alloc_chrdev_region(&bmp180_dev, 0, 1, "etx_Dev")) <0){
                pr_err("Cannot allocate major number\n");
                return -1;
        }
        pr_info("Major = %d Minor = %d \n",MAJOR(bmp180_dev), MINOR(bmp180_dev));

        /*Creating cdev structure*/
        cdev_init(&bmp180_cdev,&fops);
        
	/*Adding character device to the system*/
        if((cdev_add(&bmp180_cdev,bmp180_dev,1)) < 0){
            pr_err("Cannot add the device to the system\n");
            return -1;
        }
        /*Creating struct class*/
        if(IS_ERR(bmp180_class = class_create("bmp180_class"))){
            pr_err("Cannot create the struct class\n");
            goto r_class;
        }
        /*Creating device*/
        if(IS_ERR(device_create(bmp180_class,NULL,bmp180_dev,NULL,"bmp180_device"))){
            pr_err("Cannot create the Device 1\n");
            goto r_device;
        }

	pr_info("driver added successfully\n");
	return return_status;
r_device:
        class_destroy(bmp180_class);
r_class:
        unregister_chrdev_region(bmp180_dev,1);
        return -1;
}

static void i2c_driver_exit(void)
{
	device_destroy(bmp180_class,bmp180_dev);
        class_destroy(bmp180_class);
        cdev_del(&bmp180_cdev);
        unregister_chrdev_region(bmp180_dev, 1);
	i2c_unregister_device(i2c_client_bmp180);
	i2c_del_driver(&bmp180_driver);
	pr_info("driver removed\n");
}

module_init(i2c_driver_init);
module_exit(i2c_driver_exit);


MODULE_LICENSE("GPL");
MODULE_AUTHOR("LINCE");
MODULE_DESCRIPTION("DRIVER FOR THE BMP180");
