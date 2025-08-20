include ../../config.mk

BUILDDIR=../../build

$(BUILDDIR)/mbr.img: $(BUILDDIR)/mbr.o mbr.ld
	$(LD) $(LD_FLAGS) -T mbr.ld -o $(BUILDDIR)/mbr.img $(BUILDDIR)/mbr.o

$(BUILDDIR)/mbr.o: mbr.S
	$(AS) $(AS_FLAGS) -o $(BUILDDIR)/mbr.o mbr.S

$(BUILDDIR)/vbr.img: $(BUILDDIR)/vbr.o vbr.ld
	$(LD) $(LD_FLAGS) -T vbr.ld -o $(BUILDDIR)/vbr.img $(BUILDDIR)/vbr.o

$(BUILDDIR)/vbr.o: vbr.S
	$(AS) $(AS_FLAGS) -o $(BUILDDIR)/vbr.o vbr.S

