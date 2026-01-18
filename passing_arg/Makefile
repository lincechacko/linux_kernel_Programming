obj-m := param_passing.o

# KDIR points to the kernel source in your SDK
KDIR := /home/lince/yocto_project/beagleBone/build/tmp/work/beaglebone_yocto-poky-linux-gnueabi/linux-yocto/6.6.21+git/linux-beaglebone_yocto-standard-build

ARCH := arm
CROSS_COMPILE := arm-poky-linux-gnueabi-

all:
	$(MAKE) -C $(KDIR) M=$(PWD) ARCH=$(ARCH) CROSS_COMPILE=$(CROSS_COMPILE) modules

clean:
	$(MAKE) -C $(KDIR) M=$(PWD) clean
