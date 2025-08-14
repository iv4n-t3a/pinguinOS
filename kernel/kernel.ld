OUTPUT_FORMAT(binary)
SECTIONS
{
    . = 0x7E00;
    .text :
    {
        /* To analyze how much space is left */
        FILL(0xc6)
        *(.text)
    }
}
