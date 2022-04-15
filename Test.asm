0x3F800000 ;1.0
0x3F000000 ;0.5
0x3E800000 ;0.25

$main
li r3 0x0000 ;load offset into register
lfs r3 r3 0x0000
lfs r4 r3 0x0000
lfs r5 r3 0x0000
lfs r6 r3 0x0001
lfs r7 r3 0x0002
lfs r8 r3 0x0003
b dotProduct ;call dotProduct
blr

$dotProduct
fmul r0 r3 r6 
fmul r1 r4 r7
fmul r2 r5 r8
fadd r1 r1 r2
fadd r0 r0 r1 ;result is in the r0 register
blr
