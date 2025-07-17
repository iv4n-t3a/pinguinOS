build/mbr.img: build/mbr.o
	mkdir -p build
	ld --oformat binary -o build/mbr.img -Ttext 0x7c00 build/mbr.o

build/mbr.o: mbr.S build
	as -o build/mbr.o mbr.S

run: build/mbr.img
	qemu-system-i386 -drive file=build/mbr.img,format=raw
.PHONY: run

clean:
	rm -r build
.PHONY: clean
