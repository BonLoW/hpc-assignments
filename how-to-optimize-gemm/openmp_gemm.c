#include "defs.h"
#include <omp.h>

void openmp_gemm(int m, int n, int k, double *a, int lda,
              double *b, int ldb,
              double *c, int ldc)
{
  int i, j, p;

  #pragma omp parallel for private(j, p) shared(a, b, c) schedule(dynamic)
  for (i = 0; i < m; i++) /* Loop over the rows of C */
  {
    for (j = 0; j < n; j++) /* Loop over the columns of C */
    {
      int temp = 0;
      for (p = 0; p < k; p++)
      { /* Update C( i,j ) with the inner product of the ith row of A and the jth column of B */
        temp += A(i, p) * B(p, j);
      }
      C(i, j) = temp;
    }
  }
}
