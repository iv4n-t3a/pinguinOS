include ../config.mk

CSOURCES := $(shell find . -type f -name '*.c')
CHEADERS := $(shell find . -type f -name '*.h')
OBJECTS := $(patsubst ./%.c,$(BUILD_DIR)/core_lib_%.o,$(CSOURCES))
TARGET := $(BUILD_DIR)core_lib.o

all: $(TARGET)
.PHONY: all

$(TARGET): $(OBJECTS)
	$(LD) $(LD_FLAGS) -r -o $@ $^

$(BUILD_DIR)/core_lib_%.o: %.c
	$(CC) $(CC_FLAGS) -c -o $@ $<
