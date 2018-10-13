#include <stdio.h>
#include <stdint.h>

#define LEN  (1024*8)
int array[LEN][LEN];

inline uint64_t RDTSCP()
{
    uint32_t hi, lo, aux=0;
    __asm__ volatile("rdtscp" : "=a" (lo), "=d" (hi), "=c" (aux));
    return ((uint64_t)hi << 32) | lo;
}

int main(int argc, char* argv[])
{
    int i,j,value;
    uint64_t start, end;

    start = RDTSCP();
    for (i=0; i<LEN; i++) {
        for (j=0; j<LEN; j++) {
            value += array[i][j];
        }
    }
    end = RDTSCP();
    printf("%lu\n", end-start);
    start = RDTSCP();
    for (i=0; i<LEN; i++) {
        for (j=0; j<LEN; j++) {
            value += array[j][i];
        }
    }
    end = RDTSCP();
    printf("%lu\n", end-start);

    return 0;
}
