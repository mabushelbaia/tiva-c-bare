/* Linker script for the TM4C123GH6PM: 256 KB flash, 32 KB SRAM.
 * Symbol names (_ldata/_data/_edata/_bss/_ebss, _estack) match the CMSIS-style
 * device/startup_TM4C123.c. The `vtable` section holds the RAM vector table
 * used by driverlib's runtime interrupt registration (IntRegister), which
 * the UART and SysTick drivers rely on; it must be 1 KB aligned and is
 * placed first in SRAM so it lands on a 1 KB boundary (0x20000000). */

MEMORY
{
    FLASH (rx) : ORIGIN = 0x00000000, LENGTH = 0x00040000
    SRAM (rwx) : ORIGIN = 0x20000000, LENGTH = 0x00008000
}

ENTRY(Reset_Handler)

/* Initial stack pointer: top of SRAM, grows down. Loaded from vector[0]. */
_estack = ORIGIN(SRAM) + LENGTH(SRAM);

SECTIONS
{
    .text :
    {
        _text = .;
        KEEP(*(.isr_vector))
        *(.text*)
        *(.rodata*)
        . = ALIGN(4);
    } > FLASH

    /* GCC-generated exception unwind tables. Kept in FLASH so .data's load
     * address (set below) starts after them instead of overlapping. */
    .ARM.exidx :
    {
        __exidx_start = .;
        *(.ARM.exidx*)
        __exidx_end = .;
        . = ALIGN(4);
        _etext = .;
    } > FLASH

    .data : AT(_etext)
    {
        _data = .;
        _ldata = LOADADDR(.data);
        *(vtable)
        *(.data*)
        . = ALIGN(4);
        _edata = .;
    } > SRAM

    .bss :
    {
        _bss = .;
        *(.bss*)
        *(COMMON)
        . = ALIGN(4);
        _ebss = .;
    } > SRAM
}
