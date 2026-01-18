#include <linux/module.h>
#include <linux/kernel.h>

int init_module(void) {
    printk(KERN_INFO "Hello BeagleBone! Module loaded.\n");
    return 0;
}

void cleanup_module(void) {
    printk(KERN_INFO "Goodbye BeagleBone! Module removed.\n");
}

MODULE_LICENSE("GPL");
