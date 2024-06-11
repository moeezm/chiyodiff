#ifndef FUNCS_H
#define FUNCS_H

#include "tensor.h"

// non-scalar stuff

// matmul subsumes mat-mat, mat-vec, and vec-vec (dot product) multiplications
// is a shorthand for tensor contraction along the innermost dimension so can be used for those too
Tensor* matmul(Tensor* ts[]);
Tensor* matmul_D(Tensor* ts[], int i);

// scalar/coordinate wise operations
double Sadd(double a, double b);
double Sadd_D(double a, double b);
double Ssub(double a, double b);
double Ssub_D_A(double a, double b);
double Ssub_D_B(double a, double b);
double Smul(double a, double b);
double Smul_D_A(double a, double b);
double Smul_D_B(double a, double b);
double Sdiv(double a, double b);
double Sdiv_D_A(double a, double b);
double Sdiv_D_B(double a, double b);

double Sexp(double a);
double Sexp_D(double a);
double Slog(double a);
double Slog_D(double a);
double Ssin(double a);
double Ssin_D(double a);
double Scos(double a);
double Scos_D(double a);

// tensorized versions, to use in nodes
Tensor* Tadd(Tensor* ts[]);
Tensor* Tadd_D(Tensor* ts[], int i);
Tensor* Tsub(Tensor* ts[]);
Tensor* Tsub_D(Tensor* ts[], int i);
Tensor* Tmul(Tensor* ts[]);
Tensor* Tmul_D(Tensor* ts[], int i);
Tensor* Tdiv(Tensor* ts[]);
Tensor* Tdiv_D(Tensor* ts[], int i);
Tensor* Texp(Tensor* ts[]);
Tensor* Texp_D(Tensor* ts[], int i);
Tensor* Tlog(Tensor* ts[]);
Tensor* Tlog_D(Tensor* ts[], int i);
Tensor* Tsin(Tensor* ts[]);
Tensor* Tsin_D(Tensor* ts[], int i);
Tensor* Tcos(Tensor* ts[]);
Tensor* Tcos_D(Tensor* ts[], int i);
#endif
