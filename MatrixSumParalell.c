#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define n 4
#define m 4


__global__ void matAdd(int* A, int* B, int* C, int w){
	int row= blockIdx.y*blockDim.y + threadIdx.y;

	int col= blockIdx.x*blockDim.x + threadIdx.x;

	if ((row < w) && (col < w)){
		C[row*w+col]=A[row*w+col]+B[row*w+col];
		//printf("%i + %i = %i \n",A[row*columnas+col] ,B[row*columnas+col],C[row*columnas+col]);
	}
	
}

int matrixAdd( int *A, int *B, int *C){
	int size = n*m*sizeof(int);
	int *d_A, *d_B, *d_C;
	//int dimGrid = 0;
	//Reservo Memoria en el dispositivo
	cudaMalloc((void **)&d_A, size);
	cudaMalloc((void **)&d_B, size);
	cudaMalloc((void **)&d_C, size);
	dim3 blockSize(m, n);
	dim3 dimGrid(ceil(n/blockSize.x),ceil(m/blockSize.y));
  	//printf("%d\n", dimGrid);
	clock_t t;
	t=clock();
	//Copio los datos al device
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	// Ejecuto el Kernel (del dispositivo)
	matAdd<<< dimGrid, blockSize>>>(d_A, d_B, d_C, n);
	cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);

	printf("El tiempo es de \t: %.8f\n",(clock()-t)/(double)CLOCKS_PER_SEC);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	return 0;
}
void ImpMatrix(int *matrix){
	printf("\n");
	for(int i=0; i< n; i++){
		for(int j=0 ; j< m;j++){
			printf("%i ",matrix[i*m+j]);
		}
		printf("\n");
	}
}

int main(){
    
	int *A=(int *) malloc(n*m*sizeof(int));
	
	int *B=(int *) malloc(n*m*sizeof(int));
	
	int *C=(int *) malloc(n*m*sizeof(int));

	int i;
	for(i=0; i < n*m; i++){
		A[i]=rand()% 20;
		B[i]=rand()% 20;
	}
	matrixAdd(A,B,C);
	ImpMatrix(A);
	ImpMatrix(B);
	ImpMatrix(C);
	//sleep(5);
	free(A);
	free(B);
  free(C);
	return 0;

}
