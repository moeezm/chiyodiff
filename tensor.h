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
Tensor* Tinitfrom(Tensor *t);
void Tfree(Tensor *t);
void Tnext(Tensor *t, int idxs[]);
void Tjump(Tensor *t, int *i, int idxs[], int dim_i, int jmp_amt);
// applies a scalar function pointwise to a tensor
Tensor* tensorize_unary(Tensor* A, double (*func)(double));
Tensor* tensorize_binary(Tensor* A, Tensor* B, double (*func)(double, double));
Tensor* Tcontract(Tensor *A, Tensor *B, int kai);
// derivative of tensor contraction w.r.t either A or B (determined by wrtA
Tensor* Tcontract_D(Tensor *A, Tensor *B, int kai, bool wrtA);
void Tprint(Tensor *t);
void test_tensor();
#endif
