ROOT_DIR := $(dir $(realpath $(lastword $(MAKEFILE_LIST))))

TOOLCHAIN_PREFIX=${ROOT_DIR}/third-party
TARGET=i686-elf

AS=${ROOT_DIR}/third-party/bin/i686-elf-as
CC=${ROOT_DIR}/third-party/bin/i686-elf-gcc
LD=${ROOT_DIR}/third-party/bin/i686-elf-ld

AS_FLAGS=
CC_FLAGS=-I${ROOT_DIR} -ffreestanding
LD_FLAGS=

GCC_VERSION=15.2.0
BINUTILS_VERSION=2.45
GMP_VERSION=6.3.0
MPFR_VERSION=4.2.2
MPC_VERSION=1.3.1

BUILD_TOOLS_JOBS=4
