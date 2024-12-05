#include <cblas.h>
#include "defs.h"

void cblas_gemm(int m, int n, int k, double *A, int lda, double *B, int ldb,
                double *C, int ldc) {
  double alpha = 1.0;
  double beta = 2.0;
  cblas_dgemm(CblasRowMajor, CblasNoTrans, CblasNoTrans, m, n, k, alpha, A, lda,
              B, ldb, beta, C, ldc);
}
