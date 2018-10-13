# rcx(1st) rdx(2nd) r8(3rd) r9(4th)
    .globl  main
main:
    subq    $40, %rsp
    call    __main

    # EDX Bit 08: Invariant TSC available if 1.
    movl    $0x80000007, %eax
    cpuid
    # printf
    leaq    s0, %rcx   # 1st
    movl    %edx, %edx # 2nd
    call    printf

    xorl    %eax, %eax
    addq    $40, %rsp
    ret
    .section .rodata
s0: .asciz "cpuid 0x80000007 edx : 0x%08X \n"
