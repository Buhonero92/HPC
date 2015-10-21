#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#define n 40
#define m 40


__global__ void matMult(int* A, int* B, int* C, int w){
	int row= blockIdx.y*blockDim.y + threadIdx.y;

	int col= blockIdx.x*blockDim.x + threadIdx.x;

	if ((row < w) && (col < w)){
		int pvalue=0;
		
		for(int k=0;k<w;k++){
			pvalue+=A[row*w+k]*B[k*w+col];
		}
		C[row*w+col]=pvalue;
	}
	
}

int matrixMult( int *A, int *B, int *C){
	int size = n*m*sizeof(int);
	int *d_A, *d_B, *d_C;
	//int dimGrid = 0;
	//Reservo Memoria en el dispositivo
	cudaMalloc((void **)&d_A, size);
	cudaMalloc((void **)&d_B, size);
	cudaMalloc((void **)&d_C, size);
	
    dim3 blockSize(32,32); //mayor cantidad de hilos por bloque
	dim3 dimGrid(ceil((float)n/ blockSize.x),ceil((float)m/ blockSize.y),1);
	clock_t t;
	t=clock();
	//Copio los datos al device
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	// Ejecuto el Kernel (del dispositivo)
	matMult<<< dimGrid, blockSize>>>(d_A, d_B, d_C, m);
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
		A[i]=rand()% 10;
		B[i]=rand()% 10;
	}
	matrixMult(A,B,C);
	ImpMatrix(A);
	ImpMatrix(B);
	ImpMatrix(C);
	//sleep(5);
	free(A);
	free(B);
	free(C);
	return 0;

}
