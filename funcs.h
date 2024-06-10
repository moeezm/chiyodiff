#ifndef FUNCS_H
#define FUNCS_H

#include "tensor.h"

// lin alg stuff
Tensor* matmul(Tensor* mats[]) {
	Tensor* res = Tcontract(mats[0], mats[1]);


#endif
