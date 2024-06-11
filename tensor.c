#include "tensor.h"
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <stdio.h>
#include <stdbool.h>

const bool CHECK_DIMS = true;

void Tinit(Tensor *t, int dims[], int dim_len) {
	t->dims = malloc(dim_len * sizeof(int));
	for (int i = 0; i < dim_len; i++) t->dims[i] = dims[i];
	t->dimlen = dim_len;
	t->dimszs = malloc((dim_len+1) * sizeof(int));
	t->dimszs[dim_len] = 1;
	for (int i = dim_len-1; i >= 0; i--) {
		t->dimszs[i] = t->dimszs[i+1] * t->dims[i];
	}
	t->n = t->dimszs[0];
	t->arr = malloc(t->n * sizeof(double));
	memset(t->arr, 0, t->n * sizeof(double));
}
// zero tensor of same shape as t
Tensor* Tinitfrom(Tensor *t) {
	Tensor *res = malloc(sizeof(Tensor));
	Tinit(res, t->dims, t->dimlen);
	return res;
}
void Tfree(Tensor *t) {
	free(t->dims);
	free(t->dimszs);
	free(t->arr);
}

// given coordinates, compute the next coordinate
// (basically add with carry)
// so to iterate over a tensor you can do something like for (int i = 0; i < t->n; i++,next(i, coords))
void Tnext(Tensor *t, int idxs[]) {
	int carry = 1;
	for (int i = t->dimlen - 1; i >= 0; i--) {
		idxs[i] += carry;
		carry = 0;
		if (idxs[i] == t->dims[i]) {
			idxs[i] = 0;
			carry = 1;
		}
	}
}

// more powerful/fine grained next
// takes the "global" index i, as well as the index of the dimension to increment, as well as how much to increment it by
void Tjump(Tensor *t, int *i, int idxs[], int dim_i, int jmp_amt) {
	(*i) += (jmp_amt * t->dimszs[dim_i+1]);
	for (int i = dim_i; i >= 0; i--) {
		idxs[dim_i] += jmp_amt;
		jmp_amt = idxs[dim_i] / t->dims[dim_i];
		idxs[dim_i] %= t->dims[dim_i];
	}
}

// add two tensors of the same shape
Tensor* Tadd(Tensor *A, Tensor *B) {
	if (CHECK_DIMS) {
		assert(A->dimlen == B->dimlen);
		for (int i = 0; i < A->dimlen; i++) {
			assert(A->dims[i] == B->dims[i]);
		}
	}
	Tensor *res = malloc(sizeof(Tensor));
	Tinit(res, A->dims, A->dimlen);
	for (int i = 0; i < A->n; i++) res->arr[i] = A->arr[i] + B->arr[i];
	return res;
}

Tensor* tensorize_unary(Tensor* A, double (*func)(double)) {
	Tensor *res = malloc(sizeof(Tensor));
	Tinit(res, A->dims, A->dimlen);
	for (int i = 0; i < A->n; i++) {
		res[i] = func(A[i]);
	}
	return res;
}

Tensor* tensorize_binary(Tensor* A, Tensor* B, double (*func)(double, double)) {
	if (CHECK_DIMS) {
		assert(A->dimlen == B->dimlen);
		for (int i = 0; i < A->dimlen; i++) {
			assert(A->dims[i] == B->dims[i]);
		}
	}
	Tensor *res = malloc(sizeof(Tensor));
	Tinit(res, A->dims, A->dimlen);
	for (int i = 0; i < A->n; i++) {
		res[i] = func(A[i], B[i]);
	}
	return res;
}
// contract A and B along the innermost kai dimensions
Tensor* Tcontract(Tensor *A, Tensor *B, int kai) {
	if (CHECK_DIMS) {
		assert(kai <= A->dimlen);
		assert(kai <= B->dimlen);
		for (int i = 0; i < kai; i++) {
			assert(A->dims[A->dimlen - i - 1] == B->dims[i]);
		}
	}
	Tensor *C = malloc(sizeof(Tensor));
	int Cdimlen = A->dimlen + B->dimlen - 2*kai;
	int *Cdims = malloc(Cdimlen * sizeof(int));
	for (int i = 0; i < A->dimlen - kai; i++) {
//		Cdims[i] = A->dims[A->dimlen - i - 1];
		Cdims[i] = A->dims[i];
	}
	for (int i = 0; i < B->dimlen - kai; i++) {
		Cdims[A->dimlen-kai+i] = B->dims[kai+i];
	}
	Tinit(C, Cdims, Cdimlen);
	free(Cdims);
	int Ai = 0, Bi = 0, Ci = 0;
	// three for loops just like matrix multiplication
	// i is the part of the final coordinate from A, j is the part from B, k runs through the common part
	for (int i = 0; i < (A->dimszs[0] / A->dimszs[A->dimlen - kai]); i++) {
		for (int j = 0; j < B->dimszs[kai]; j++) {
			Ci = i * B->dimszs[kai] + j;
			double s = 0;
			for (int k = 0; k < A->dimszs[A->dimlen - kai]; k++) {
				Ai = i * A->dimszs[A->dimlen - kai] + k;
				Bi = k * B->dimszs[kai] + j;
				s += A->arr[Ai] * B->arr[Bi];
			}
			C->arr[Ci] = s;
		}
	}
	return C;
}

// derivative of tensor contraction w.r.t either A or B (determined by wrtA
Tensor* Tcontract_D(Tensor *A, Tensor *B, int kai, bool wrtA) {
	// idea is to do a similar nested 3 for loops to Tcontract
	// and compute Ci, Ai, Bi
	// with Ci/(Ai,Bi), we can easily compute the relevant index in the derivative (e.g. Ci * some_dimszs + Ai)
	// and then (del Ci)/(del Ai) is the corresponding Bi
	// not sure if there's a cleaner/more efficient way to do this
	Tensor *D = malloc(sizeof(Tensor));
	int Ddimlen = A->dimlen + B->dimlen - 2*kai; // C part
	int *Ddims;
	if (wrtA) {
		Ddimlen += A->dimlen;
	}
	else {
		Ddimlen += B->dimlen;
	}
	Ddims = malloc(Ddimlen * sizeof(int));
	for (int i = 0; i < A->dimlen - kai; i++) {
		//Ddims[i] = A->dims[A->dimlen - i - 1];
		Ddims[i] = A->dims[i];
	}
	for (int i = 0; i < B->dimlen - kai; i++) {
		Ddims[A->dimlen - kai + i] = B->dims[kai + i];
	}
	if (wrtA) {
		for (int i  = 0; i < A->dimlen; i++) {
			Ddims[A->dimlen + B->dimlen - 2*kai + i] = A->dims[i];
		}
	}
	else {
		for (int i  = 0; i < B->dimlen; i++) {
			Ddims[A->dimlen + B->dimlen - 2*kai + i] = B->dims[i];
		}
	}
	printf("==\n");
	for (int i = 0; i < Ddimlen; i++) {
		printf("%d ", Ddims[i]);
	}
	printf("\n");
	printf("==\n");
	Tinit(D, Ddims, Ddimlen);
	free(Ddims);
	int Ai, Bi, Ci, Di;
	for (int i = 0; i < (A->dimszs[0]/A->dimszs[A->dimlen - kai]); i++) {
		for (int j = 0; j < B->dimszs[kai]; j++) {
			Ci = i * B->dimszs[kai] + j;
			for (int k = 0; k < A->dimszs[A->dimlen - kai]; k++) {
				Ai = i * A->dimszs[A->dimlen - kai] + k;
				Bi = k * B->dimszs[kai] + j;
				if (wrtA) {
					Di = Ci * A->dimszs[0] + Ai;
					D->arr[Di] = B->arr[Bi];
				}
				else {
					Di = Ci * B->dimszs[0] + Bi;
					D->arr[Di] = A->arr[Ai];
				}
			}
		}
	}
	return D;
}

void Tprint(Tensor *t) {
	if (t->dimlen <= 1 || t->dimlen > 3) {
		for (int i = 0; i < t->n; i++) printf("%f ", t->arr[i]);
	}
	else if (t->dimlen == 2) {
		for (int i = 0; i < t->dims[0]; i++) {
			for (int j = 0; j < t->dims[1]; j++) {
				printf("%f ", t->arr[i * t->dimszs[1] + j]);
			}
			printf("\n");
		}
	}
	else {
		for (int i = 0; i < t->dims[0]; i++) {
			for (int j = 0; j < t->dims[1]; j++) {
				for (int k = 0; k < t->dims[2]; k++) {
					printf("%f ", t->arr[i*t->dimszs[1] + j*t->dimszs[2] + k]);
				}
				printf("\n");
			}
			printf("\n");
		}
	}
}

void test_tensor() {
	Tensor tscal1;
	Tensor tscal2;
	Tinit(&tscal1, NULL, 0);
	Tinit(&tscal2, NULL, 0);
	tscal1.arr[0] = 3;
	tscal2.arr[0] = 2;
	Tensor* res = Tcontract(&tscal1, &tscal2, 0);
	Tprint(res);
	printf("\n");
	Tensor *deriv = Tcontract_D(&tscal1, &tscal2, 0, false);
	Tprint(deriv);
	printf("\n");

	Tensor t1;
	Tensor t2;
	int t1dims[] = {3, 2};
	int t2dims[] = {2};
	Tinit(&t1, t1dims, 2);
	Tinit(&t2, t2dims, 1);
	t1.arr[0] = 2; t1.arr[1] = 3; t1.arr[2] = 1; t1.arr[3] = 5;
	t1.arr[4] = 7; t1.arr[5] = 4;
	t2.arr[0] = 1; t2.arr[1] = 3;
	Tprint(&t1);
	printf("\n");
	Tensor *res2 = Tcontract(&t1, &t2, 1);
	Tprint(res2);
	printf("\n");
	Tensor *deriv2 = Tcontract_D(&t1, &t2, 1, false);
	printf("%d\n", deriv2->dimlen);
	Tprint(deriv2);
	printf("\n");
}
