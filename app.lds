ENTRY(_entry)
SECTIONS
{
    . = 0x80000000;
    . = ALIGN(0x1000);
    .text : { 
        *(.text.entry)
        *(.text) 
    }
    . = ALIGN(0x1000);
    .data : { 
        *(.data.msg)
        . = ALIGN(0x1000);
        *(.data) 
    }
    .bss : { 
        *(.bss.stack)
        *(.bss) 
    }
}