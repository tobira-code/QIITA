#include <math.h>
#include <stdio.h>
#include <string.h>

const double C4[] = {
  7.07106781186548E-01,
};

const double C8[] = {
  5.41196100146197E-01, // 1/(2*cos(pi/8*1))
  1.30656296487638E+00, // 1/(2*cos(pi/8*3))
};

const double C16[] = {
  5.09795579104159E-01, // 1/(2*cos(pi/16*1))
  6.01344886935045E-01, // 1/(2*cos(pi/16*3))
  8.99976223136416E-01, // 1/(2*cos(pi/16*5))
  2.56291544774151E+00, // 1/(2*cos(pi/16*7))
};

const double C32[] = {
  5.02419286188156E-01, // 1/(2*cos(pi/32*1))
  5.22498614939689E-01, // 1/(2*cos(pi/32*3))
  5.66944034816358E-01, // 1/(2*cos(pi/32*5))
  6.46821783359990E-01, // 1/(2*cos(pi/32*7))
  7.88154623451250E-01, // 1/(2*cos(pi/32*9))
  1.06067768599035E+00, // 1/(2*cos(pi/32*11))
  1.72244709823833E+00, // 1/(2*cos(pi/32*13))
  5.10114861868916E+00, // 1/(2*cos(pi/32*15))
};

static void idctII2(double* input, double* output)
{
  double* x = output;
  double* X = input;
  double tmp = C4[0] * X[1];
  x[0] = X[0] + tmp;
  x[1] = X[0] - tmp;
}

static void idctII4(double* input, double* output)
{
  double* x = output;
  static double in[2], g[2], h[2], tmp[2];
  
  in[0] = input[0];
  in[1] = input[2];
  idctII2(in, g);
  
  in[0] = input[1];
  in[1] = input[3] + input[1];
  idctII2(in, h);
  
  tmp[0] = C8[0] * h[0];
  tmp[1] = C8[1] * h[1];
  x[0] = g[0] + tmp[0];
  x[1] = g[1] + tmp[1];
  x[2] = g[1] - tmp[1];
  x[3] = g[0] - tmp[0];
}

static void idctII8(double* input, double* output)
{
  double* x = output;
  static double in[4], g[4], h[4], tmp[4];
  
  in[0] = input[0];
  in[1] = input[2];
  in[2] = input[4];
  in[3] = input[6];
  idctII4(in, g);
  
  in[0] = input[1];
  in[1] = input[3] + input[1];
  in[2] = input[5] + input[3];
  in[3] = input[7] + input[5];
  idctII4(in, h);
  
  tmp[0] = C16[0] * h[0];
  tmp[1] = C16[1] * h[1];
  tmp[2] = C16[2] * h[2];
  tmp[3] = C16[3] * h[3];
  x[0] = g[0] + tmp[0];
  x[1] = g[1] + tmp[1];
  x[2] = g[2] + tmp[2];
  x[3] = g[3] + tmp[3];
  x[4] = g[3] - tmp[3];
  x[5] = g[2] - tmp[2];
  x[6] = g[1] - tmp[1];
  x[7] = g[0] - tmp[0];
}

static void fdctII2(double* input, double* output)
{
  double* X = output;
  double* x = input;
  
  X[0] =  x[0] + x[1];
  X[1] = (x[0] - x[1]) * C4[0];
}

static void fdctII4(double* input, double* output)
{
  double* X = output;
  static double in[2], G[2], H[2];
  
  in[0] = input[0] + input[3];
  in[1] = input[1] + input[2];
  fdctII2(in, G);
  
  in[0] = C8[0] * (input[0] - input[3]);
  in[1] = C8[1] * (input[1] - input[2]);
  fdctII2(in, H);
  
  X[0] = G[0];
  X[1] = H[0] + H[1];
  X[2] = G[1];
  X[3] = H[1];
}

static void fdctII8(double* input, double* output)
{
  double* X = output;
  static double in[4], G[4], H[4];
  
  in[0] = input[0] + input[7];
  in[1] = input[1] + input[6];
  in[2] = input[2] + input[5];
  in[3] = input[3] + input[4];
  fdctII4(in, G);
  
  in[0] = C16[0] * (input[0] - input[7]);
  in[1] = C16[1] * (input[1] - input[6]);
  in[2] = C16[2] * (input[2] - input[5]);
  in[3] = C16[3] * (input[3] - input[4]);
  fdctII4(in, H);
  
  X[0] = G[0];
  X[1] = H[0] + H[1];
  X[2] = G[1];
  X[3] = H[1] + H[2];
  X[4] = G[2];
  X[5] = H[2] + H[3];
  X[6] = G[3];
  X[7] = H[3];
}

static void dctIV8(double* input, double* output)
{
  int i;
  static double in[8];
  
  in[0] = input[0];
  for (i=1; i<8; i++) {
    in[i] = input[i-1] + input[i];
  }
  
  idctII8(in, output);
  for (i=0; i<8; i++) {
    output[i] *= C32[i];
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

int main(int argc, char* argv[])
{
  double input[] = {0.0, 1.0, 2.0, 3.0, 4.0, 5.0, 6.0, 7.0};
  double output[8];
  
  print_data("input\n", input, 8);
  
  memset(output, 0, sizeof(output));
  fdctII8(input, output);
  print_data("fdctII8\n", output, 8);
  
  memset(output, 0, sizeof(output));
  idctII8(input, output);
  print_data("idctII8\n", output, 8);
  
  memset(output, 0, sizeof(output));
  dctIV8(input, output);
  print_data("dctIV8\n", output, 8);
  
  return 0;
}

