# 1st(%rcx) 2nd(%rdx) 3rd(%r8) 4th(%r9) Microsoft x64 calling convention
  .globl cipher_key_expansion128
  .globl cipher_key_expansion192
  .globl cipher_key_expansion256
  .globl inv_cipher_key_expansion128
  .globl inv_cipher_key_expansion192
  .globl inv_cipher_key_expansion256
  .globl cipher128
  .globl cipher192
  .globl cipher256
  .globl inv_cipher128
  .globl inv_cipher192
  .globl inv_cipher256

# void cipher_key_expansion128(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* key /*Nk*/);
cipher_key_expansion128:
  push %rcx
  movdqu (%rdx), %xmm1
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  aeskeygenassist $0x01, %xmm1, %xmm2 # |a|b|c|d| => |x|-|-|-|, x=rotword(subword(a)) xor Rcon
  call cipher_key_expansion128_round
  aeskeygenassist $0x02, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x04, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x08, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x10, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x20, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x40, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x80, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x1b, %xmm1, %xmm2
  call cipher_key_expansion128_round
  aeskeygenassist $0x36, %xmm1, %xmm2
  call cipher_key_expansion128_round
  pop %rcx
  movdqu     0(%rcx), %xmm0  # round key 0
  movdqu  0x10(%rcx), %xmm1  # round key 1
  movdqu  0x20(%rcx), %xmm2  # round key 2
  movdqu  0x30(%rcx), %xmm3  # round key 3
  movdqu  0x40(%rcx), %xmm4  # round key 4
  movdqu  0x50(%rcx), %xmm5  # round key 5
  movdqu  0x60(%rcx), %xmm6  # round key 6
  movdqu  0x70(%rcx), %xmm7  # round key 7
  movdqu  0x80(%rcx), %xmm8  # round key 8
  movdqu  0x90(%rcx), %xmm9  # round key 9
  movdqu  0xa0(%rcx), %xmm10 # round key 10
  ret

cipher_key_expansion128_round:
  pshufd $0xff, %xmm2, %xmm2 #  |x|-|-|-| => |x|x|x|x|
  vpslldq $0x4, %xmm1, %xmm3 #  |a|b|c|d|
  pxor %xmm3, %xmm1          # +|b|c|d|0|
  vpslldq $0x4, %xmm1, %xmm3 #  |a+b|b+c|c+d|d|
  pxor %xmm3, %xmm1          # +|b+c|c+d|d  |0|
  vpslldq $0x4, %xmm1, %xmm3 #  |a+c|b+d|c|d|
  pxor %xmm3, %xmm1          # +|b+d|c  |d|0|
                             #  |a+b+c+d|b+c+d|c+d|d|
  pxor %xmm2, %xmm1          #  |x+a+b+c+d|x+b+c+d|x+c+d|x+d|
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  ret

# void cipher_key_expansion192(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* key /*Nk*/);
cipher_key_expansion192:
  push %rcx
  movdqu (%rdx), %xmm1     # |c|d|e|f|
  movdqu %xmm1, (%rcx)
  movdqu 0x10(%rdx), %xmm2 # |-|-|a|b|
  movdqu %xmm2, 0x10(%rcx)
  add $0x18, %rcx
  aeskeygenassist $0x01, %xmm2, %xmm3 # |-|-|a|b| => |-|-|x|-|, x=rotword(subword(a)) xor Rcon
  call cipher_key_expansion192_round
  aeskeygenassist $0x02, %xmm2, %xmm3
  call cipher_key_expansion192_round
  aeskeygenassist $0x04, %xmm2, %xmm3
  call cipher_key_expansion192_round
  aeskeygenassist $0x08, %xmm2, %xmm3
  call cipher_key_expansion192_round
  aeskeygenassist $0x10, %xmm2, %xmm3
  call cipher_key_expansion192_round
  aeskeygenassist $0x20, %xmm2, %xmm3
  call cipher_key_expansion192_round
  aeskeygenassist $0x40, %xmm2, %xmm3
  call cipher_key_expansion192_round
  aeskeygenassist $0x80, %xmm2, %xmm3
  pshufd $0x55, %xmm3, %xmm2 #  |-|-|x|-| => |x|x|x|x|
  call cipher_key_expansion128_round
  pop %rcx
  movdqu     0(%rcx), %xmm0  # round key 0
  movdqu  0x10(%rcx), %xmm1  # round key 1
  movdqu  0x20(%rcx), %xmm2  # round key 2
  movdqu  0x30(%rcx), %xmm3  # round key 3
  movdqu  0x40(%rcx), %xmm4  # round key 4
  movdqu  0x50(%rcx), %xmm5  # round key 5
  movdqu  0x60(%rcx), %xmm6  # round key 6
  movdqu  0x70(%rcx), %xmm7  # round key 7
  movdqu  0x80(%rcx), %xmm8  # round key 8
  movdqu  0x90(%rcx), %xmm9  # round key 9
  movdqu  0xa0(%rcx), %xmm10 # round key 10
  movdqu  0xb0(%rcx), %xmm11 # round key 11
  movdqu  0xc0(%rcx), %xmm12 # round key 12
  ret

cipher_key_expansion192_round:
  pshufd $0x55, %xmm3, %xmm3 #  |-|-|x|-| => |x|x|x|x|
  vpslldq $0x4, %xmm1, %xmm4 #  |c|d|e|f|
  pxor %xmm4, %xmm1          # +|d|e|f|0|
  vpslldq $0x4, %xmm1, %xmm4 #  |c+d|d+e|e+f|f|
  pxor %xmm4, %xmm1          # +|d+e|e+f|f  |0|
  vpslldq $0x4, %xmm1, %xmm4 #  |c+e|d+f|e|f|
  pxor %xmm4, %xmm1          # +|d+f|e  |f|0|
                             #  |c+d+e+f|d+e+f|e+f|f|
  pxor %xmm3, %xmm1          #  |x+c+d+e+f|x+d+e+f|x+e+f|x+f|
  vpslldq $0x4, %xmm2, %xmm4 #  |-|-|a|b|
  pxor %xmm4, %xmm2          # +|-|a|b|0|
                             #  |-|-|a+b|b|
  pshufd $0x0f, %xmm1, %xmm3 #  |-|-|x+c+d+e+f|x+c+d+e+f|
  pxor %xmm3, %xmm2          # +|-|-|a+b      |b      |
                             #  |-|-|x+a+b+c+d+e+f|x+b+c+d+e+f|
  movdqu %xmm1, (%rcx)
  movdqu %xmm2, 0x10(%rcx)
  add $0x18, %rcx
  ret

# void cipher_key_expansion256(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* key /*Nk*/);
cipher_key_expansion256:
  push %rcx
  movdqu (%rdx), %xmm1     # |e|f|g|h|
  movdqu %xmm1, (%rcx)
  movdqu 0x10(%rdx), %xmm2 # |a|b|c|d|
  movdqu %xmm2, 0x10(%rcx)
  add $0x20, %rcx
  aeskeygenassist $0x01, %xmm2, %xmm3 # |a|b|c|d| => |x|-|-|-|, x=rotword(subword(a)) xor Rcon
  call cipher_key_expansion256_round
  aeskeygenassist $0x02, %xmm2, %xmm3
  call cipher_key_expansion256_round
  aeskeygenassist $0x04, %xmm2, %xmm3
  call cipher_key_expansion256_round
  aeskeygenassist $0x08, %xmm2, %xmm3
  call cipher_key_expansion256_round
  aeskeygenassist $0x10, %xmm2, %xmm3
  call cipher_key_expansion256_round
  aeskeygenassist $0x20, %xmm2, %xmm3
  call cipher_key_expansion256_round
  aeskeygenassist $0x40, %xmm2, %xmm2
  call cipher_key_expansion128_round
  pop %rcx
  movdqu     0(%rcx), %xmm0  # round key 0
  movdqu  0x10(%rcx), %xmm1  # round key 1
  movdqu  0x20(%rcx), %xmm2  # round key 2
  movdqu  0x30(%rcx), %xmm3  # round key 3
  movdqu  0x40(%rcx), %xmm4  # round key 4
  movdqu  0x50(%rcx), %xmm5  # round key 5
  movdqu  0x60(%rcx), %xmm6  # round key 6
  movdqu  0x70(%rcx), %xmm7  # round key 7
  movdqu  0x80(%rcx), %xmm8  # round key 8
  movdqu  0x90(%rcx), %xmm9  # round key 9
  movdqu  0xa0(%rcx), %xmm10 # round key 10
  movdqu  0xb0(%rcx), %xmm11 # round key 11
  movdqu  0xc0(%rcx), %xmm12 # round key 12
  movdqu  0xd0(%rcx), %xmm13 # round key 13
  movdqu  0xe0(%rcx), %xmm14 # round key 14
  ret

cipher_key_expansion256_round:
  pshufd $0xff, %xmm3, %xmm3 #  |x|-|-|-| => |x|x|x|x|
  vpslldq $0x4, %xmm1, %xmm4 #  |e|f|g|h|
  pxor %xmm4, %xmm1          # +|f|g|h|0|
  vpslldq $0x4, %xmm1, %xmm4 #  |e+f|f+g|g+h|h|
  pxor %xmm4, %xmm1          # +|f+g|g+h|h  |0|
  vpslldq $0x4, %xmm1, %xmm4 #  |e+g|f+h|g|h|
  pxor %xmm4, %xmm1          # +|f+h|g|f|0|
                             #  |e+f+g+h|f+g+h|g+h|h|
  pxor %xmm3, %xmm1          #  |x+e+f+g+h|x+f+g+h|x+g+h|x+h|
  aeskeygenassist $0x01, %xmm1, %xmm3
  pshufd $0xaa, %xmm3, %xmm3 #  |-|y|-|-| => |y|y|y|y|
  vpslldq $0x4, %xmm2, %xmm4 #  |a|b|c|d|
  pxor %xmm4, %xmm2          # +|b|c|d|0|
  vpslldq $0x4, %xmm2, %xmm4 #  |a+b|b+c|c+d|d|
  pxor %xmm4, %xmm2          # +|b+c|c+d|d  |0|
  vpslldq $0x4, %xmm2, %xmm4 #  |a+c|b+d|c|d|
  pxor %xmm4, %xmm2          # +|b+d|c|d|0|
                             #  |a+b+c+d|b+c+d|c+d|d|
  pxor %xmm3, %xmm2          #  |y+a+b+c+d|y+b+c+d|y+c+d|y+d|
  movdqu %xmm1, (%rcx)
  movdqu %xmm2, 0x10(%rcx)
  add $0x20, %rcx
  ret

# void inv_cipher_key_expansion128(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* cipher_w /*Nb*(Nr+1)*/);
inv_cipher_key_expansion128:
  push %rcx
  push %rdx
  movdqu (%rdx), %xmm1
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  add $0x10, %rdx
  mov $9, %rax
inv_cipher_key_expansion128_loop:
  movdqu (%rdx), %xmm1
  aesimc %xmm1, %xmm1
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  add $0x10, %rdx
  dec %rax
  jne inv_cipher_key_expansion128_loop
  movdqu (%rdx), %xmm1
  movdqu %xmm1, (%rcx)
  pop %rdx
  pop %rcx
  movdqu     0(%rcx), %xmm0  # round key 0
  movdqu  0x10(%rcx), %xmm1  # round key 1
  movdqu  0x20(%rcx), %xmm2  # round key 2
  movdqu  0x30(%rcx), %xmm3  # round key 3
  movdqu  0x40(%rcx), %xmm4  # round key 4
  movdqu  0x50(%rcx), %xmm5  # round key 5
  movdqu  0x60(%rcx), %xmm6  # round key 6
  movdqu  0x70(%rcx), %xmm7  # round key 7
  movdqu  0x80(%rcx), %xmm8  # round key 8
  movdqu  0x90(%rcx), %xmm9  # round key 9
  movdqu  0xa0(%rcx), %xmm10 # round key 10
  ret

# void inv_cipher_key_expansion192(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* cipher_w /*Nb*(Nr+1)*/);
inv_cipher_key_expansion192:
  push %rcx
  push %rdx
  movdqu (%rdx), %xmm1
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  add $0x10, %rdx
  mov $11, %rax
inv_cipher_key_expansion192_loop:
  movdqu (%rdx), %xmm1
  aesimc %xmm1, %xmm1
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  add $0x10, %rdx
  dec %rax
  jne inv_cipher_key_expansion192_loop
  movdqu (%rdx), %xmm1
  movdqu %xmm1, (%rcx)
  pop %rdx
  pop %rcx
  movdqu     0(%rcx), %xmm0  # round key 0
  movdqu  0x10(%rcx), %xmm1  # round key 1
  movdqu  0x20(%rcx), %xmm2  # round key 2
  movdqu  0x30(%rcx), %xmm3  # round key 3
  movdqu  0x40(%rcx), %xmm4  # round key 4
  movdqu  0x50(%rcx), %xmm5  # round key 5
  movdqu  0x60(%rcx), %xmm6  # round key 6
  movdqu  0x70(%rcx), %xmm7  # round key 7
  movdqu  0x80(%rcx), %xmm8  # round key 8
  movdqu  0x90(%rcx), %xmm9  # round key 9
  movdqu  0xa0(%rcx), %xmm10 # round key 10
  movdqu  0xb0(%rcx), %xmm11 # round key 11
  movdqu  0xc0(%rcx), %xmm12 # round key 12
  ret

# void inv_cipher_key_expansion256(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* cipher_w /*Nb*(Nr+1)*/);
inv_cipher_key_expansion256:
  push %rcx
  push %rdx
  movdqu (%rdx), %xmm1
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  add $0x10, %rdx
  mov $13, %rax
inv_cipher_key_expansion256_loop:
  movdqu (%rdx), %xmm1
  aesimc %xmm1, %xmm1
  movdqu %xmm1, (%rcx)
  add $0x10, %rcx
  add $0x10, %rdx
  dec %rax
  jne inv_cipher_key_expansion256_loop
  movdqu (%rdx), %xmm1
  movdqu %xmm1, (%rcx)
  pop %rdx
  pop %rcx
  movdqu     0(%rcx), %xmm0  # round key 0
  movdqu  0x10(%rcx), %xmm1  # round key 1
  movdqu  0x20(%rcx), %xmm2  # round key 2
  movdqu  0x30(%rcx), %xmm3  # round key 3
  movdqu  0x40(%rcx), %xmm4  # round key 4
  movdqu  0x50(%rcx), %xmm5  # round key 5
  movdqu  0x60(%rcx), %xmm6  # round key 6
  movdqu  0x70(%rcx), %xmm7  # round key 7
  movdqu  0x80(%rcx), %xmm8  # round key 8
  movdqu  0x90(%rcx), %xmm9  # round key 9
  movdqu  0xa0(%rcx), %xmm10 # round key 10
  movdqu  0xb0(%rcx), %xmm11 # round key 11
  movdqu  0xc0(%rcx), %xmm12 # round key 12
  movdqu  0xd0(%rcx), %xmm13 # round key 13
  movdqu  0xe0(%rcx), %xmm14 # round key 14
  ret

# void cipher128(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
# Registers xmm0-xmm10 hold the round keys(from 0 to 10 in this order).
cipher128:
  movdqu     (%rdx), %xmm15 # in
  pxor        %xmm0, %xmm15 # round 0
  aesenc      %xmm1, %xmm15 # round 1
  aesenc      %xmm2, %xmm15 # round 2
  aesenc      %xmm3, %xmm15 # round 3
  aesenc      %xmm4, %xmm15 # round 4
  aesenc      %xmm5, %xmm15 # round 5
  aesenc      %xmm6, %xmm15 # round 6
  aesenc      %xmm7, %xmm15 # round 7
  aesenc      %xmm8, %xmm15 # round 8
  aesenc      %xmm9, %xmm15 # round 9
  aesenclast %xmm10, %xmm15 # round 10
  movdqu     %xmm15, (%rcx) # out
  ret

# void cipher192(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
# Registers xmm0-xmm12 hold the round keys(from 0 to 12 in this order).
cipher192:
  movdqu     (%rdx), %xmm15 # in
  pxor        %xmm0, %xmm15 # round 0
  aesenc      %xmm1, %xmm15 # round 1
  aesenc      %xmm2, %xmm15 # round 2
  aesenc      %xmm3, %xmm15 # round 3
  aesenc      %xmm4, %xmm15 # round 4
  aesenc      %xmm5, %xmm15 # round 5
  aesenc      %xmm6, %xmm15 # round 6
  aesenc      %xmm7, %xmm15 # round 7
  aesenc      %xmm8, %xmm15 # round 8
  aesenc      %xmm9, %xmm15 # round 9
  aesenc     %xmm10, %xmm15 # round 10
  aesenc     %xmm11, %xmm15 # round 11
  aesenclast %xmm12, %xmm15 # round 12
  movdqu     %xmm15, (%rcx) # out
  ret

# void cipher256(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
# Registers xmm0-xmm14 hold the round keys(from 0 to 14 in this order).
cipher256:
  movdqu     (%rdx), %xmm15 # in
  pxor        %xmm0, %xmm15 # round 0
  aesenc      %xmm1, %xmm15 # round 1
  aesenc      %xmm2, %xmm15 # round 2
  aesenc      %xmm3, %xmm15 # round 3
  aesenc      %xmm4, %xmm15 # round 4
  aesenc      %xmm5, %xmm15 # round 5
  aesenc      %xmm6, %xmm15 # round 6
  aesenc      %xmm7, %xmm15 # round 7
  aesenc      %xmm8, %xmm15 # round 8
  aesenc      %xmm9, %xmm15 # round 9
  aesenc     %xmm10, %xmm15 # round 10
  aesenc     %xmm11, %xmm15 # round 11
  aesenc     %xmm12, %xmm15 # round 12
  aesenc     %xmm13, %xmm15 # round 13
  aesenclast %xmm14, %xmm15 # round 14
  movdqu     %xmm15, (%rcx) # out
  ret

# void inv_cipher128(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
# Registers xmm0-xmm10 hold the round keys(from 0 to 10 in this order).
inv_cipher128:
  movdqu     (%rdx), %xmm15 # in
  pxor       %xmm10, %xmm15 # round 10
  aesdec      %xmm9, %xmm15 # round 9
  aesdec      %xmm8, %xmm15 # round 8
  aesdec      %xmm7, %xmm15 # round 7
  aesdec      %xmm6, %xmm15 # round 6
  aesdec      %xmm5, %xmm15 # round 5
  aesdec      %xmm4, %xmm15 # round 4
  aesdec      %xmm3, %xmm15 # round 3
  aesdec      %xmm2, %xmm15 # round 2
  aesdec      %xmm1, %xmm15 # round 1
  aesdeclast  %xmm0, %xmm15 # round 0
  movdqu     %xmm15, (%rcx) # out
  ret

# void inv_cipher192(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
# Registers xmm0-xmm12 hold the round keys(from 0 to 12 in this order).
inv_cipher192:
  movdqu     (%rdx), %xmm15 # in
  pxor       %xmm12, %xmm15 # round 12
  aesdec     %xmm11, %xmm15 # round 11
  aesdec     %xmm10, %xmm15 # round 10
  aesdec      %xmm9, %xmm15 # round 9
  aesdec      %xmm8, %xmm15 # round 8
  aesdec      %xmm7, %xmm15 # round 7
  aesdec      %xmm6, %xmm15 # round 6
  aesdec      %xmm5, %xmm15 # round 5
  aesdec      %xmm4, %xmm15 # round 4
  aesdec      %xmm3, %xmm15 # round 3
  aesdec      %xmm2, %xmm15 # round 2
  aesdec      %xmm1, %xmm15 # round 1
  aesdeclast  %xmm0, %xmm15 # round 0
  movdqu     %xmm15, (%rcx) # out
  ret

# void inv_cipher256(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
# Registers xmm0-xmm14 hold the round keys(from 0 to 14 in this order).
inv_cipher256:
  movdqu     (%rdx), %xmm15 # in
  pxor       %xmm14, %xmm15 # round 14
  aesdec     %xmm13, %xmm15 # round 13
  aesdec     %xmm12, %xmm15 # round 12
  aesdec     %xmm11, %xmm15 # round 11
  aesdec     %xmm10, %xmm15 # round 10
  aesdec      %xmm9, %xmm15 # round 9
  aesdec      %xmm8, %xmm15 # round 8
  aesdec      %xmm7, %xmm15 # round 7
  aesdec      %xmm6, %xmm15 # round 6
  aesdec      %xmm5, %xmm15 # round 5
  aesdec      %xmm4, %xmm15 # round 4
  aesdec      %xmm3, %xmm15 # round 3
  aesdec      %xmm2, %xmm15 # round 2
  aesdec      %xmm1, %xmm15 # round 1
  aesdeclast  %xmm0, %xmm15 # round 0
  movdqu     %xmm15, (%rcx) # out
  ret
