#!/bin/bash

rm ext2_hda.img
rm ext2_hdb.img
rm ext2_hdc.img
rm ext2_hdd.img

dd if=/dev/zero of=ext2_hda.img bs=1k count=100000 > /dev/zero
mkfs -t ext2 -i 1024 -b 1024 -F ext2_hda.img > /dev/zero
fdisk ext2_hda.img <<EOF 
x
c
10
h
16
s
63
r
n
p
1
2048
199999
a
w
EOF

cp ext2_hda.img ext2_hdb.img
cp ext2_hda.img ext2_hdc.img
cp ext2_hda.img ext2_hdd.img
printf "ext2-formatted disk, hda, hdb, hdc, and hdd created\n"

./mount_disk.sh
sudo cp -r files/* /mnt
./umount_disk.sh