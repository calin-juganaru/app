#include <omp.h>
#include <stdio.h>
#include <stdlib.h>

#define NRA 62                 /* number of rows in matrix A */
#define NCA 15                 /* number of columns in matrix A */
#define NCB 7                  /* number of columns in matrix B */

int main(int argc, char *argv[])
{
    int	tid, i, j, k;
    double	a[NRA][NCA],           /* matrix A to be multiplied */
            b[NCA][NCB],           /* matrix B to be multiplied */
            c[NRA][NCB];           /* result matrix C */

    int nthreads;// = 4;
    int chunk = 10; // dimensiunea chunk-ului la schedule

    double begin = omp_get_wtime();

    // ------------------------------------------------------------------------

    #pragma omp parallel shared(a, b, c, nthreads, chunk) private(tid, i, j, k)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        #pragma omp for schedule (static, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j< NCA; j++)
                a[i][j] = i + j;

        #pragma omp for schedule (static, chunk)
        for (i = 0; i < NCA; i++)
            for (j = 0; j < NCB; j++)
                b[i][j]= i * j;

        #pragma omp for schedule (static, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                c[i][j] = 0;


        #pragma omp for schedule (static, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                for (k = 0; k < NCA; k++)
                    c[i][j] += a[i][k] * b[k][j];
    }

    double end = omp_get_wtime();
    printf("Total execution time for schedule static  = %lfs\n", (end - begin));

    // ------------------------------------------------------------------------

    begin = omp_get_wtime();

    #pragma omp parallel shared(a, b, c, nthreads, chunk) private(tid, i, j, k)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        #pragma omp for schedule (dynamic, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j< NCA; j++)
                a[i][j] = i + j;

        #pragma omp for schedule (dynamic, chunk)
        for (i = 0; i < NCA; i++)
            for (j = 0; j < NCB; j++)
                b[i][j]= i * j;

        #pragma omp for schedule (dynamic, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                c[i][j] = 0;


        #pragma omp for schedule (dynamic, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                for (k = 0; k < NCA; k++)
                    c[i][j] += a[i][k] * b[k][j];
    }

    end = omp_get_wtime();
    printf("Total execution time for schedule dynamic = %lfs\n", (end - begin));

    // ------------------------------------------------------------------------

    begin = omp_get_wtime();

    #pragma omp parallel shared(a, b, c, nthreads, chunk) private(tid, i, j, k)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        #pragma omp for schedule (guided, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j< NCA; j++)
                a[i][j] = i + j;

        #pragma omp for schedule (guided, chunk)
        for (i = 0; i < NCA; i++)
            for (j = 0; j < NCB; j++)
                b[i][j]= i * j;

        #pragma omp for schedule (guided, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                c[i][j] = 0;


        #pragma omp for schedule (guided, chunk)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                for (k = 0; k < NCA; k++)
                    c[i][j] += a[i][k] * b[k][j];
    }

    end = omp_get_wtime();
    printf("Total execution time for schedule guided  = %lfs\n", (end - begin));

    // ------------------------------------------------------------------------

    begin = omp_get_wtime();

    #pragma omp parallel shared(a, b, c, nthreads) private(tid, i, j, k)
    {
        tid = omp_get_thread_num();
        nthreads = omp_get_num_threads();

        #pragma omp for schedule (auto)
        for (i = 0; i < NRA; i++)
            for (j = 0; j< NCA; j++)
                a[i][j] = i + j;

        #pragma omp for schedule (auto)
        for (i = 0; i < NCA; i++)
            for (j = 0; j < NCB; j++)
                b[i][j]= i * j;

        #pragma omp for schedule (auto)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                c[i][j] = 0;


        #pragma omp for schedule (auto)
        for (i = 0; i < NRA; i++)
            for (j = 0; j < NCB; j++)
                for (k = 0; k < NCA; k++)
                    c[i][j] += a[i][k] * b[k][j];
    }

    end = omp_get_wtime();
    printf("Total execution time for schedule auto    = %lfs\n", (end - begin));

    // ------------------------------------------------------------------------
}