#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <math.h>

#define NUM_OF_ARRAY (1024)

extern double sin(double x);
extern double fsin(double x);

static inline uint64_t RDTSCP()
{
  uint32_t hi, lo, aux;
  __asm__ volatile("rdtscp" : "=a" (lo), "=d" (hi), "=c" (aux));
  return ((uint64_t)hi << 32) | lo;
}

static double bufx[NUM_OF_ARRAY+1];
static double bufs[NUM_OF_ARRAY+1];
static double bufc[NUM_OF_ARRAY+1];

int main(int argc, char* argv[])
{
  uint64_t start, end, diffs, diffc;
  int count = NUM_OF_ARRAY, i;
  double xst=-1.0*M_PI, len = 2.0*M_PI;

  /* x */
  for (i=0; i<=count; i++) {
    double w = len * (double)i / count;
    bufx[i] = xst+w;
  }

  /* fsin */
  start = RDTSCP();
  for (i=0; i<=count; i++) {
    bufs[i]  = fsin(bufx[i]);
  }
  end = RDTSCP();
  diffs = end - start;

  /* sin(C) */
  start = RDTSCP();
  for (i=0; i<=count; i++) {
    bufc[i]  = sin(bufx[i]);
  }
  end = RDTSCP();
  diffc = end - start;

  /* print */
  printf("x\tfsin(x)\tsin(x)\n");
  for (i=0; i<=count; i++) {
    printf("%+.16e\t%+.16e\t%+.16e\n", bufx[i], bufs[i], bufc[i]);
  }

  printf("fsin tsc %" PRIu64 " sin tsc %" PRIu64 "\n", diffs/NUM_OF_ARRAY, diffc/NUM_OF_ARRAY);
  return 0;
}
