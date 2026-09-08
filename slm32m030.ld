/* SLM32X030 linker script
 * 256KB flash, 32KB SRAM (0x20000000 ~ 0x20008000)
 */
ENTRY(Reset_Handler)

_estack          = ORIGIN(RAM) + LENGTH(RAM);
_Min_Stack_Size  = 0x400;   /* stack 2KB */

MEMORY
{
  FLASH (rx)     : ORIGIN = 0x08000000, LENGTH = 256K
  RAM (xrw)      : ORIGIN = 0x20000000, LENGTH = 32K
}

SECTIONS
{
  .isr_vector : { . = ALIGN(4); KEEP(*(.isr_vector)) . = ALIGN(4); } >FLASH

  .text : {
    . = ALIGN(4);
    *(.text*)
    *(.rodata*)
    . = ALIGN(4);
    _etext = .;
  } >FLASH

  .ARM.extab : { *(.ARM.extab*) } >FLASH
  .ARM       : { *(.ARM.exidx*) } >FLASH

  _sidata = LOADADDR(.data);
  .data : {
    . = ALIGN(4);
    _sdata = .;
    *(.data*)
    . = ALIGN(4);
    _edata = .;
  } >RAM AT>FLASH

  .bss : {
    . = ALIGN(4);
    _sbss = .;
    *(.bss*)
    *(COMMON)
    . = ALIGN(4);
    _ebss = .;
  } >RAM

  /* Heap boundary for newlib (_sbrk returns this address). Nothing in the
   * firmware mallocs today; the symbol must exist so the nano libc links. */
  . = ALIGN(4);
  PROVIDE( end = . );

  /* stack guard section, placed after the heap (high address) */
  ._stack_guard : {
    . = ALIGN(8);
    . = . + _Min_Stack_Size;
    . = ALIGN(8);
  } >RAM
}