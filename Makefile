AS=as
CC=gcc
LD=ld

AS_FLAGS=
CC_FLAGS=
LD_FLAGS=

build/bootable.dd: build/mbr.img build/vbr.img build/stage2.img
	dd if=/dev/zero of=$@ bs=1048576 count=32
	mkfs.fat -F 16 -S 512 -s 4 -R 1 -f 2 -r 512 --offset=1 $@
	dd if=build/mbr.img of=$@        bs=512     count=1    conv=notrunc
	dd if=build/vbr.img of=$@ seek=1 bs=512     count=1    conv=notrunc
	mcopy -i $@@@512 build/stage2.img ::stage2.bin

build/mbr.img: build/mbr.o boot/stage1/mbr.ld
	mkdir -p build
	$(LD) $(LD_FLAGS) -T boot/stage1/mbr.ld -o build/mbr.img build/mbr.o

build/mbr.o: boot/stage1/mbr.S
	$(AS) $(AS_FLAGS) -o build/mbr.o boot/stage1/mbr.S

build/vbr.img: build/vbr.o boot/stage1/vbr.ld
	mkdir -p build
	$(LD) $(LD_FLAGS) -T boot/stage1/vbr.ld -o build/vbr.img build/vbr.o

build/vbr.o: boot/stage1/vbr.S
	$(AS) $(AS_FLAGS) -o build/vbr.o boot/stage1/vbr.S

build/stage2.img: build/stage2_entry.o boot/stage2/stage2.ld
	mkdir -p build
	$(LD) $(LD_FLAGS) -T boot/stage2/stage2.ld -o build/stage2.img build/stage2_entry.o

build/stage2_entry.o: boot/stage2/entry.S
	$(AS) $(AS_FLAGS) -o build/stage2_entry.o boot/stage2/entry.S

run: build/bootable.dd
	qemu-system-i386 -nographic -drive file=build/bootable.dd,format=raw -serial mon:stdio
.PHONY: run

debug: build/bootable.dd
	bochs-debugger -debugger
.PHONY: debug

clean:
	rm -r build/*
.PHONY: clean
