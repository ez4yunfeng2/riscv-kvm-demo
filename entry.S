.globl _entry, sbi_putchar, sbi_shutdown

.section .text.entry
_entry:
    la sp, _stack_end
    j start

sbi_putchar:
    li a7, 0x01
    ecall
    ret
sbi_shutdown:
    li a7, 0x08
    ecall
    ret

.section .bss.stack
.space 0x1000 * 4
_stack_end:
