# 1st(%rcx) 2nd(%rdx) 3rd(%r8) 4th(%r9) Microsoft x64 calling convention
  .globl _stosq
  .globl _movsq

# void _stosq(uint64_t size, uint64_t* buf, uint64_t value);
_stosq:
  push %rcx
  push %rdi
  mov %r8, %rax
                 # %rcx = size
  mov %rdx, %rdi # %rdi = buf
  rep stosq
  pop %rdi
  pop %rcx
  ret

# void _movsq(uint64_t* dst, uint64_t* src, uint64_t size);
_movsq:
  push %rsi
  push %rdi
  push %rcx
  mov %rcx, %rdi
  mov %rdx, %rsi
  mov %r8, %rcx # %rcx = size
  rep movsq
  pop %rcx
  pop %rdi
  pop %rsi
  ret
