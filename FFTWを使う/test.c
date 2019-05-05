#include <stdio.h>
#include <inttypes.h>
#include <fftw3.h>

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

inline void toc(const char* str)
{
  _rdtscp_end = RDTSCP();
  printf("profile:%s %" PRIu64 " clock\n", str, _rdtscp_end-_rdtscp_start);
}

int main(int argc, char* argv[])
{
  int i, w=32, len=1024;
  fftw_plan p;
  static double in[1024], out[1024];
  
  for (i=0; i<1024; i++) {
    in[i] = i;
  }
  printf("input\n");
  for (i=0; i<len; i++) {
    printf("%+05.3e ", in[i]);
    if ((i+1)%32 == 0) {
      printf("\n");
    }
  }
  
  p = fftw_plan_r2r_1d(len/*int n*/, in, out,
    FFTW_REDFT11 /* fftw_r2r_kind */, FFTW_ESTIMATE /* flags */);
  tic();
  fftw_execute(p);
  toc("");
  
  printf("output forward\n");
  for (i=0; i<len; i++) {
    printf("%+05.3e ", out[i]);
    if ((i+1)%w == 0) {
      printf("\n");
    }
  }
  
  p = fftw_plan_r2r_1d(len/*int n*/, out, in,
    FFTW_REDFT11 /* fftw_r2r_kind */, FFTW_ESTIMATE /* flags */);
  tic();
  fftw_execute(p);
  toc("");
  
  printf("output inverse\n");
  for (i=0; i<len; i++) {
    printf("%+05.3e ", in[i]/2048.0);
    if ((i+1)%w == 0) {
      printf("\n");
    }
  }
  
  return 0;
}
