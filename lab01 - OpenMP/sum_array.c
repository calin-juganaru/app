#include <stdio.h>
#include <stdlib.h>
#include <omp.h>

const int SIZE = 1000000;

int main(void)
{
    int a[SIZE], i;
    long sum = 0;

    #pragma omp parallel private(i) shared(a)
    {
        #pragma omp for
        for (i = 0; i < SIZE; ++i)
            a[i] = i;
    }

    // ------------------------------------------------------------------------

    #pragma omp parallel for private(i) shared(a, sum)
        for (i = 0; i < SIZE; i++)
        {
            #pragma omp critical
            sum += a[i];
        }

    printf("Total critical sum = %ld\n", sum);

    // ------------------------------------------------------------------------

    sum = 0;
    #pragma omp parallel for reduction (+ : sum) private(i) shared(a)
        for (i = 0; i < SIZE; i++) sum += a[i];

    printf("Total reduced  sum = %ld\n", sum);

    return 0;
}