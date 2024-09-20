#include "numerical.h"
int function_zeros_newton_method_residue(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, f64* result, int* niter) {
    return function_zeros_newton_method_multiple_residue(f, df, x0, maxiter, tol, 1, result, niter);
}

int function_zeros_newton_method_increment(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, f64* result, int* niter) {
    return function_zeros_newton_method_multiple_increment(f, df, x0, maxiter, tol, 1, result, niter);
}
int function_zeros_newton_method_multiple_residue(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, int m,f64* result, int* niter) {
    int k = 0;
    f64 xk = x0;
    while (k <= maxiter && fabs(f(xk)) > tol) {
	xk = xk - m * f(xk) / df(xk);
	k++;
    }
    *result = xk;
    if (niter)
	*niter = k;
    if (k > maxiter)
	return -1;
    return 0;
}

int f64_equal(f64 left, f64 right) {
    return (fabs(left - right) <= DBL_MIN);
}

int function_zeros_newton_method_multiple_increment(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, int m, f64* result, int* niter) {
    int k = 0;
    f64 xk = x0, xprev = x0;
   
    do {
	if (f64_equal(df(xk), 0))
	    return -1;
	xprev = xk;
	xk = xk - m * f(xk) / df(xk);
	k++;
    } while (k <= maxiter && fabs(xk - xprev) > tol);
    *result = xk;
    if (niter)
	*niter = k;
    if (k > maxiter)
	return -1;
    return 0;
}

int function_zeros_chords_method_residue(function1x1 f, f64 a, f64 b, f64 x0, int maxiter, f64 tol, f64* result, int* niter) {
    int k = 0;
    f64 xk = x0;
    f64 q = (f(b) - f(a)) / (b - a);
    if (f64_equal(q, 0))
	return -1;
    while (k <= maxiter && fabs(f(xk)) > tol) {
	xk = xk - f(xk) / q;
	k++;
    }
    *result = xk;
    if (niter)
	*niter = k;
    if (k > maxiter)
	return -1;
    return 0;
}

int function_zeros_chords_method_increment(function1x1 f, f64 a, f64 b, f64 x0, int maxiter, f64 tol, f64* result, int* niter) {
    int k = 0;
    f64 xk = x0, xprev = x0;
    f64 q = (f(b) - f(a)) / (b - a);
    if (f64_equal(q, 0))
	return -1;
    do {
	xprev = xk;
	xk = xk - f(xk) / q;
	k++;
    } while (k <= maxiter && fabs(xk - xprev) > tol);
    *result = xk;
    if (niter)
	*niter = k;
    if (k > maxiter)
	return -1;
    return 0;
}
int function_zeros_secant_method_residue(function1x1 f, f64 x0, f64 x1, int maxiter, f64 tol, f64* result, int* niter) {
    int k = 0;
    f64 xk = x0;
    f64 xk1 = x1;
    f64 temp;
    while (k <= maxiter && fabs(f(xk1)) > tol) {
	temp = xk1;
	xk1 = xk1 - f(xk1) * (xk1 - xk) / (f(xk1) - f(xk));
	xk = temp;
	k++;
    }
    *result = xk1;
    if (niter)
	*niter = k;
    if (k > maxiter)
	return -1;
    return 0;
}

int function_zeros_secant_method_increment(function1x1 f, f64 x0, f64 x1, int maxiter, f64 tol, f64* result, int* niter) {
    int k = 0;
    f64 xk = x0;
    f64 xk1 = x1;
    f64 temp;
    while (k <= maxiter && fabs(xk1 - xk) > tol) {
	temp = xk1;
	xk1 = xk1 - f(xk1) * (xk1 - xk) / (f(xk1) - f(xk));
	xk = temp;
	k++;
    }
    *result = xk1;
    if (niter)
	*niter = k;
    if (k > maxiter)
	return -1;
    return 0;
}


