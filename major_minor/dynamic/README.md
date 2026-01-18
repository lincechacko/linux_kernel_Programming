[ 4139.289009] Allocated major number is 245 and the minor number is 0
[ 4139.294840] kernel module added successfully

root@beaglebone-yocto:~# cat /proc/devices
Character devices:
  1 mem
  4 /dev/vc/0
  4 tty
  4 ttyS
  5 /dev/tty
  5 /dev/console
  5 /dev/ptmx
  7 vcs
 10 misc
 13 input
 29 fb
 89 i2c
 90 mtd
116 alsa
128 ptm
136 pts
180 usb
189 usb_device
226 drm
245 new_device # This is the major number allocated

