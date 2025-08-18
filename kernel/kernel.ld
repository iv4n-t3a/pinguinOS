OUTPUT_FORMAT(binary)
SECTIONS
{
    .text :
    {
        /* To analyze how much space is left */
        FILL(0x90)
        *(.text)
        . = 0x1200;
        *(.far_text)
    }
}
