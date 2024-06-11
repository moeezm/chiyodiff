#ifndef CHIYODIFF_H
#define CHIYODIFF_H

#include "A.h"
#include "tensor.h"

typedef struct Node {
	Tensor* (*f)(Tensor*[]);
	Tensor* (*fp)(Tensor*[], int);
	A* children;
	A* parents;
	Tensor* val;
	Tensor* deriv;
	int counter;
} Node;

typedef struct Graph {
	A* nodes;
} Graph;

// general init, better to use one of the wrappers
Node* Ninit(Node* child1, Node* child2, Tensor* (*f)(Tensor*[]), Tensor* (*fp)(Tensor*[], int i), int* dims, int dimlen, Graph* g);

// for input variables
Node* Ninit_val(Tensor* t, Graph* g);
// init for when the function is some tensorized scalar operation
Node* Ninit_tizedop(Node* child1, Node* child2, Tensor* (*f)(Tensor*[]), Tensor* (*fp)(Tensor*[], int i), Graph* g);
// for matmuls
Node* Ninit_matmul(Node* child1, Node* child2, Graph* g);
void Neval(Node* u);
Tensor* Neval_D(Node* u, int i);
void Nfree(Node* u);

Graph* Ginit();
void Gfree(Graph* g);
void Gadd(Graph* g, Node* u);
void Gforward(Graph* g);
void Gbackward(Graph* g, Node* start);

void test_graph();
#endif


