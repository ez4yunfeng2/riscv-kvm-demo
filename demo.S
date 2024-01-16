.globl _start
.data
.LC0:
    .string "Hello World!\r\n"
.text
_start:
    li      sp,0x5000
    # load string
    lui     a1,%pcrel_hi(.LC0)
    addi    a1,a1,%lo(.LC0)
    # char*
    li      a2,268435456
    
.L2:
    lb      a3,0(a1)
    beqz    a3, .L1 # if t0 == 0 then target
    sw      a3,0(a2)
    addi    a1,a1,1
    bne     a3, zero, .L2 # if t0 != t1 then target
    
.L1:
    j .L1

