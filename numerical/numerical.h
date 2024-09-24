#ifndef NUMERICAL_H
#define NUMERICAL_H

#include <math.h>
#include <float.h>

#include "../data_structures/ds.h"

typedef double f64;
typedef float f32;

typedef f64 (*function1x1)(f64);

/* FUNCTION ZEROS */
int function_zeros_newton_method_multiple_residue(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, int m, f64* result, int* niter);
int function_zeros_newton_method_multiple_increment(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, int m, f64* result, int* niter);
int function_zeros_newton_method_residue(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, f64* result, int* niter);
int function_zeros_newton_method_increment(function1x1 f, function1x1 df, f64 x0, int maxiter, f64 tol, f64* result, int* niter);
int function_zeros_chords_method_residue(function1x1 f, f64 a, f64 b, f64 x0, int maxiter, f64 tol, f64* result, int* niter);
int function_zeros_chords_method_increment(function1x1 f, f64 a, f64 b, f64 x0, int maxiter, f64 tol, f64* result, int* niter);
int function_zeros_secant_method_residue(function1x1 f, f64 x0, f64 x1, int maxiter, f64 tol, f64* result, int* niter);
int function_zeros_secant_method_increment(function1x1 f, f64 x0, f64 x1, int maxiter, f64 tol, f64* result, int* niter);

int f64_equal(f64 left, f64 right);

/* LINEAR SYSTEMS OF EQUATIONS */
typedef struct dense_matrix_s {
    int n, m;
    f64 *data;
} dense_matrix_t;

typedef struct vector_s {
    int n;
    f64 *data;
} vector_t;

int dense_matrix_create_zeros(dense_matrix_t* A, int n, int m);
int dense_matrix_create_identity(dense_matrix_t* A, int n, int m);
int vector_create_zeros(vector_t* v, int n);
void vector_close(vector_t* v);
void dense_matrix_close(dense_matrix_t* A);
void dense_matrix_print(dense_matrix_t* A);
void vector_print(vector_t* v);
int dense_matrix_cholesky_factorization(dense_matrix_t* A);
int dense_matrix_LU_decomposition(dense_matrix_t* A, array_t* p);
int dense_matrix_LU_decomposition_full_pivoting(dense_matrix_t* A, array_t* p, array_t* q);
int dense_matrix_LU_solution(dense_matrix_t* LU, dense_matrix_t* P, vector_t* b, vector_t* x);
int dense_matrix_LU_solution_full_pivoting(dense_matrix_t* LU, dense_matrix_t* P, dense_matrix_t* Q, vector_t* b, vector_t* x);
int dense_matrix_permutation_from_array(array_t* p, dense_matrix_t* P);
int dense_matrix_multiply(dense_matrix_t* A, dense_matrix_t* B, dense_matrix_t* C);
int dense_matrix_multiply_vector(dense_matrix_t* A, vector_t* b, vector_t* c);
int dense_matrix_forward_substitution(dense_matrix_t* L, vector_t* b, vector_t* x);
int dense_matrix_backward_substitution(dense_matrix_t* U, vector_t* b, vector_t* x);
int dense_matrix_svd(dense_matrix_t* A, f64 tol, int maxiter, dense_matrix_t* S, dense_matrix_t* U, dense_matrix_t* V);
int dense_matrix_svd_diagonalizable(dense_matrix_t* A, f64 tol, int maxiter, list_t* out, dense_matrix_t* U, dense_matrix_t* V);
f64 dense_matrix_condition(dense_matrix_t* A);

typedef struct {
    f64 svalue;
    vector_t v, u;
} singular_value_tuple_t;
enum {
    DENSE_MATRIX_STRICT_ROW_DIAGONAL_DOMINANCE,
    DENSE_MATRIX_STRICT_COLUMN_DIAGONAL_DOMINANCE
};
int dense_matrix_strict_diagonal_dominance(dense_matrix_t* A, int type);
int dense_matrix_symmetric(dense_matrix_t* A);
int dense_matrix_positive_definite_s(dense_matrix_t* A);
int dense_matrix_ndiagonal(dense_matrix_t* A, int n);

typedef struct sparse_matrix_coo_element_s {
    f64 value;
    int row, col;
} sparse_matrix_coo_element_t;

typedef struct sparse_matrix_coo_s {
    array_t elements;
    int n, m;
} sparse_matrix_coo_t;

int sparse_matrix_coo_create(sparse_matrix_coo_t* matrix, int n, int m);
int sparse_matrix_coo_insert(sparse_matrix_coo_t* matrix, f64 v, int row, int col);
void sparse_matrix_coo_print(sparse_matrix_coo_t* matrix);
int sparse_matrix_coo_close(sparse_matrix_coo_t* matrix);

typedef struct sparse_matrix_csc_s {
    f64 *elements;
    int n, m, nvalues;
    int *col_ptr, *row_idx;
} sparse_matrix_csc_t;

int sparse_matrix_csc_create_from_coo(sparse_matrix_csc_t* new_matrix, sparse_matrix_coo_t* old_matrix);
void sparse_matrix_csc_print(sparse_matrix_csc_t* matrix);
int sparse_matrix_csc_close(sparse_matrix_csc_t* matrix);
f64 sparse_matrix_csc_condition(sparse_matrix_csc_t* A);
#endif
