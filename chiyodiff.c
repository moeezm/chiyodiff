#include "A.h"
#include "tensor.h"
#include "funcs.h"
#include "chiyodiff.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

Node* Ninit(Node* child1, Node* child2, Tensor* (*f)(Tensor*[]), Tensor* (*fp)(Tensor*[], int i), int *dims, int dimlen, Graph* g) {
	Node* out = malloc(sizeof(Node));
	out->f = f;
	out->fp = fp;
	out->children = Ainit();
	out->parents = Ainit();
	assert(!(child1 == NULL && child2 != NULL));
	if (child1 != NULL) {
		Apb(out->children, child1);
		Apb(child1->parents, out);
	}
	if (child2 != NULL) {
		Apb(out->children, child2);
		Apb(child2->parents, out);
	}
	out->val = Tinit(dims, dimlen);
	out->deriv = Tinit(dims, dimlen);
	if (g != NULL) {
		Gadd(g, out);
	}
	return out;
}

Node* Ninit_val(Tensor* t, Graph* g) {
	Node* u = Ninit(NULL, NULL, NULL, NULL, t->dims, t->dimlen, g);
	for (int i = 0; i < t->n; i++) u->val->arr[i] = t->arr[i];
	return u;
}

Node* Ninit_tizedop(Node* child1, Node* child2, Tensor* (*f)(Tensor*[]), Tensor* (*fp)(Tensor*[], int i), Graph* g) {
	return Ninit(child1, child2, f, fp, child1->val->dims, child1->val->dimlen, g);
}
Node* Ninit_matmul(Node* child1, Node* child2, Graph* g) {
	int dimlen;
	int* dims = Tcontractshape(child1->val, child2->val, 1, &dimlen);
	Node* res = Ninit(child1, child2, matmul, matmul_D, dims, dimlen, g);
	free(dims);
	return res;
}
void Neval(Node* u) {
	if (u->children->n == 0) return;
	Tensor** ts = malloc(u->children->n * sizeof(Tensor*));
	for (int i = 0; i < u->children->n; i++) ts[i] = Aget(u->children, i)->val;
	printf("PRINTING CHILDREN\n"); 
	for (int i = 0; i < u->children->n; i++) {
		Tprint(ts[i]);
		printf("\n");
		printf("\n");
	}
	printf("\n");
	u->val = u->f(ts);
	free(ts);
}
Tensor* Neval_D(Node* u, int i) {
	Tensor** ts = malloc(u->children->n * sizeof(Tensor*));
	for (int i = 0; i < u->children->n; i++) ts[i] = Aget(u->children, i)->val;
	Tensor* res = u->fp(ts, i);
	free(ts);
	return res;
}

void Nfree(Node* u) {
	Afree(u->children);
	Afree(u->parents);
	free(u->children);
	free(u->parents);
	Tfree(u->val);
	Tfree(u->deriv);
	free(u->val);
	free(u->deriv);
	free(u);
}

Graph* Ginit() {
	Graph* g = malloc(sizeof(Graph));
	g->nodes = Ainit();
	return g;
}
void Gfree(Graph* g) {
	for (int i = 0; i < g->nodes->n; i++) {
		Nfree(Aget(g->nodes, i));
	}
	free(g);
}
void Gadd(Graph* g, Node* u) {
	Apb(g->nodes, u);
}
void Gforward(Graph* g) {
	A* q = Ainit();
	for (int i = 0; i < g->nodes->n; i++) {
		Node* u = Aget(g->nodes, i);
		if (u->children->n == 0) {
			Apb(q, u);
		}
		u->counter = u->children->n;
	}
	while (q->n > 0) {
		Node* u = Apopf(q);
		Neval(u);
		for (int i = 0; i < u->parents->n; i++) {
			Node* v = Aget(u->parents, i);
			v->counter--;
			if (v->counter == 0) {
				Apb(q, v);
			}
		}
	}
}
void Gbackward(Graph* g, Node* start) {
	assert(start->parents->n == 0);
	assert(start->val->dimlen == 0);
	for (int i = 0; i < g->nodes->n; i++) {
		Node* u = Aget(g->nodes, i);
		u->counter = u->parents->n;
	}
	A* q = Ainit();
	Apb(q, start);
	start->deriv->arr[0] = 1;
	while (q->n > 0) {
		Node* u = Apopf(q);
		for (int i = 0; i < u->children->n; i++) {
			Node* v = Aget(u->children, i);
			Tensor* D = Neval_D(u, i);
			Tensor* DD = Tcontract(u->deriv, D, u->val->dimlen);
			Tfree(D);
			Tprint(v->deriv);
			printf("\n");
			Tprint(DD);
			printf("\n");
			printf("\n");
			Tpadd(v->deriv, DD);
			Tfree(DD);
			v->counter--;
			if (v->counter == 0) {
				Apb(q, v);
			}
		}
	}
}

void test_graph() {
	// we'll do R^3 -> R^2 by a matmul and then from R^2 -> R^2 by exponentiating each element and then R^2 -> R by taking the sum of elements (dot with 1,1)
	Graph* g = Ginit();
	int dims1[] = {3};
	int dimsmat[] = {2, 3};
	int dims2[] = {2};
	Tensor* t1 = Tinit(dims1, 1);
	for (int i = 0; i < 3; i++) t1->arr[i] = i+1;
	Tensor* mat = Tinit(dimsmat, 2);
	mat->arr[0] = 5; mat->arr[1] = 2; mat->arr[2] = 1; mat->arr[3] = 3; mat->arr[4] = 4; mat->arr[5] = 2;
	Tensor* ones = Tinit(dims2, 1);
	ones->arr[0] = 1; ones->arr[1] = 1;

	Node* n1 = Ninit_val(t1, g);
	Node* nmat = Ninit_val(mat, g);
	Node* nones = Ninit_val(ones, g);
	Node* n2 = Ninit_matmul(nmat, n1, g);
	Node* n3 = Ninit_tizedop(n2, NULL, Texp, Texp_D, g);
	Node* n4 = Ninit_matmul(n3, nones, g);
	Gforward(g);
	printf("\n");
	printf("\n");
	printf("=== results === \n") ;
	Tprint(n2->val);
	printf("\n");
	printf("\n");
	Tprint(n3->val);
	printf("\n");
	printf("\n");
	Tprint(n4->val);
	printf("\n");
	printf("\n");

	printf("backward passing\n");
	Gbackward(g, n4);
	Tprint(nmat->deriv);
	printf("\n");
}

int main() {
	test_graph();
	return 0;
}
