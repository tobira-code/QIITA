#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

static inline uint64_t RDTSCP();
static void print_tsc();

int main(int argc, char* argv[])
{
  while(1) {
    char buf[4];
    print_tsc();
    printf("press ENTER key.");
    fflush(stdout);
    fgets(buf, 4, stdin);
  }

  return 0;
}

static inline uint64_t RDTSCP()
{
  uint32_t hi, lo, aux;
  __asm__ volatile("rdtscp" : "=a" (lo), "=d" (hi), "=c" (aux));
  return ((uint64_t)hi << 32) | lo;
}

static void print_tsc()
{
  #define MAXU64 (0xFFFFFFFFFFFFFFFF)
  static uint64_t prev = MAXU64;
  uint64_t tsc, diff = 0;

  tsc = RDTSCP();
  if (prev != MAXU64) {
    diff = tsc - prev;
    printf("tsc=%" PRIu64 " diff=%" PRIu64 "\n", tsc, diff);
  } else {
    printf("tsc=%" PRIu64 "\n", tsc);
  }
  prev = tsc;
}
