#include <stdio.h>
#include <math.h>

int main(int argc, char* argv[])
{
  printf("casting\n");
  printf("  (int) 1.0 = %+d\n", (int) 1.0);
  printf("  (int) 0.9 = %+d\n", (int) 0.9);
  printf("  (int) 0.5 = %+d\n", (int) 0.5);
  printf("  (int) 0.1 = %+d\n", (int) 0.1);
  printf("  (int) 0.0 = %+d\n", (int) 0.0);
  printf("  (int)-0.1 = %+d\n", (int)-0.1);
  printf("  (int)-0.5 = %+d\n", (int)-0.5);
  printf("  (int)-0.9 = %+d\n", (int)-0.9);
  printf("  (int)-1.0 = %+d\n", (int)-1.0);
  
  printf("division\n");
  printf("   10/10    = %+d\n",  10/10);
  printf("    9/10    = %+d\n",   9/10);
  printf("    5/10    = %+d\n",   5/10);
  printf("    1/10    = %+d\n",   1/10);
  printf("    0/10    = %+d\n",   0/10);
  printf("   -1/10    = %+d\n",  -1/10);
  printf("   -5/10    = %+d\n",  -5/10);
  printf("   -9/10    = %+d\n",  -9/10);
  printf("  -10/10    = %+d\n", -10/10);
  
  printf("ceiling\n");
  printf("  ceil( 1.0) = %+lf\n", ceil( 1.0));
  printf("  ceil( 0.9) = %+lf\n", ceil( 0.9));
  printf("  ceil( 0.5) = %+lf\n", ceil( 0.5));
  printf("  ceil( 0.1) = %+lf\n", ceil( 0.1));
  printf("  ceil( 0.0) = %+lf\n", ceil( 0.0));
  printf("  ceil(-0.1) = %+lf\n", ceil(-0.1));
  printf("  ceil(-0.5) = %+lf\n", ceil(-0.5));
  printf("  ceil(-0.9) = %+lf\n", ceil(-0.9));
  printf("  ceil(-1.0) = %+lf\n", ceil(-1.0));
  
  printf("flooring\n");
  printf("  floor( 1.0) = %+lf\n", floor( 1.0));
  printf("  floor( 0.9) = %+lf\n", floor( 0.9));
  printf("  floor( 0.5) = %+lf\n", floor( 0.5));
  printf("  floor( 0.1) = %+lf\n", floor( 0.1));
  printf("  floor( 0.0) = %+lf\n", floor( 0.0));
  printf("  floor(-0.1) = %+lf\n", floor(-0.1));
  printf("  floor(-0.5) = %+lf\n", floor(-0.5));
  printf("  floor(-0.9) = %+lf\n", floor(-0.9));
  printf("  floor(-1.0) = %+lf\n", floor(-1.0));
  
  printf("rounding(Round half away from zero)\n");
  printf("  round( 1.0) = %+lf\n", round( 1.0));
  printf("  round( 0.9) = %+lf\n", round( 0.9));
  printf("  round( 0.5) = %+lf\n", round( 0.5));
  printf("  round( 0.1) = %+lf\n", round( 0.1));
  printf("  round( 0.0) = %+lf\n", round( 0.0));
  printf("  round(-0.1) = %+lf\n", round(-0.1));
  printf("  round(-0.5) = %+lf\n", round(-0.5));
  printf("  round(-0.9) = %+lf\n", round(-0.9));
  printf("  round(-1.0) = %+lf\n", round(-1.0));
  
  return 0;
}
