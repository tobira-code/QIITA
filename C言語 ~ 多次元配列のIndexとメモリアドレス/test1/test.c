#include <stdio.h>
#define LEN (2)
int array[LEN][LEN];

int main(int argc, char* argv[])
{
    int i,j;
    printf("sizeof(int) = %lu\n", sizeof(int));
    for (i=0; i<LEN; i++) {
        for (j=0; j<LEN; j++) {
            printf("i=%d j=%d, &array[i][j] = %p\n",
                i, j, &array[i][j]);
        }
    }
    printf("-----------------------\n");
    for (i=0; i<LEN; i++) {
        for (j=0; j<LEN; j++) {
            printf("i=%d j=%d, &array[j][i] = %p\n",
                i, j, &array[j][i]);
        }
    }

    return 0;
}
