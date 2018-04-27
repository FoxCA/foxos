#!/bin/bash

dd if=/dev/zero of="$1" bs=512 count=131072

fdisk "$1" <<EOF 
n
p
1
2048
131071
a
w
EOF

echo "$1"

sudo losetup -D
dev0=$(losetup --find)
sudo losetup "$dev0" "$1"
dev1=$(losetup --find)
sudo losetup "$dev1" "$1" -o 1048576
echo "$dev0"
echo "$dev1"
sudo mke2fs "$dev1"
sudo mkdosfs -F32 -f 2 "$dev1"

sudo mount "$dev1" /mnt

sudo grub-install --root-directory=/mnt --no-floppy --modules="normal part_msdos ext2 multiboot" "$dev0"

