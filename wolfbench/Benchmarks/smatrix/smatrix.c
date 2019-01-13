




/*
 * Matrix Multiplication for M3T
 */

#include <stdio.h>
#include <fcntl.h>
#include <stdint.h>
#include <stdlib.h>

#define MAXSIZE 1024

int size=64;
double total=0;

int A[MAXSIZE][MAXSIZE],B[MAXSIZE][MAXSIZE],C[MAXSIZE][MAXSIZE];

float RA[MAXSIZE*MAXSIZE];
float RB[MAXSIZE*MAXSIZE];
float RC[MAXSIZE*MAXSIZE];

void matmult()
{
	int i,j,k;

	printf("Native Matrix Multiplication\n");
	
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			RC[C[i][0]] = 0;
			for(k=0;k<size;k++)
				RC[C[i][j]] = RC[C[i][j]] + RA[A[i][j]]*RB[B[j][k]];
		}
	}

}


int main(int argc, char **argv)
{
	int i,j,k;
	int opt;
	
	if( argc > 2 ){
		printf("usage:\n\tsmatrix [size]\n");
		exit(0);
	}
	if( argc > 1 ){
		size = atoi(argv[1]);
		if( size < 2 || size > MAXSIZE )
			size = MAXSIZE;
	}
	
	printf("Matrix Multiplication Kernel (%dx%d)\n",size,size);
	
	/* SETUP */
	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			A[i][j] = (i*size+j*size) % size;
			B[i][j] = i*size+j;
			C[i][j] = (i*size/2+j*size/2) % size;
		}
	}

	for(i=0;i<size;i++){
		for(j=0;j<size;j++){
			RA[A[i][j]] = i*j*i+10;
			RB[B[i][j]] = i/(j*i-i*j/3+3);
		}
	}
	
	printf("Phase 2\n");

	matmult();
	
	printf("Phase 3\n");

	/* Verify RESULTS */
	for(i=0;i<size;i+=2)
	  for(j=0;j<size;j+=3)
		if( RC[C[i][j]] > 3000000)
			total +=  RC[C[i][j]]/1000000;
	
	if( size == 64 )
		printf("Verification total=%g should be 31599.2\n",total);
	else
		printf("Verification total=%g\n",total);

	return 0;
}

