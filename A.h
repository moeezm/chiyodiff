#ifndef A_H
#define A_H

typedef struct Node Node;

typedef struct {
	Node** arr;
	int sz;
	int n;
	int front;
} A;

A* Ainit();
Node* Aget(A* a, int i);
void Aset(A* a, int i, Node* x);
void Apb(A* a, Node* x);
Node* Afront(A* a);
Node* Apopf(A* a);
void Afree(A* a);

#endif
