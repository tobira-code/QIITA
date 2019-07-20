#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint32_t crc32_table[256];

void make_crc32_table(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i << 24;
        for (int j = 0; j < 8; j++) {
            c = (c << 1) ^ ((c & 0x80000000) ? 0x04c11db7 : 0);
        }
        crc32_table[i] = c;
    }
}

uint32_t crc32(uint8_t *buf, size_t len) {
    uint32_t c = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        c = (c << 8) ^ crc32_table[((c >> 24) ^ buf[i]) & 0xFF];
    }
    return c;
}

int main()
{
  char* buf = "123456789";
  int sz = strlen(buf);
  uint32_t _crc32;

  printf("ascii=%s\n", buf);
  make_crc32_table();
  _crc32 = crc32((uint8_t*)buf, sz);
  printf("crc32=%08x\n", _crc32);

  return 0;
}
