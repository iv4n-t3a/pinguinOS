include config.mk

build/bootable.dd: \
		build/mbr.img    \
		build/vbr.img    \
		build/stage2.img \
		build/kernel.img
	dd if=/dev/zero of=$@ bs=1048576 count=32
	mkfs.fat -F 16 -S 512 -s 4 -R 1 -f 2 -r 512 --offset=1 $@
	dd if=build/mbr.img of=$@        bs=512     count=1    conv=notrunc
	dd if=build/vbr.img of=$@ seek=1 bs=512     count=1    conv=notrunc
	mcopy -i $@@@512 build/stage2.img ::stage2.bin
	mcopy -i $@@@512 build/kernel.img ::kernel.elf

build/mbr.img: boot/stage1/*
	make -C boot/stage1

build/vbr.img: boot/stage1/*
	make -C boot/stage1

build/stage2.img: boot/stage2/* build/core_lib.o
	make -C boot/stage2

build/kernel.img: kernel/* build/core_lib.o
	make -C kernel

build/core_lib.o: core_lib/*
	make -C core_lib

tools:
	make -C third-party
.PHONY: tools

run: build/bootable.dd
	qemu-system-i386 -drive file=build/bootable.dd,format=raw -serial mon:stdio
.PHONY: run

debug: build/bootable.dd
	bochs-debugger -debugger
.PHONY: debug

clean:
	rm -r build/* stage2.map kernel.map
.PHONY: clean
