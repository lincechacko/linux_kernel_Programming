root@beaglebone-yocto:~# insmod char_fileOperations.ko 

root@beaglebone-yocto:~# echo 1 > /dev/newDevice 

root@beaglebone-yocto:~# cat /dev/newDevice 

root@beaglebone-yocto:~# dmesg | tail

[  480.910112] major and minor number allocation success 245 0

[  480.914804] class creation is success

[  480.923118] device creation is success

[  480.925613] kernal module insert successfully

[  491.192364] Driver Open Function Called...!!!

[  491.196238] Driver Write Function Called...!!!

[  491.199663] Driver Release Function Called...!!!

[  498.219014] Driver Open Function Called...!!!

[  498.222254] Driver Read Function Called...!!!

[  498.225577] Driver Release Function Called...!!!

