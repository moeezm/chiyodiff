#ifndef TENSOR_H
#define TENSOR_H

#include <stdbool.h>
extern const bool CHECK_DIMS;

typedef struct {
	double *arr;
	int *dims;
	int dimlen;
	int *dimszs;
	int n;
} Tensor;

void Tinit(Tensor *t, int dims[], int dim_len);
void Tfree(Tensor *t);
void Tnext(Tensor *t, int idxs[]);
void Tjump(Tensor *t, int *i, int idxs[], int dim_i, int jmp_amt);
Tensor* Tadd(Tensor *A, Tensor *B);
Tensor* Tcontract(Tensor *A, Tensor *B, int kai);
void Tprint(Tensor *t);
void test_tensor();
#endif
