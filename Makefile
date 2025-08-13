build/bootable.dd: build/mbr.img build/vbr.img
	dd if=/dev/zero     of=$@        bs=1048576 count=128
	dd if=build/mbr.img of=$@        bs=512     count=1    conv=notrunc
	dd if=build/vbr.img of=$@ seek=1 bs=512     count=1    conv=notrunc

build/mbr.img: build/mbr.o boot/mbr.ld
	mkdir -p build
	ld -T boot/mbr.ld -o build/mbr.img build/mbr.o

build/mbr.o: boot/mbr.S
	as -o build/mbr.o boot/mbr.S

build/vbr.img: build/vbr.o boot/vbr.ld
	mkdir -p build
	ld -T boot/vbr.ld -o build/vbr.img build/vbr.o

build/vbr.o: boot/vbr.S
	as -o build/vbr.o boot/vbr.S

run: build/bootable.dd
	qemu-system-i386 -drive file=build/bootable.dd,format=raw
.PHONY: run

gdb: build/bootable.dd
	qemu-system-i386 -drive file=build/bootable.dd,format=raw -s -S
.PHONY: gdb

clean:
	rm -r build/*
.PHONY: clean
