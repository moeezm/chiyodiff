#include "A.h"
#include <stdlib.h>

// dynamic circular array, also supports queue operations
void A_init(A* a) {
	a->arr = malloc(sizeof(double));
	a->sz = 1;
	a->n = 1;
	a->front = 0;
}

double Aget(A* a, int i) {
	return (a->arr[(a->front + i)%(a->sz)]);
}

void Aset(A* a, int i, double x) {
	a->arr[(a->front + i)%(a->sz)] = x;
}

// push_back
void Apb(A* a, double x) {
	if (a->n == a->sz) {
		// double size of underlying array and copy
		double* newarr = malloc(2*(a->sz)*sizeof(double));
		for (int i = 0; i < a->n; i++) {
			newarr[i] = Aget(a, i);
		}
		free(a->arr);
		a->arr = newarr;
		a->sz *= 2;
		a->front = 0;
	}
	Aset(a, a->n, x);
	a->n++;
}

double Afront(A* a) {
	return Aget(a, 0);
}
// pop front
double Apopf(A* a) {
	double x = Aget(a, 0);
	a->front = (a->front+1)%(a->sz);
	return x;
}

void Aclean(A* a) {
	free(a->arr);
}
