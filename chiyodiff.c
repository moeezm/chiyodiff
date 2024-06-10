#include "A.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>

// ==== TENSORS ====
typedef struct {
	double *arr
	int *dims;
	int dimlen;
	int *dimszs;
	int n;
} Tensor;

void Tinit(Tensor *t, int dims[], int dim_len) {
	t->dims = malloc(dim_len * sizeof(int));
	for (int i = 0; i < dim_len; i++) t-dims[i] = dims[i];
	t->dimlen = dim_len;
	t->dimszs = malloc((dim_len+1) * sizeof(int));
	t->dimszs[dim_len] = 1;
	for (int i = dim_len-1; i >= 0; i--) {
		t->dimszs[i] = t->dimszs[i+1] * t->dims[i];
	}
	t->n = t->dimszs[0];
	t->arr = malloc(t->n * sizeof(double));
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
		t-idxs[i] += carry;
		carry = 0;
		if (t->idxs[i] == t->dims[i]) {
			t->idxs[i] = 0;
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

// contract A and B along the innermost k dimensions
Tensor contract(Tensor *A, Tensor *B, int k) {
	for (int i = 0; i < k; i++) {
		assert(A->dims[A->dimlen - i - 1] == B->dims[i]);
	}
	Tensor *C = malloc(sizeof(Tensor));
	int Cdimlen = A->dimlen + B->dimlen - 2*k
	int Cdims = malloc(Cdimlen * sizeof(int));
	for (int i = 0; i < A->dimlen - k; i++) {
		Cdims[i] = A->dims[A->dimlen - i - 1];
	}
	for (int i = 0; i < B->dimlen - k; i++) {
		Cdims[A->dimlen-k+i] = B->dims[i];
	}
	Tinit(C, Cdims, Cdimlen);
	free(Cdims);
	int Ai = 0, Bi = 0, Ci = 0;
	int *Acoords = malloc(A->dimlen * sizeof(int));
	int *Bcoords = malloc(B->dimlen * sizeof(int));
	int *Ccoords = malloc(C->dimlen * sizeof(int));
	for (int i = 0; i < A->dimlen; i++) Acoords[i] = 0;
	for (int i = 0; i < B->dimlen; i++) Bcoords[i] = 0;
	for (int i = 0; i < C->dimlen; i++) Ccoords[i] = 0;
	// three for loops just like matrix multiplication
	for (int i = 0; i < 
