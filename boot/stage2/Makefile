include ../../config.mk

BUILDDIR=../../build

$(BUILDDIR)/stage2.img: stage2.ld \
		$(BUILDDIR)/stage2_entry.o \
		$(BUILDDIR)/stage2_main.o
	$(LD) $(LD_FLAGS) -T stage2.ld -o $(BUILDDIR)/stage2.img \
		$(BUILDDIR)/stage2_entry.o \
		$(BUILDDIR)/stage2_main.o \

$(BUILDDIR)/stage2_entry.o: entry.S
	$(AS) $(AS_FLAGS) -o $@ $<

$(BUILDDIR)/stage2_main.o: main.c
	$(CC) $(CC_FLAGS) -c $< -o $@
