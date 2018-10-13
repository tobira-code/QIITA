    .globl  fsin
fsin:
    movq   %xmm0, -8(%rsp)
    fldl   -8(%rsp)
    fsin
    fstpl  -8(%rsp)
    movq   -8(%rsp), %xmm0
    ret
