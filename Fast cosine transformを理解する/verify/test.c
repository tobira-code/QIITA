#include <stdio.h>
#include <fftw3.h>

int main(int argc, char* argv[])
{
  int i, n=8;
  fftw_plan p;
  static double in[8], out[8];
  
  for (i=0; i<n; i++) {
    in[i] = i;
  }
  printf("input\n");
  for (i=0; i<n; i++) {
    printf("%+1.3e ", in[i]);
  }
  printf("\n");
  
  n = 8;
  p = fftw_plan_r2r_1d(n/*int n*/, in, out,
    FFTW_REDFT10 /* fftw_r2r_kind */, FFTW_ESTIMATE /* flags */);
  fftw_execute(p);
  printf("fdctII%d\n", n);
  for (i=0; i<n; i++) {
    printf("%+1.3e ", out[i]/2.0);
  }
  printf("\n");
  
  n = 8;
  p = fftw_plan_r2r_1d(n/*int n*/, in, out,
    FFTW_REDFT01 /* fftw_r2r_kind */, FFTW_ESTIMATE /* flags */);
  fftw_execute(p);
  printf("idctII%d\n", n);
  for (i=0; i<n; i++) {
    printf("%+1.3e ", out[i]/2.0);
  }
  printf("\n");
  
  n = 8;
  p = fftw_plan_r2r_1d(n/*int n*/, in, out,
    FFTW_REDFT11 /* fftw_r2r_kind */, FFTW_ESTIMATE /* flags */);
  fftw_execute(p);
  printf("dctIV%d\n", n);
  for (i=0; i<n; i++) {
    printf("%+1.3e ", out[i]/2.0);
  }
  printf("\n");
  
  return 0;
}
