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

Tensor* Tinit(int dims[], int dim_len);
Tensor* Tinitfrom(Tensor *t);
void Tfree(Tensor *t);
void Tnext(Tensor *t, int idxs[]);
void Tjump(Tensor *t, int *i, int idxs[], int dim_i, int jmp_amt);
// A += B
void Tpadd(Tensor *A, Tensor *B);
// applies a scalar function pointwise to a tensor
Tensor* tensorize_unary(Tensor* A, double (*func)(double));
Tensor* tensorize_binary(Tensor* A, Tensor* B, double (*func)(double, double));
Tensor* tensorize_unary_D(Tensor* A, double (*func)(double));
Tensor* tensorize_binary_D(Tensor* A, Tensor* B, double (*func)(double, double));
// returns shape of contracting A with B along inner kai dimensions
int* Tcontractshape(Tensor *A, Tensor *B, int kai, int *out_dimlen);
Tensor* Tcontract(Tensor *A, Tensor *B, int kai);
// derivative of tensor contraction w.r.t either A or B (determined by wrtA
Tensor* Tcontract_D(Tensor *A, Tensor *B, int kai, bool wrtA);
void Tprint(Tensor *t);
void test_tensor();
#endif
