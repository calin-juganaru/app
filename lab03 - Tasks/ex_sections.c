#include <omp.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define N 1000000
double a[N], b[N], c[N], d[N];

int main(int argc, char *argv[])
{
    double start = omp_get_wtime();

    #pragma omp parallel
    {
        #pragma omp sections
        {
            #pragma omp section
            {
                for (int i = 0; i < N; ++i)
                    a[i] = 1.5 * i;
                printf("Done with a[], thread %d \n", omp_get_thread_num());
            }

            #pragma omp section
            {
                for (int i = 0; i < N; ++i)
                    b[i] = 22.35 + i;
                printf("Done with b[], thread %d \n", omp_get_thread_num());
            }

            #pragma omp section
            {
                for (int i = 0; i < N; ++i)
                    c[i] = 0.0;
                printf("Done with c[], thread %d \n", omp_get_thread_num());
            }

            #pragma omp section
            {
                for (int i = 0; i < N; ++i)
                    d[i] = 1.0;
                printf("Done with d[], thread %d \n", omp_get_thread_num());
            }
        }

        // --------------------------------------------------------------------

        #pragma omp single
        {
            for (int i = 0; i < N; ++i)
                if (a[i] != 1.5 * i)
                    { printf("in"); break; }
            printf("correct a[], thread %d \n", omp_get_thread_num());
        }

        #pragma omp single
        {
            for (int i = 0; i < N; ++i)
                if (b[i] != i + 22.35)
                    { printf("in"); break; }
            printf("correct b[], thread %d \n", omp_get_thread_num());
        }

        #pragma omp single
        {
            for (int i = 0; i < N; ++i)
                if (c[i] != 0)
                    { printf("in"); break; }
            printf("correct c[], thread %d \n", omp_get_thread_num());
        }

        #pragma omp single
        {
            for (int i = 0; i < N; ++i)
                if (d[i] != 1)
                    { printf("in"); break; }
            printf("correct d[], thread %d \n", omp_get_thread_num());
        }

        // --------------------------------------------------------------------

        #pragma omp sections
        {
            #pragma omp section
            {
                for (int i = 0; i < N; ++i)
                    c[i] = sin(a[i] + b[i]);
                printf("Done with sin, thread %d \n", omp_get_thread_num());
            }

            #pragma omp section
            {
                for (int i = 0; i < N; ++i)
                    d[i] *= sqrt(a[i]);
                printf("Done with sqrt a, thread %d \n", omp_get_thread_num());
            }

            #pragma omp section
            {
                for (int i = 0; i < N; ++i)
                    d[i] *= sqrt(b[i]);
                printf("Done with sqrt b, thread %d \n", omp_get_thread_num());
            }
        }

        // --------------------------------------------------------------------

        #pragma omp single
        {
            for (int i = 0; i < N; ++i)
                if (fabs(d[i] - sqrt(a[i] * b[i])) > 0.001)
                    { printf("in"); break; }
            printf("correct d[], thread %d \n", omp_get_thread_num());
        }
    }

    double stop = omp_get_wtime();
    printf("Duration  = %g\n", stop - start);

    return 0;
}
