#include "tensor.h"
#include "funcs.h"
#include <stdlib.h>
#include <math.h>

Tensor* matmul(Tensor* mats[]) {
    return Tcontract(mats[0], mats[1], 1);
}

Tensor* matmul_D(Tensor* mats[], int i) {
    return Tcontract_D(mats[0], mats[1], 1, i==0);
}

double Sadd(double a, double b) {
    return a + b;
}

double Sadd_D(double a, double b) {
    return 1;
}

double Ssub(double a, double b) {
    return a - b;
}

double Ssub_D_A(double a, double b) {
    return 1;
}

double Ssub_D_B(double a, double b) {
    return -1;
}

double Smul(double a, double b) {
    return a * b;
}

double Smul_D_A(double a, double b) {
    return b;
}

double Smul_D_B(double a, double b) {
    return a;
}

double Sdiv(double a, double b) {
    return a / b;
}

double Sdiv_D_A(double a, double b) {
    return 1.0/b;
}

double Sdiv_D_B(double a, double b) {
    return -1*a/(b*b);
}

double Sexp(double a) {
    return exp(a);
}

double Sexp_D(double a) {
    return exp(a);
}

double Slog(double a) {
    return log(a);
}

double Slog_D(double a) {
    return 1/a;
}

double Ssin(double a) {
    return sin(a);
}

double Ssin_D(double a) {
    return cos(a);
}

double Scos(double a) {
    return cos(a);
}

double Scos_D(double a) {
    return -1*sin(a);
}

Tensor* Tadd(Tensor* ts[]) {
    return tensorize_binary(ts[0], ts[1], Sadd);
}

Tensor* Tadd_D(Tensor* ts[], int i) {
    return tensorize_binary(ts[0], ts[1], Sadd_D);
}

Tensor* Tsub(Tensor* ts[]) {
    return tensorize_binary(ts[0], ts[1], Ssub);
}

Tensor* Tsub_D(Tensor* ts[], int i) {
    if (i == 0) return tensorize_binary(ts[0], ts[1], Ssub_D_A);
    else return tensorize_binary(ts[0], ts[1], Ssub_D_B);
}

Tensor* Tmul(Tensor* ts[]) {
    return tensorize_binary(ts[0], ts[1], Smul);
}

Tensor* Tmul_D(Tensor* ts[], int i) {
    if (i == 0) return tensorize_binary(ts[0], ts[1], Smul_D_A);
    else return tensorize_binary(ts[0], ts[1], Smul_D_B);
}

Tensor* Tdiv(Tensor* ts[]) {
    return tensorize_binary(ts[0], ts[1], Sdiv);
}

Tensor* Tdiv_D(Tensor* ts[], int i) {
    if (i == 0) return tensorize_binary(ts[0], ts[1], Sdiv_D_A);
    else return tensorize_binary(ts[0], ts[1], Sdiv_D_B);
}

Tensor* Texp(Tensor* ts[]) {
    return tensorize_unary(ts[0], Sexp);
}

Tensor* Texp_D(Tensor* ts[], int i) {
    return tensorize_unary(ts[0], Sexp_D);
}

Tensor* Tlog(Tensor* ts[]) {
    return tensorize_unary(ts[0], Slog);
}

Tensor* Tlog_D(Tensor* ts[], int i) {
    return tensorize_unary(ts[0], Slog_D);
}

Tensor* Tsin(Tensor* ts[]) {
    return tensorize_unary(ts[0], Ssin);
}

Tensor* Tsin_D(Tensor* ts[], int i) {
    return tensorize_unary(ts[0], Ssin_D);
}

Tensor* Tcos(Tensor* ts[]) {
    return tensorize_unary(ts[0], Scos);
}

Tensor* Tcos_D(Tensor* ts[], int i) {
    return tensorize_unary(ts[0], Scos_D);
}
