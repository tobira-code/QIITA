#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define NK_MAX (8)
#define NR_MAX (14)

enum {
  AES128 = 0,
  AES192,
  AES256,
};

uint8_t aes_type = AES128;
const uint8_t Nb = 4;

extern void cipher_key_expansion128(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* key /*Nk*/);
extern void cipher_key_expansion192(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* key /*Nk*/);
extern void cipher_key_expansion256(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* key /*Nk*/);
extern void inv_cipher_key_expansion128(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* cipher_w /*Nb*(Nr+1)*/);
extern void inv_cipher_key_expansion192(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* cipher_w /*Nb*(Nr+1)*/);
extern void inv_cipher_key_expansion256(uint32_t* w /*Nb*(Nr+1)*/, const uint32_t* cipher_w /*Nb*(Nr+1)*/);
extern void cipher128(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
extern void cipher192(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
extern void cipher256(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
extern void inv_cipher128(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
extern void inv_cipher192(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);
extern void inv_cipher256(uint8_t* out /*4*Nb*/, const uint8_t* in /*4*Nb*/);

static void print_Nwords(const uint32_t* word, int N)
{
  int i;
  
  for (i=0; i<N; i++) {
    uint8_t* p = (uint8_t*)(word+i);
    printf("%02x %02x %02x %02x ", p[0], p[1], p[2], p[3]);
  }
}

static void cipher_and_inv_cipher(const uint32_t* key, const uint32_t* in)
{
  uint32_t w[Nb*(NR_MAX+1)], w2[Nb*(NR_MAX+1)], out[4], tmp[4];
  uint8_t Nk;
  
  if (aes_type == AES128) {
    Nk = 4;
  } else if (aes_type == AES192) {
    Nk = 6;
  } else { // aes_type == AES256
    Nk = 8;
  }
  
  printf("Cipher Key = "); print_Nwords(key, Nk); printf("\n");
  if (aes_type == AES128) {
    printf("Input      = "); print_Nwords(in, 4); printf("\n");
    cipher_key_expansion128(w, key);
    cipher128((uint8_t*)out, (uint8_t*)in);
    printf("Output     = "); print_Nwords(out, 4); printf("\n");
    inv_cipher_key_expansion128(w2, w);
    inv_cipher128((uint8_t*)tmp, (uint8_t*)out);
    printf("Input(Inv) = "); print_Nwords(tmp, 4); printf("\n");
    printf("\n");
  } else if (aes_type == AES192) {
    printf("Input      = "); print_Nwords(in, 4); printf("\n");
    cipher_key_expansion192(w, key);
    cipher192((uint8_t*)out, (uint8_t*)in);
    printf("Output     = "); print_Nwords(out, 4); printf("\n");
    inv_cipher_key_expansion192(w2, w);
    inv_cipher192((uint8_t*)tmp, (uint8_t*)out);
    printf("Input(Inv) = "); print_Nwords(tmp, 4); printf("\n");
    printf("\n");
  } else { // aes_type == AES256
    printf("Input      = "); print_Nwords(in, 4); printf("\n");
    cipher_key_expansion256(w, key);
    cipher256((uint8_t*)out, (uint8_t*)in);
    printf("Output     = "); print_Nwords(out, 4); printf("\n");
    inv_cipher_key_expansion256(w2, w);
    inv_cipher256((uint8_t*)tmp, (uint8_t*)out);
    printf("Input(Inv) = "); print_Nwords(tmp, 4); printf("\n");
    printf("\n");
  }
}

int main(int argc, char* argv[])
{
  uint8_t key[NK_MAX*4], in[Nb*4], i;
  
  for (i=0; i<Nb*4; i++) {
    in[i] = i << 4 | i;
  }
  
  printf("AES-128\n");
  aes_type = AES128;
  memset(key, 0, sizeof(key));
  for (i=0; i<4*4; i++) {
    key[i] = i;
  }
  cipher_and_inv_cipher((uint32_t*)key, (uint32_t*)in);
  
  printf("AES-192\n");
  aes_type = AES192;
  memset(key, 0, sizeof(key));
  for (i=0; i<6*4; i++) {
    key[i] = i;
  }
  cipher_and_inv_cipher((uint32_t*)key, (uint32_t*)in);
  
  printf("AES-256\n");
  aes_type = AES256;
  memset(key, 0, sizeof(key));
  for (i=0; i<8*4; i++) {
    key[i] = i;
  }
  cipher_and_inv_cipher((uint32_t*)key, (uint32_t*)in);
  
  return 0;
}
