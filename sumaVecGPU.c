#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <time.h>

#define blockSize 1024

const int  SIZE[] = {512,1024,3000,10000,500000, 1000000, 10000000, 50000000};


__global__ void vecAdd(int *A, int *B, int *C,int n){
	int i = blockIdx.x*blockDim.x+threadIdx.x;
	if(i< n){
		C[i]=A[i]+B[i];		
	}


}

int vectorAdd( int *A, int *B, int *C, int n,int j){
	int size = n*sizeof(int);
	int *d_A, *d_B, *d_C;
	int dimGrid = 0;
	//Reservo Memoria en el dispositivo
	cudaMalloc((void **)&d_A, size);
	cudaMalloc((void **)&d_B, size);
	cudaMalloc((void **)&d_C, size);
	//Copio los datos al device
	
	dimGrid = (int)ceil((float)n/blockSize);
  	//printf("%d\n", dimGrid);
	clock_t t;
	t=clock();
	cudaMemcpy(d_A, A, size, cudaMemcpyHostToDevice);
	cudaMemcpy(d_B, B, size, cudaMemcpyHostToDevice);
	// Ejecuto el Kernel (del dispositivo)
	vecAdd<<< dimGrid, blockSize >>>(d_A, d_B, d_C, n);
	cudaMemcpy(C, d_C, size, cudaMemcpyDeviceToHost);
	printf("El tiempo transcurrido para procesar %i datos es:\t %.8f\n",SIZE[j],(clock()-t)/(double)CLOCKS_PER_SEC);
	cudaFree(d_A);
	cudaFree(d_B);
	cudaFree(d_C);
	return 0;
}


int main(){
	int j;
	int *A;
    int *B;
  	int *C;
	for( j=0; j<8;j++){
    
		A=(int *) malloc(SIZE[j]*sizeof(int));
	
		B=(int *) malloc(SIZE[j]*sizeof(int));
	
		C=(int *) malloc(SIZE[j]*sizeof(int));
		//time_t inicio,fin;
		//inicio=time(NULL);
		int i;
		for(i=0;i < SIZE[j]; i++){
			A[i]=rand()% 20;
			B[i]=rand()% 20;
		
		}
		/*for(i=0;i< sizeof(A); i++){
			C[i]=A[i]+B[i];
			printf("%f+",A[i]);
			printf("%f=",B[i]);
			printf("%f\n",C[i]);
		}*/
		vectorAdd(A,B,C,SIZE[j],j);
	}
	free(A);
	free(B);
	free(C);
	return 0;
}
