#include "stdlib.h"
#include <unistd.h>
#include <cblas.h>
#include "defs.h"

void MY_MMult(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
  int i, j, p;

  for (i = 0; i < m; i++) /* Loop over the rows of C */
  {
    for (j = 0; j < n; j++) /* Loop over the columns of C */
    {
      for (p = 0; p < k; p++)
      { /* Update C( i,j ) with the inner product of the ith row of A and the jth column of B */
        C(i, j) = C(i, j) + A(i, p) * B(p, j);
      }
    }
  }
}

void random_matrix(int m, int n, double *a, int lda)
{
  /* drand48() generate pseudo-random numbers using the linear congruential algorithm and
     48-bit integer arithmetic. return nonnegative double-precision floating-point values
     uniformly distributed over the interval [0.0, 1.0). */
  double drand48();
  int i, j;

  for (j = 0; j < n; j++)
    for (i = 0; i < m; i++)
      // A(i, j) = drand48();
      A(i, j) = 2.0 * drand48() - 1.0;
}

int main(int argc, char *argv[]) {
  int m, n, k, lda, ldb, ldc;
  m=n=k=lda=ldb=ldc = 1024;

  double
    *a,
    *b, *c;
  
  a = (double *)malloc(lda * (k + 1) * sizeof(double));
  b = (double *)malloc(ldb * n * sizeof(double));
  c = (double *)malloc(ldc * n * sizeof(double));
  
  /* Generate random matrices A, B, Cold */
  random_matrix(m, k, a, lda);
  random_matrix(k, n, b, ldb);
  random_matrix(m, n, c, ldc);
  
  if (fork() == 0) {
    MY_MMult(m, n, k, a, lda, b, ldb, c, ldc);
  } else {
    MY_MMult(m, n, k, a, lda, b, ldb, c, ldc);
  }

  free(a);
  free(b);
  free(c);

  exit(0);
}
