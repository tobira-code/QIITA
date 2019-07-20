#include <stdio.h>
#include <string.h>
#include <stdint.h>

uint16_t crc16_table[256];
uint32_t crc32_table[256];

void make_crc16_table(void) {
    for (uint16_t i = 0; i < 256; i++) {
        uint16_t c = i;
        for (int j = 0; j < 8; j++) {
            c = (c & 1) ? (0xA001 ^ (c >> 1)) : (c >> 1);
        }
        crc16_table[i] = c;
    }
}

uint16_t crc16(uint8_t *buf, size_t len) {
    uint16_t c = 0;
    for (size_t i = 0; i < len; i++) {
        c = crc16_table[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    }
    return c;
}

void make_crc32_table(void) {
    for (uint32_t i = 0; i < 256; i++) {
        uint32_t c = i;
        for (int j = 0; j < 8; j++) {
            c = (c & 1) ? (0xEDB88320 ^ (c >> 1)) : (c >> 1);
        }
        crc32_table[i] = c;
    }
}

uint32_t crc32(uint8_t *buf, size_t len) {
    uint32_t c = 0xFFFFFFFF;
    for (size_t i = 0; i < len; i++) {
        c = crc32_table[(c ^ buf[i]) & 0xFF] ^ (c >> 8);
    }
    return c ^ 0xFFFFFFFF;
}

int main()
{
  char* buf = "123456789";
  int sz = strlen(buf);
  uint16_t _crc16;
  uint32_t _crc32;

  printf("ascii=%s\n", buf);
  make_crc16_table();
  _crc16 = crc16((uint8_t*)buf, sz);
  printf("crc16=%04x\n", _crc16);

  make_crc32_table();
  _crc32 = crc32((uint8_t*)buf, sz);
  printf("crc32=%08x\n", _crc32);

  return 0;
}
