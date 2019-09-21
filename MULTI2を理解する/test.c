#include <stdio.h>
#include <stdint.h>
#include <string.h>

#define ROUND (4) // (round number) = ROUND * 8

static uint32_t _s[8]; // system key     256bit = 8 * 32bit
static uint32_t _d[2]; // data key        64bit = 2 * 32bit
static uint32_t _k[8]; // encryption key 256bit = 8 * 32bit

static void multi2_set_system_key(const uint8_t* k /*[32]*/); // 256bit
static void multi2_set_data_key(const uint8_t* d /*[8]*/);    // 64bit
static void multi2_encrypt(uint8_t* text /*[8]*/); // 64bit
static void multi2_decrypt(uint8_t* text /*[8]*/); // 64bit

static void key_schedule();
static void pi1(const uint32_t* L, uint32_t* R);
static void pi2(uint32_t* L, const uint32_t* R, uint32_t p);
static void pi3(const uint32_t* L, uint32_t* R, uint32_t p1, uint32_t p2);
static void pi4(uint32_t* L, const uint32_t* R, uint32_t p);
static uint32_t left_rot(uint32_t val, uint8_t count);
static void bswap(uint8_t* val);

static void print(const char* header, const char* footer, uint8_t* buf, int size)
{
  int i;
  printf("%s", header);
  for (i=0; i<size; i++) {
    printf("%02X", buf[i]);
  }
  printf("%s", footer);
}

int main(int argc, char* argv[])
{
  int i;
  uint8_t k[32];
  uint8_t d[8];
  uint8_t t[8];
  
  for (i=0; i<32; i++) {
    k[i] = i;
  }
  for (i=0; i<8; i++) {
    d[i] = i+0x20;
    t[i] = i+0x30;
  }
  
  print("system key     :0x", "\n", k, 32);
  print("data key       :0x", "\n", d,  8);
  print("plain text     :0x", "\n", t,  8);
  
  multi2_set_system_key(k /*[32]*/);     // 256bit  
  multi2_set_data_key(d /*[8]*/);        // 64bit  
  
  multi2_encrypt(t /*[8]*/); // 64bit  
  print("encrypted text :0x", "\n", t,  8);
  
  multi2_decrypt(t /*[8]*/); // 64bit  
  print("decrypted text :0x", "\n", t,  8);
  
  return 0;
}


void multi2_set_system_key(const uint8_t* s /*[32]*/)
{
  int i;
  memcpy(_s, s, 32);
  for (i=0; i<8; i++) {
    bswap((uint8_t*)&_s[i]);
  }
}

void multi2_set_data_key(const uint8_t* d /*[8]*/)
{
  int i;
  memcpy(_d, d, 8);
  for (i=0; i<2; i++) {
    bswap((uint8_t*)&_d[i]);
  }
  key_schedule();
}

void multi2_encrypt(uint8_t* text /*[8]*/)
{
  int i = 0;
  uint32_t* L = (uint32_t*)&text[0];
  uint32_t* R = (uint32_t*)&text[4];
  
  bswap(&text[0]);
  bswap(&text[4]);
  
  for (i=0; i<ROUND; i++) {
    pi1(L, R);
    pi2(L, R, _k[0]);
    pi3(L, R, _k[1], _k[2]);
    pi4(L, R, _k[3]);
    pi1(L, R);
    pi2(L, R, _k[4]);
    pi3(L, R, _k[5], _k[6]);
    pi4(L, R, _k[7]);
  }
  
  bswap(&text[0]);
  bswap(&text[4]);
}

void multi2_decrypt(uint8_t* text /*[8]*/)
{
  int i = 0;
  uint32_t* L = (uint32_t*)&text[0];
  uint32_t* R = (uint32_t*)&text[4];
  
  bswap(&text[0]);
  bswap(&text[4]);
  
  for (i=0; i<ROUND; i++) {
    pi4(L, R, _k[7]);
    pi3(L, R, _k[5], _k[6]);
    pi2(L, R, _k[4]);
    pi1(L, R);
    pi4(L, R, _k[3]);
    pi3(L, R, _k[1], _k[2]);
    pi2(L, R, _k[0]);
    pi1(L, R);
  }
  
  bswap(&text[0]);
  bswap(&text[4]);
}

/* static function */
static void key_schedule()
{
  uint32_t d[2];
  uint32_t *L = d, *R = d+1;
  memcpy(d, _d, 8);
  
  /* (d, s) => (k) */
  pi1(L, R);
  pi2(L, R, _s[0]);
  _k[0] = L[0];
  pi3(L, R, _s[1], _s[2]);
  _k[1] = R[0];
  pi4(L, R, _s[3]);
  _k[2] = L[0];
  pi1(L, R);
  _k[3] = R[0];
  pi2(L, R, _s[4]);
  _k[4] = L[0];
  pi3(L, R, _s[5], _s[6]);
  _k[5] = R[0];
  pi4(L, R, _s[7]);
  _k[6] = L[0];
  pi1(L, R);
  _k[7] = R[0];
}

static void pi1(const uint32_t* L, uint32_t* R)
{
  R[0] ^= L[0];
}

static void pi2(uint32_t* L, const uint32_t* R, uint32_t p)
{
  uint32_t x, y;
  
  /* x = ((R + p) <<< 1) + R + p - 1 */
  y = R[0] + p;
  x = left_rot(y, 1) + y - 1;
  /* L ^= (x <<< 4) ^ x */
  L[0] ^= left_rot(x, 4) ^ x;
}

static void pi3(const uint32_t* L, uint32_t* R, uint32_t p1, uint32_t p2)
{
  uint32_t x, y, z, w;
  
  z = L[0] + p1;
  y = left_rot(z, 2) + z + 1;
  w = (left_rot(y, 8) ^ y) + p2;
  x = left_rot(w, 1) - w;
  R[0] ^= left_rot(x, 16) ^ (x | L[0]);
}

static void pi4(uint32_t* L, const uint32_t* R, uint32_t p)
{
  uint32_t x;
  
  x = R[0] + p;
  L[0] ^= left_rot(x, 2) + x + 1;
}

static uint32_t left_rot(uint32_t val, uint8_t count)
{
  return val<<count | val>>(32-count);
}

static void bswap(uint8_t* val)
{
  uint8_t tmp;
  
  tmp = val[0];
  val[0] = val[3]; val[3] = tmp;
  tmp = val[1];
  val[1] = val[2]; val[2] = tmp;
}
