include ../config.mk

# TODO: Add architecture dependent sources set configuration
C_SOURCES := $(wildcard *.c) \
             $(wildcard */*.c) \
             $(wildcard */*/*.c) \
             $(wildcard */*/*/*.c) \
             $(wildcard */*/*/*/*.c)

ASM_SOURCES := $(wildcard *.S) \
               $(wildcard */*.S) \
               $(wildcard */*/*.S) \
               $(wildcard */*/*/*.S) \
               $(wildcard */*/*/*/*.S)

HEADERS := $(wildcard *.h ../../kernel_libs/*.h)
C_OBJECTS := $(patsubst %.c,$(BUILD_DIR)kernel_%.o,$(C_SOURCES))
ASM_OBJECTS := $(patsubst %.S,$(BUILD_DIR)kernel_%.o,$(ASM_SOURCES))
OBJECTS := $(C_OBJECTS) $(ASM_OBJECTS)

$(BUILD_DIR)kernel.img: kernel.ld $(OBJECTS) $(BUILD_DIR)core_lib.o
	$(LD) $(LD_FLAGS) -T kernel.ld -Map=$(ROOT_DIR)kernel.map \
		-o $@ $(OBJECTS) $(BUILD_DIR)core_lib.o

$(BUILD_DIR)kernel_%.o: %.c $(HEADERS)
	mkdir -p $(dir $@)
	$(CC) $(CC_FLAGS) -c $< -o $@

$(BUILD_DIR)kernel_%.o: %.S $(HEADERS)
	mkdir -p $(dir $@)
	$(AS) $(AS_FLAGS) -c $< -o $@
