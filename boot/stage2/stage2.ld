OUTPUT_FORMAT(binary)

SECTIONS {
  . = 0x8000;

  .entry : {
    _ENTRY_START_ = .;
    *(.entry)
    _ENTRY_END_ = .;
  }

  .text : {
    _TEXT_START_ = .;
    *(.text)
    _TEXT_END_ = .;
  }

  .rodata : {
    _RODATA_START_ = .;
     *(.rodata)
    _RODATA_END_ = .;
  }

  .data : ALIGN(CONSTANT(MAXPAGESIZE)) {
    _DATA_START_ = .;
     *(.data)
    _DATA_END_ = .;
  }

  .bss : ALIGN(CONSTANT(MAXPAGESIZE)) {
    _BSS_START_ = .;
     *(.bss)
    _BSS_END_ = .;
  }
}
