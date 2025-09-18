# !/bin/bash

TARGET=$BUILDDIR/bootable.dd

dd if=/dev/zero of=$TARGET bs=1048576 count=32
mkfs.fat -F 16 -S 512 -s 4 -R 1 -f 2 -r 512 --offset=1 $TARGET
dd if=$BUILDDIR/boot/stage1/mbr.img of=$TARGET        bs=512 count=1 conv=notrunc
dd if=$BUILDDIR/boot/stage1/vbr.img of=$TARGET seek=1 bs=512 count=1 conv=notrunc
mcopy -i $TARGET@@512 $BUILDDIR/boot/stage2/stage2.bin ::stage2.bin
mcopy -i $TARGET@@512 $BUILDDIR/kernel/kernel.elf ::kernel.elf
