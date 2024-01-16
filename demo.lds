ENTRY(_start)
SECTIONS
{
    . = 0x80000000;
    . = ALIGN(0x1000);
    .text : { 
        *(.text._start)
        *(.text) 
    }
    . = 0x80005000;
    .data : { *(.data) }
    .bss : { *(.bss) }
}