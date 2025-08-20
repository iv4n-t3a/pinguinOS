include ../../config.mk

BUILDDIR=../../build

$(BUILDDIR)/stage2.img: $(BUILDDIR)/stage2_entry.o stage2.ld
	$(LD) $(LD_FLAGS) -T stage2.ld -o $(BUILDDIR)/stage2.img $(BUILDDIR)/stage2_entry.o

$(BUILDDIR)/stage2_entry.o: entry.S
	$(AS) $(AS_FLAGS) -o $(BUILDDIR)/stage2_entry.o entry.S
