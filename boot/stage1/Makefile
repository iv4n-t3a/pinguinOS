include ../../config.mk

all: $(BUILD_DIR)/mbr.img $(BUILD_DIR)/vbr.img
.PHONY: all

$(BUILD_DIR)/mbr.img: $(BUILD_DIR)/mbr.o mbr.ld
	$(LD) $(LD_FLAGS) -T mbr.ld -o $(BUILD_DIR)/mbr.img $(BUILD_DIR)/mbr.o

$(BUILD_DIR)/mbr.o: mbr.S
	$(AS) $(AS_FLAGS) -o $(BUILD_DIR)/mbr.o mbr.S

$(BUILD_DIR)/vbr.img: $(BUILD_DIR)/vbr.o vbr.ld
	$(LD) $(LD_FLAGS) -T vbr.ld -o $(BUILD_DIR)/vbr.img $(BUILD_DIR)/vbr.o

$(BUILD_DIR)/vbr.o: vbr.S
	$(AS) $(AS_FLAGS) -o $(BUILD_DIR)/vbr.o vbr.S

