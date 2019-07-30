#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CPU_FREQ (2200000000.0)   // 2.2GHz

inline uint64_t RDTSCP()
{
  uint32_t hi, lo, aux=0;
  __asm__ volatile("rdtscp" : "=a" (lo), "=d" (hi), "=c" (aux));
  return ((uint64_t)hi << 32) | lo;
}

uint64_t _rdtscp_start, _rdtscp_end;
inline void tic()
{
  _rdtscp_start = RDTSCP();
}

inline uint64_t toc()
{
  _rdtscp_end = RDTSCP();
  return _rdtscp_end - _rdtscp_start;
}

extern void _stosq(uint64_t size, uint64_t* buf, uint64_t value);
extern void _movsq(uint64_t* dst, uint64_t* src, uint64_t size);

void print_rate(char* header, uint64_t* buf, size_t size, uint64_t clk_cnt)
{
  double MBps = (double)size / 1048576.0 * CPU_FREQ / (double)clk_cnt;
  printf("%s%12" PRIu64 " byte, %12" PRIu64 " clk, %8.1f MiB/s\n", header, size, clk_cnt, MBps);
}

void usage()
{
  printf("usage:\n");
  printf(" test.exe [size_in_byte] [loop]\n");
}

int main(int argc, char* argv[])
{
  int loop, i;
  size_t size = 0;
  uint64_t* buf = 0, * buf2 = 0;
  uint64_t clk_cnt = 0;
  
  if (argc != 3) {
    usage();
    return -1;
  }
  
  size = (size_t)atoi(argv[1]);
  loop  = atoi(argv[2]);
  
  buf  = malloc(size);
  buf2 = malloc(size);
  if (buf == NULL || buf2 == NULL) {
    printf("error malloc: \n");
    usage();
    return -1;
  }
  
  // _stosq (assembly code)
  for (i=0; i<loop; i++) {
    tic();
    _stosq(size/8, buf, i);
    clk_cnt = toc();
    print_rate("_stosq ", buf, size, clk_cnt);
  }
  
  // _movsq (assembly code)
  for (i=0; i<loop; i++) {
    tic();
    _movsq(buf, buf2, size/8);
    clk_cnt = toc();
    print_rate("_movsq ", buf, size, clk_cnt);
  }
  
  // memset
  for (i=0; i<loop; i++) {
    tic();
    memset(buf, 0, size);
    clk_cnt = toc();
    print_rate("memset ", buf, size, clk_cnt);
  }
  
  // memcpy
  for (i=0; i<loop; i++) {
    tic();
    memcpy(buf, buf2, size);
    clk_cnt = toc();
    print_rate("memcpy ", buf, size, clk_cnt);
  }
  
  free(buf);
  free(buf2);
  
  return 0;
}
