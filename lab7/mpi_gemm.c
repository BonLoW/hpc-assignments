#include <stdio.h>
#include "mpi.h"
#include <stdlib.h>
#include "mpi_test.h"
 
 
int main(int argc, char **argv)
{
   int my_rank,comm_sz,line;
   double start, stop; //计时时间
   MPI_Status status;
   char Processorname[20];
 
   double *Matrix_A,*Matrix_B,*Matrix_C,*ans,*buffer_A,*buffer_C;
 
   MPI_Init(&argc,&argv);
   MPI_Comm_size(MPI_COMM_WORLD, &comm_sz);
   MPI_Comm_rank(MPI_COMM_WORLD,&my_rank);
 
   int M, N, K;
   M = N = K = 8;
  
   line=M/comm_sz; // 每个进程分多少行数据
   Matrix_A=(double*)malloc(M*N*sizeof(double));
   Matrix_B=(double*)malloc(N*K*sizeof(double));
   Matrix_C=(double*)malloc(M*K*sizeof(double));
   buffer_A=(double*)malloc(line*N*sizeof(double)); // A的均分行的数据
   buffer_C=(double*)malloc(line*K*sizeof(double)); // C的均分行的数据
   ans=(double*)malloc(line*K*sizeof(double)); // 临时保存部分数据计算结果
   test_init(3, Matrix_A, Matrix_B, Matrix_C);
 
   // 给矩阵A B,C赋值
   if(my_rank==0){
      /*将矩阵广播出去*/
      for(int i=1;i<comm_sz;i++){
         MPI_Send(Matrix_A+(i-1)*line*N,line*N,MPI_DOUBLE,i,66,MPI_COMM_WORLD);
         MPI_Send(Matrix_C+(i-1)*line*K,line*K,MPI_DOUBLE,i,99,MPI_COMM_WORLD);
      }
      MPI_Bcast(Matrix_B,N*K,MPI_DOUBLE,0,MPI_COMM_WORLD);
 
      // 接收从进程的计算结果
      for(int p=1;p<comm_sz;p++){
         MPI_Recv(ans,line*K,MPI_DOUBLE,p,33,MPI_COMM_WORLD,&status);
         for(int i=0;i<line;i+=comm_sz)
            for(int j=0;j<K;j++)
               Matrix_C[((p-1)*line+i)*K+j]=ans[i*K+j];
      }
 
      // 计算A剩下的行数据
      for(int i=(comm_sz-1)*line;i<M;i++){
         for(int j=0;j<K;j++){
            double temp=0;
            for(int p=0;p<N;p++)
               temp+=Matrix_A[i*N+p]*Matrix_B[p*K+j];
            Matrix_C[i*K+j]=temp+Matrix_C[i*K+j];
         }
      }
 
      Matrix_print(Matrix_C,M,K);
      stop=MPI_Wtime();
 
      printf("rank:%d time:%lfs\n",my_rank,stop-start);
 
      free(Matrix_A);
      free(Matrix_B);
      free(Matrix_C);
      free(buffer_A);
      free(buffer_C);
      free(ans);
   }
   else{
      //接收广播的数据
      MPI_Recv(buffer_A,line*N,MPI_DOUBLE,0,66,MPI_COMM_WORLD,&status);
      MPI_Recv(buffer_C,line*K,MPI_DOUBLE,0,99,MPI_COMM_WORLD,&status);
      MPI_Bcast(Matrix_B,N*K,MPI_DOUBLE,0,MPI_COMM_WORLD);
 
      //计算乘积结果,并将结果发送给主进程
      for(int i=0;i<line;i++){
         for(int j=0;j<K;j++){
            double temp=0;
            for(int p=0;p<N;p++){
               temp+=buffer_A[i*N+p]*Matrix_B[p*K+j];
            }
            ans[i*line+j]=temp+buffer_C[i*K+j];
         }
      }
      MPI_Send(ans,line*K,MPI_DOUBLE,0,33,MPI_COMM_WORLD);
   }
 
   MPI_Finalize();
   
   return 0;
}
