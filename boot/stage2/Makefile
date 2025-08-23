include ../../config.mk

ASM_SOURCES := $(wildcard *.S)
C_SOURCES := $(wildcard *.c)
HEADERS := $(wildcard *.h ../../core_lib/*.h)

ASM_OBJECTS := $(ASM_SOURCES:%.S=$(BUILD_DIR)stage2_%.o)
C_OBJECTS := $(C_SOURCES:%.c=$(BUILD_DIR)stage2_%.o)
OBJECTS := $(ASM_OBJECTS) $(C_OBJECTS)

$(BUILD_DIR)stage2.img: stage2.ld $(OBJECTS) $(BUILD_DIR)core_lib.o
	$(LD) $(LD_FLAGS) -T stage2.ld -Map=$(ROOT_DIR)stage2.map \
		-o $@ $(OBJECTS) $(BUILD_DIR)core_lib.o

$(BUILD_DIR)stage2_%.o: %.S $(HEADERS)
	$(AS) $(AS_FLAGS) -o $@ $<

$(BUILD_DIR)stage2_%.o: %.c $(HEADERS)
	$(CC) $(CC_FLAGS) -c $< -o $@
