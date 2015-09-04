#include <stdio.h>
#include <cuda.h>
#include <stdlib.h>
#include <time.h>
const int  SIZE[] = {512,1024,3000,10000,500000, 1000000, 10000000, 50000000};

int main(){
  
	
	int *A;
    int *B;
  	int *C;
	int j;
	for( j=0; j<8;j++){
    
		A=(int *) malloc(SIZE[j]*sizeof(int));
	
		B=(int *) malloc(SIZE[j]*sizeof(int));
	
		C=(int *) malloc(SIZE[j]*sizeof(int));
    
		clock_t t;
		t=clock();
		int i;
		for(i=0;i< SIZE[j]; i++){
			A[i]=rand()% 20;
			B[i]=rand()% 20;
		
		}
		
		for(i=0 ; i< SIZE[j] ; i++){
			C[i]=A[i]+B[i];
			//printf("%i + %i = %i\n",A[i],B[i],C[i]);

		}
		//sleep(5);
		printf("El tiempo transcurrido para procesar %i datos es:\t%.8f\n",SIZE[j],(clock()-t)/(double)CLOCKS_PER_SEC);
	}
	free(A);
	free(B);
	return 0;
	
}
