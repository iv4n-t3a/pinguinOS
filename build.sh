#!/bin/bash

create_bootable() {
    set -e
    mkdir -p $BUILDDIR
    cmake -S . -B $BUILDDIR -DCMAKE_TOOLCHAIN_FILE=$TOOLCHAIN
    make -C $BUILDDIR
    dd if=/dev/zero of=$BOOTABLE bs=1048576 count=32
    mkfs.fat -F 16 -S 512 -s 4 -R 1 -f 2 -r 512 --offset=1 $BOOTABLE
    dd if=$BUILDDIR/boot/stage1/mbr.img of=$BOOTABLE        bs=512 count=1 conv=notrunc
    dd if=$BUILDDIR/boot/stage1/vbr.img of=$BOOTABLE seek=1 bs=512 count=1 conv=notrunc
    mcopy -i $BOOTABLE@@512 $BUILDDIR/boot/stage2/stage2.bin ::stage2.bin
    mcopy -i $BOOTABLE@@512 $BUILDDIR/kernel/kernel.elf ::kernel.elf
}

run_tests() {
    echo "Unimplemented"
}

run_qemu() {
    qemu-system-i386 -debugcon stdio -drive file=$BOOTABLE,format=raw
}

run_bochs_debugger() {
    bochs-debugger -debugger
}

build_tools() {
    make -C third_party
}

clean() {
    rm -rf build
    rm -f stage2.map kernel.map
}

show_help() {
    echo "Usage: $0 [COMMAND] [OPTION]..."
    echo ""
    echo "Commands:"
    echo "  bootable        Creates bootable CD"
    echo "  tests           Run tests"
    echo "  qemu            Run in QEMU"
    echo "  bochs-debugger  Run in Bochs"
    echo "  tools           Build tools (cross-compiler, etc)"
    echo "  clean           Clean binaries"
    echo ""
    echo "Options:"
    echo "  --arch ARCH           Target architecture (One of the following: x86)"
    echo "  --toolchain PATH      Path to CMake toolchain file"
    echo "  --builddir PATH       Path to CMake build dir"
    echo "  -h, --help            Show this help message"
}

ARCH="x86"
BUILDDIR="build"

# Parse arguments
POSITIONAL=()
while [[ $# -gt 0 ]]; do
    case "$1" in
        bootable|tests|qemu|bochs-debugger|tools|clean)
            COMMAND="$1"
            shift
            ;;
        --arch)
            ARCH="$2"
            shift 2
            ;;
        --toolchain)
            TOOLCHAIN="$2"
            shift 2
            ;;
        --builddir)
            BUILDDIR="$2"
            shift 2
            ;;
        -h|--help)
            show_help
            exit 0
            ;;
        *)
            POSITIONAL+=("$1")
            shift
            ;;
    esac
done

if [[ -z "$COMMAND" ]]; then
    show_help
    exit 1
fi

if [[ -z "$TOOLCHAIN" ]]; then
    TOOLCHAIN="toolchain-$ARCH.cmake"
fi

if [[ "$ARCH" != "x86" ]]; then
    echo "Error: unsupported architecture $ARCH."
    exit 1
fi

if [[ ! -f "$TOOLCHAIN" ]]; then
    echo "Error: Toolchain file '$TOOLCHAIN' does not exist."
    exit 1
fi

BOOTABLE=$BUILDDIR/bootable.dd

case "$COMMAND" in
    bootable)
        echo "Creating bootable CD for $ARCH"
        create_bootable
        ;;
    tests)
        echo "Running tests for $ARCH"
        run_tests
        ;;
    qemu)
        echo "Running in QEMU for $ARCH"
        run_qemu
        ;;
    bochs-debugger)
        echo "Running in Bochs debugger for $ARCH"
        run_bochs_debugger
        ;;
    tools)
        echo "Building tools for $ARCH"
        build_tools
        ;;
    clean)
        echo "Clean binaries"
        clean
        ;;
    *)
        show_help
        exit 1
        ;;
esac
