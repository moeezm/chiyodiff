struct typedef {
	double *arr;
	int sz;
	int n;
	int front;
} A;

void A_init(A* a);
double Aget(A* a, int i);
void Aset(A* a, int i, double x);
void Apb(A* a, double x);
double Afront(A* a);
double Apopf(A* a);
void Aclean(A* a);
