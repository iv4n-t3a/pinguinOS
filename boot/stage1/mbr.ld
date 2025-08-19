OUTPUT_FORMAT(binary)
SECTIONS
{
    .text :
    {
        /* To analyze how much space is left */
        FILL(0x90)

        *(.text)
        . = 0x1B8;
        *(.partition_table)
        . = 0x1FE;
        SHORT(0xAA55)
    }
    /DISCARD/ : { *(*) }
}
