set -e

nasm -felf32 boot.s -o boot.o
i686-elf-gcc -c kernel.c -o kernel.o -std=gnu99 -ffreestanding -O2 -Wall -Wextra
i686-elf-gcc -T linker.ld -o ZenOS.bin -ffreestanding -O2 -nostdlib boot.o kernel.o -lgcc

if ! grub-file --is-x86-multiboot ZenOS.bin; then
  echo the file is not multiboot
  exit 1
fi

cp ZenOS.bin iso.d/boot/ZenOS.bin
grub-mkrescue -o ZenOS.iso iso.d

if [ $1 -a $1 = "run" ]; then
	qemu-system-i386 -cdrom ZenOS.iso
fi

