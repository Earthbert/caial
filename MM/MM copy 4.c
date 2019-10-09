/*
* Matrix multiplication
*
*/
#include <stdlib.h>
#include "util.h"
#include "../common/common.h"

#define DATA_LENGTH 3

// element_t A[DATA_LENGTH][DATA_LENGTH], B[DATA_LENGTH][DATA_LENGTH], C[DATA_LENGTH][DATA_LENGTH];

element_t AA[DATA_LENGTH][DATA_LENGTH] = {{1.0, 2.0, 3.0}, {4.0, 5.0, 6.0}, {7.0, 8.0, 9.0}};
element_t BB[DATA_LENGTH][DATA_LENGTH] = {{1.0, 0.0, 0.0}, {0.0, 1.0, 0.0}, {0.0, 0.0, 1.0}};

uint32_t iAA[DATA_LENGTH][DATA_LENGTH] = {{0x00000040, 0x00000048, 0x0000004c}, {0x00000050, 0x00000052, 0x00000054}, {0x00000056, 0x00000058, 0x00000059}};
uint32_t iBB[DATA_LENGTH][DATA_LENGTH] = {{0x00000040, 0x0, 0x0}, {0x0, 0x00000040, 0x0}, {0x0, 0x0, 0x00000040}};

element_t CC[DATA_LENGTH][DATA_LENGTH] = {{0}};
element_t C2[DATA_LENGTH*DATA_LENGTH];

void init() {
	int i;
	uint32_t* addr;
	for (i = 0; i < 9; i++) {
		//memcpy(&AA+i, &iAA+i, 4);
		//memcpy(&BB+i, &iBB+i, 4);
		addr = (uint32_t*)&AA[i/3][i%3]; 
		*addr = iAA[i/3][i%3];
		addr = (uint32_t*)&BB[i/3][i%3];
                *addr = iBB[i/3][i%3];
	}
}

/**
* Initialize matrix A
*
*/
void gen_matrix(element_t A[DATA_LENGTH][DATA_LENGTH], size_t rows, size_t columns) {
	size_t index, jindex, kindex = 0;
	for (index = 0; index < rows; index++)
		for (jindex = 0; jindex < columns; jindex++) {
#ifdef WITHINT
			A[index][jidnex] = kindex++;
#else
			A[index][jindex] = kindex++/3.14;
#endif
		}
}

#ifdef DEBUG
/**
* Print elements of matrix A
*
*/
void print_matrix(element_t A[DATA_LENGTH][DATA_LENGTH], size_t rows, size_t columns) {
	size_t index, jindex;
	for (index = 0; index < rows; index++)
		for (jindex = 0; jindex < columns; jindex++) {
			printf("%ld ", A[index][jindex]);
		printf("\n");
	}
	printf("\n");
}
#endif


/**
* Multiply matrix A (n rows and m columns) 
* with B (m rows and p columns) and put the result
* in matrix C (n rows and p columns)
*/
void matrix_multiplication(element_t A[DATA_LENGTH][DATA_LENGTH], element_t B[DATA_LENGTH][DATA_LENGTH], element_t C[DATA_LENGTH][DATA_LENGTH], size_t n, size_t m, size_t p) {
	size_t index, jindex, kindex;
	/**/ for (index = 0; index < n; index++)
		for (jindex = 0; jindex < p; jindex++) {
			C[index][jindex] = A[0][0] * B[0][0];
			C2[index*3+jindex] = A[index][jindex] * B[0][0];
			/* for (kindex = 0; kindex < m; kindex++) {
				C[index][jindex] = C[index][jindex] + A[index][kindex] * B[kindex][jindex];
			}*/
		}
		/* 
	for (index = 0; index < n; index++) {
		C2[2*index] = A[0][0] * B[0][0];
	}
	*/
}

int main() {
	size_t n = 3;
	size_t m = 3;
	size_t p = 3;
//	gen_matrix(A, n, m);
//	gen_matrix(B, m, p);
	init();	
	matrix_multiplication(AA, BB, CC, n, m, p);

#ifdef DEBUG
	print_matrix(A, n, m);
	print_matrix(B, m, p);
	print_matrix(C, n, p);
#endif

	return 0;
}
