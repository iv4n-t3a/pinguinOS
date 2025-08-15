build/bootable.dd: build/mbr.img build/vbr.img build/kernel.img
	dd if=/dev/zero of=$@ bs=1048576 count=128
	mkfs.fat -F 16 -S 512 -s 4 -R 1 -f 2 -r 512 --offset=1 $@
	dd if=build/mbr.img of=$@        bs=512     count=1    conv=notrunc
	dd if=build/vbr.img of=$@ seek=1 bs=512     count=1    conv=notrunc
	mcopy -i $@@@512 build/kernel.img ::kernel.bin

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

build/kernel.img: build/kernel.o kernel/kernel.ld
	mkdir -p build
	ld -T kernel/kernel.ld -o build/kernel.img build/kernel.o

build/kernel.o: kernel/kernel.S
	as -o build/kernel.o kernel/kernel.S

run: build/bootable.dd
	qemu-system-i386 -nographic -drive file=build/bootable.dd,format=raw
.PHONY: run

clean:
	rm -r build/*
.PHONY: clean
