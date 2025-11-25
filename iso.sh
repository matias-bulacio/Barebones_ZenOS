#!/bin/sh
set -e
. ./build.sh

mkdir -p iso.d
mkdir -p iso.d/boot
mkdir -p iso.d/boot/grub

cp sysroot/boot/zenos.kernel iso.d/boot/zenos.kernel
cat > iso.d/boot/grub/grub.cfg << EOF
menuentry "ZenOS" {
	multiboot /boot/zenos.kernel
}
EOF
grub-mkrescue -o ZenOS.iso iso.d
