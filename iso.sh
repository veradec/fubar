#!/bin/sh
set -e
. ./build.sh

mkdir -p isodir
mkdir -p isodir/boot
mkdir -p isodir/boot/grub

cp sysroot/boot/fubar.kernel isodir/boot/fubar.kernel
cat > isodir/boot/grub/grub.cfg << EOF
menuentry "fubar" {
	multiboot /boot/fubar.kernel
}
EOF
grub-mkrescue -o fubar.iso isodir
