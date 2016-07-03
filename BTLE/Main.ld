
MEMORY
{
    CODEFLASH (RWX)   : ORIGIN = 0x00000000, LENGTH = 0x20000
    SRAM (RWX)        : ORIGIN = 0x20000000, LENGTH = 0x4000
}

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.vectors))
        *(.text*)
        *(.rodata*)
        _etext = .;

    } > CODEFLASH= 0

    .data : AT(ADDR(.text) + SIZEOF(.text))
    {
        _data = .;
        *(vtable)
        *(.data*)
        _edata = .;

    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        _ebss = .;

    } > SRAM

}
