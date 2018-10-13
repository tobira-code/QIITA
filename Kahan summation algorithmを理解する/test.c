#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#define FLOAT_TYPE  double

void usage()
{
  printf("usage:./a.out [loop] [k] <q>\n");
  printf(" e:enable kahan !e:disable kahan\n");
}

void kahan_sum(FLOAT_TYPE* d, int size, FLOAT_TYPE* sum, FLOAT_TYPE* c)
{
  int i=0;
  for (i=0; i<size; i++) {
    FLOAT_TYPE tmp_sum;
    if (fabs(d[i]) <= fabs(*sum)) {
        tmp_sum = *sum + (d[i] + *c);
        *c = (d[i] + *c) - (tmp_sum - *sum);
    } else {
        tmp_sum = (*sum + *c) + d[i];
        *c = (*sum + *c) - (tmp_sum - d[i]);
    }
    *sum = tmp_sum;
  }
}

int main(int argc, char* argv[])
{
  int i, loop, q, ek = 0; /* ek:enalbe kahan */
  FLOAT_TYPE max = 1.0;
  FLOAT_TYPE dx, x = 0;
  FLOAT_TYPE e = 0; /* error */
  FLOAT_TYPE  k  = 0; /* kahan c */

  if (!(argc == 3 || argc == 4)) {
    usage();
    return -1;
  }

  loop = atoi(argv[1]);
  dx = max / loop;
  if (argc == 4 && argv[3][0] == 'q') {
    q = 1;
    if (argv[2][0] == 'e') {
        ek = 1; /* enable kahan */
    } else {
        ek = 0; /* disable */
    }
  } else if (argc == 3) {
    q = 0;
    if (argv[2][0] == 'e') {
        ek = 1; /* enable kahan */
    } else {
        ek = 0; /* disable */
    }
  } else {
    usage();
    return -1;
  }

  for (i=0; i<loop; i++) {
    FLOAT_TYPE c, d; /* c:correct, d:diff */
    if (ek == 1) {
        kahan_sum(&dx, 1, &x, &k);
    } else {
        x += dx;
    }
    c = dx*(i+1);
    d = c-x;
    e = fmax(e, d);
    if (!q) {
      printf("%+.15e\t%+.15e\t%+.15e\t%+.15e\n", x, c, d, k);
    }
  }
  printf("dx\t%+.15e\te\t%+.15e\tloop\t%d\n", dx, e, loop);

  return 0;
}
