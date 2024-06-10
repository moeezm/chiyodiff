#include "A.h"
#include "tensor.h"
#include <stdlib.h>
#include <stdbool.h>
#include <stdio.h>
#include <assert.h>
#include <stdarg.h>
#include <math.h>

// ==== COMP GRAPH ====
typedef struct {
	// function and its derivative
	Tensor* (*f)(int, Tensor*[]);
	Tensor* (*fp)(int, int, Tensor*[]);
} Node;
int main() {
	test_tensor();
	return 0;
}
