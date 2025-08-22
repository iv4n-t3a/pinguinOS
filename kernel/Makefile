include ../config.mk

ASM_SOURCES := $(wildcard *.S)
C_SOURCES := $(wildcard *.c)
HEADERS := $(wildcard *.h ../../kernel_libs/*.h)

ASM_OBJECTS := $(ASM_SOURCES:%.S=$(BUILD_DIR)kernel_%.o)
C_OBJECTS := $(C_SOURCES:%.c=$(BUILD_DIR)kernel_%.o)
OBJECTS := $(ASM_OBJECTS) $(C_OBJECTS)

$(BUILD_DIR)kernel.img: $(OBJECTS) $(KERNEL_LIBS)
	$(LD) $(LD_FLAGS) -T kernel.ld -o $@ $^

$(BUILD_DIR)kernel_%.o: %.S $(HEADERS)
	$(AS) $(AS_FLAGS) -o $@ $<

$(BUILD_DIR)kernel_%.o: %.c $(HEADERS)
	$(CC) $(CC_FLAGS) -c $< -o $@
