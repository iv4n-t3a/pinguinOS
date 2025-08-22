include ../config.mk

CSOURCES := $(shell find . -type f -name '*.c')
CHEADERS := $(shell find . -type f -name '*.h')
OBJECTS := $(patsubst ./%.c,$(BUILD_DIR)/kernel_libs_%.o,$(CSOURCES))
TARGET := $(KERNEL_LIBS)

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJECTS)
	$(LD) $(LD_FLAGS) -r -o $@ $^

$(BUILD_DIR)/kernel_libs_%.o: %.c
	$(CC) $(CC_FLAGS) -c -o $@ $<
