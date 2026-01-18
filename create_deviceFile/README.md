root@beaglebone-yocto:~# insmod deviceFile.ko 

root@beaglebone-yocto:~# dmesg | tail

[   10.803557] cpsw-switch 4a100000.switch eth0: Link is Up - 100Mbps/Full - flow control off

[   14.224147] 8021q: 802.1Q VLAN Support v1.8

[   14.243755] 8021q: adding VLAN 0 to HW filter on device eth0

[   78.682462] dynamic: loading out-of-tree module taints kernel.

[   78.687661] Allocated major number is 245 and the minor number is 0

[   78.692676] kernel module added successfully

[  289.765025] module has been removed successfully

[  309.192572] allocated major no is 245 and minor number is 0

[  309.199095] device creation successfull

[  309.201670] KERNAL MODULE INSERTED SUCCESSFULLY 

root@beaglebone-yocto:~# ls /dev/ | grep new_device

new_device

root@beaglebone-yocto:~# ls -l /dev/ | grep new_device

crw------- 1 root root    245,   0 Mar  9 23:01 new_device
