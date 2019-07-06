#include <openssl/evp.h>
#include <stdint.h>

void encrypt(uint8_t* out, const uint8_t* in, const uint8_t* key, const uint8_t* iv, int size)
{
  int out_len, in_len = size;
  EVP_CIPHER_CTX *ctx;
  
  if(!(ctx = EVP_CIPHER_CTX_new())) {
    printf("error EVP_CIPHER_CTX_new\n");
    return;
  }
  
  if(1 != EVP_EncryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv)) {
    printf("error EVP_EncryptInit_ex\n");
    return;
  }
  
  if(1 != EVP_EncryptUpdate(ctx, out, &out_len, in, in_len)) {
    printf("error EVP_EncryptUpdate\n");
    return;
  }
  if (out_len != in_len) {
    printf("error encrypt out_len(%d) != in_len(%d)\n", out_len, in_len);
  }
  
  EVP_CIPHER_CTX_free(ctx);
}

void decrypt(uint8_t* out, const uint8_t* in, const uint8_t* key, const uint8_t* iv, int size)
{
  int out_len, in_len = size;
  EVP_CIPHER_CTX *ctx;
  
  if(!(ctx = EVP_CIPHER_CTX_new())) {
    printf("error EVP_CIPHER_CTX_new\n");
    return;
  }
  
  if(1 != EVP_DecryptInit_ex(ctx, EVP_aes_128_ctr(), NULL, key, iv)) {
    printf("error EVP_DecryptInit_ex\n");
    return;
  }
  
  if(1 != EVP_DecryptUpdate(ctx, out, &out_len, in, in_len)) {
    printf("error EVP_DecryptUpdate\n");
    return;
  }
  if (out_len != in_len) {
    printf("error decrypt out_len(%d) != in_len(%d)\n", out_len, in_len);
  }
  
  EVP_CIPHER_CTX_free(ctx);
}

void print(char* header, uint8_t* buf, int size)
{
  int i;
  printf("%s", header);
  for (i=0; i<size; i++) {
    printf("%02x", buf[i]);
  }
  printf("\n");
}

/*
  NIST Special Publication 800-38A Recommendation for Block 2001 Edition Cipher Modes of Operation
  F.5.1 CTR-AES128.Encrypt
   Key           2b7e151628aed2a6abf7158809cf4f3c
   Init. Counter f0f1f2f3f4f5f6f7f8f9fafbfcfdfeff
   Plaintext     6bc1bee22e409f96e93d7e117393172aae2d8a571e03ac9c9eb76fac45af8e5130c81c46a35ce411e5fbc1191a0a52eff69f2445df4f9b17ad2b417be66c3710
   Ciphertext    874d6191b620e3261bef6864990db6ce9806f66b7970fdff8617187bb9fffdff5ae4df3edbd5d35e5b4f09020db03eab1e031dda2fbe03d1792170a0f3009cee
 */
int main(int argc, char* argv[])
{
#define LEN (64)
  int len = LEN - 4; /* is not multiples of 16. */
  static uint8_t key[16] = {0x2b, 0x7e, 0x15, 0x16, 0x28, 0xae, 0xd2, 0xa6, 0xab, 0xf7, 0x15, 0x88, 0x09, 0xcf, 0x4f, 0x3c};
  static uint8_t iv[16]  = {0xf0, 0xf1, 0xf2, 0xf3, 0xf4, 0xf5, 0xf6, 0xf7, 0xf8, 0xf9, 0xfa, 0xfb, 0xfc, 0xfd, 0xfe, 0xff};
  static uint8_t in[LEN] = {0x6b, 0xc1, 0xbe, 0xe2, 0x2e, 0x40, 0x9f, 0x96, 0xe9, 0x3d, 0x7e, 0x11, 0x73, 0x93, 0x17, 0x2a, 
                            0xae, 0x2d, 0x8a, 0x57, 0x1e, 0x03, 0xac, 0x9c, 0x9e, 0xb7, 0x6f, 0xac, 0x45, 0xaf, 0x8e, 0x51, 
                            0x30, 0xc8, 0x1c, 0x46, 0xa3, 0x5c, 0xe4, 0x11, 0xe5, 0xfb, 0xc1, 0x19, 0x1a, 0x0a, 0x52, 0xef, 
                            0xf6, 0x9f, 0x24, 0x45, 0xdf, 0x4f, 0x9b, 0x17, 0xad, 0x2b, 0x41, 0x7b, 0xe6, 0x6c, 0x37, 0x10};
  static uint8_t out[LEN];
  static uint8_t out2[LEN];
  
  encrypt(out, in, key, iv, len);
  decrypt(out2, out, key, iv, len);
  
  print("ky:", key, 16);
  print("iv:", iv, 16);
  print("encrypt\nin:", in, len);
  print("ou:", out, len);
  print("decrypt\nou:", out2, len);
  
  return 0;
}
