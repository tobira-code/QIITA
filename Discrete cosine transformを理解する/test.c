#include <math.h>
#include <stdio.h>
#include <string.h>

static void fdctII(const double *input, double *output, int N);
static void idctII(const double *input, double *output, int N);
static void dctIV(const double *input, double *output, int N);
static void print_data(const char* str, const double *data, int N);

int main(int argc, char* argv[])
{
  double input[] = {1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0, 8.0};
  double output1[8], output2[8];

  print_data("input\n", input, 8);

  memset(output1, 0, sizeof(output1));
  fdctII(input,   output1, 8);
  print_data("fdctII\n", output1, 8);
  memset(output2, 0, sizeof(output2));
  idctII(output1, output2, 8);
  print_data("idctII\n", output2, 8);

  memset(output1, 0, sizeof(output1));
  dctIV(input,   output1, 8);
  print_data("fdctIV\n", output1, 8);
  memset(output2, 0, sizeof(output2));
  dctIV(output1, output2, 8);
  print_data("idctIV\n", output2, 8);

  return 0;
}

static void fdctII(const double *input, double *output, int N)
{
  int n, k;
  double c1 = sqrt(2.0/(double)N);
  double c2 = 1.0 / sqrt(2.0);

  for (k=0; k<N; k++) {
    output[k] = 0.0;
    for (n=0; n<N; n++) {
      output[k] += input[n] * cos(M_PI/2.0/(double)N*(2.0*n+1.0)*k);
    }
    output[k] *= c1;
    if (k == 0) {
      output[k] *= c2;
    }
  }
}

static void idctII(const double *input, double *output, int N)
{
  int n, k;
  double c1 = sqrt(2.0/(double)N);
  double c2 = 1.0 / sqrt(2.0);

  for (n=0; n<N; n++) {
    output[n] = 0.0;
    for (k=0; k<N; k++) {
      if (k == 0) {
        output[n] += c2 * input[k] * cos(M_PI/2.0/(double)N*(2.0*(double)n+1.0)*(double)k);
      } else {
        output[n] +=      input[k] * cos(M_PI/2.0/(double)N*(2.0*(double)n+1.0)*(double)k);
      }
    }
    output[n] *= c1;
  }
}

static void dctIV(const double *input, double *output, int N)
{
  int n, k;
  double c1 = sqrt(2.0/(double)N);

  for (n=0; n<N; n++) {
    output[n] = 0.0;
    for (k=0; k<N; k++) {
      output[n] += input[k] * cos(M_PI/4.0/(double)N*(2.0*(double)n+1.0)*(2.0*(double)k+1.0));
    }
    output[n] *= c1;
  }
}

static void print_data(const char* str, const double *data, int N)
{
  int i;
  printf("%s", str);
  for (i=0; i<N; i++) {
    printf("%+1.3e ", data[i]);
  }
  printf("\n");
}
