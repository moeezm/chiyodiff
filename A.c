#include "A.h"
#include <stdlib.h>

// dynamic circular array, also supports queue operations
A* Ainit() {
	A* a = malloc(sizeof(A));
	a->arr = malloc(sizeof(Node*));
	a->sz = 1;
	a->n = 0;
	a->front = 0;
	return a;
}

Node* Aget(A* a, int i) {
	return (a->arr[(a->front + i)%(a->sz)]);
}

void Aset(A* a, int i, Node* x) {
	a->arr[(a->front + i)%(a->sz)] = x;
}

// push_back
void Apb(A* a, Node* x) {
	if (a->n == a->sz) {
		// double size of underlying array and copy
		Node** newarr = malloc(2*(a->sz)*sizeof(Node*));
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

Node* Afront(A* a) {
	return Aget(a, 0);
}
// pop front
Node* Apopf(A* a) {
	Node* x = Aget(a, 0);
	a->front = (a->front+1)%(a->sz);
	a->n--;
	return x;
}

void Afree(A* a) {
	free(a->arr);
	free(a);
}
