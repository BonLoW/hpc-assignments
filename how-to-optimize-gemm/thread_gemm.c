#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>
#include "defs.h"

typedef struct {
    int id;             // 线程ID
    int num_threads;    // 总线程数
    int m;              // A 矩阵的行数
    int n;              // B 矩阵的列数
    int k;              // A 矩阵的列数，B 矩阵的行数
    double *a;         // 矩阵 A
    double *b;         // 矩阵 B
    double *c;         // 矩阵 C
    int lda;
    int ldb;
    int ldc;
} thread_arg_t;


void *dgemm_thread(void *arg) {
    thread_arg_t *t_arg = (thread_arg_t *)arg;

    int i, j, l;
    int lda = t_arg->lda;
    int ldb = t_arg->ldb;
    int ldc = t_arg->ldc;
    int m_start = (t_arg->m / t_arg->num_threads) * t_arg->id;
    int m_end = (t_arg->m / t_arg->num_threads) * (t_arg->id + 1);
    if (t_arg->id == t_arg->num_threads - 1) {
        m_end = t_arg->m;  // 最后一个线程处理剩余的行
    }

    // 进行矩阵乘法的计算
    for (i = m_start; i < m_end; ++i) {
        for (j = 0; j < t_arg->n; ++j) {
            t_arg->C(i, j) = 0.0; // 初始化 C 矩阵的相应位置
            for (l = 0; l < t_arg->k; ++l) {
                t_arg->C(i, j) += t_arg->A(i, l) * t_arg->B(l, j);
            }
        }
    }

    pthread_exit(NULL);
}


void thread_gemm(int m, int n, int k, double *a, int lda, double *b, int ldb, double *c, int ldc) {
    int num_threads = 8;
    pthread_t threads[num_threads];
    thread_arg_t thread_args[num_threads];

    // 创建线程并传递参数
    for (int i = 0; i < num_threads; ++i) {
        thread_args[i].id = i;
        thread_args[i].num_threads = num_threads;
        thread_args[i].m = m;
        thread_args[i].n = n;
        thread_args[i].k = k;
        thread_args[i].a = a;
        thread_args[i].b = b;
        thread_args[i].c = c;
        thread_args[i].lda = lda;
        thread_args[i].ldb = ldb;
        thread_args[i].ldc = ldc;

        pthread_create(&threads[i], NULL, dgemm_thread, (void *)&thread_args[i]);
    }

    // 等待所有线程完成
    for (int i = 0; i < num_threads; ++i) {
        pthread_join(threads[i], NULL);
    }
}
