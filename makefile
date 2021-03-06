CC      = gcc
CFLAGS  =  -m32 -Wall -fno-builtin -nostdinc -nostdlib
LD      = ld

OBJFILES = \
	loader.o  \
	common/print_table.o \
	common/tetris.o \
	common/os_life.o \
	common/handler.o \
	common/io_commands.o \
	common/handler_asm.o \
	common/pic.o \
	common/sleep.o \
	common/gdt.o \
	common/idt.o \
	kernel.o 

image:
	@echo "Creating hdd.img..."
	@dd if=/dev/zero of=./hdd.img bs=512 count=16065 1>/dev/null 2>&1

	@echo "Creating bootable first FAT32 partition..."
	@losetup /dev/loop1 ./hdd.img
	@(echo c; echo u; echo n; echo p; echo 1; echo ;  echo ; echo a; echo 1; echo t; echo c; echo w;) | fdisk /dev/loop1 1>/dev/null 2>&1 || true

	@echo "Mounting partition to /dev/loop2..."
	@losetup /dev/loop2 ./hdd.img \
   	--offset    `echo \`fdisk -lu /dev/loop1 | tail -n1 | awk '{print $$3}'\`*512 | bc` \
   	--sizelimit `echo \`fdisk -lu /dev/loop1 | tail -n1 | awk '{print $$4}'\`*512 | bc`
	@losetup -d /dev/loop1

	@echo "Format partition..."
	@mkdosfs /dev/loop2

	@echo "Copy kernel and grub files on partition..."
	@mkdir -p tempdir
	@mount /dev/loop2 tempdir
	@mkdir tempdir/boot
	@cp -r grub tempdir/boot/
	@cp kernel.bin tempdir/
	@sleep 1
	@umount /dev/loop2
	@rm -r tempdir
	@losetup -d /dev/loop2

	@echo "Installing GRUB..."
	@echo "device (hd0) hdd.img \n \
	       root (hd0,0)         \n \
	       setup (hd0)          \n \
	       quit\n" | grub --batch 1>/dev/null
	@echo "Done!"

all: kernel.bin
rebuild: clean all
.s.o:
	as --32 -o $@ $<
.c.o:
	$(CC) -Iinclude $(CFLAGS) -o $@ -c $<
kernel.bin: $(OBJFILES)
	$(LD) -m elf_i386 -T linker.ld -o $@ $^
clean:
	rm -f $(OBJFILES) hdd.img kernel.bin
