#include "numerical.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
int main(int argc, char** argv) {
    dense_matrix_t A, Q, P, B;
    vector_t b, x1, x2, b1;
    array_t v, w;
    f64 bdata[9] = {
	2, -1, 0,
	-1, 2, -1,
	0, -1, 2
    };
    f64 data[16] = {
	5, 4, 0, 1,
	2, 6, -9, 4,
	-1, 5, 4, -2,
	5, 0, 1, 3 };
    A.n = 4;
    b.n = 4;
    b.data = calloc(sizeof(f64), 4);
    b.data[0] = 2;
    b.data[1] = 3;
    b.data[2] = -1;
    b.data[3] = -2;
    A.m = 4;
    A.data = calloc(sizeof(f64), 16);
    B.n = 3;
    B.m = 3;
    B.data = calloc(sizeof(f64), 9);
    memcpy(B.data, bdata, 9 * sizeof(f64));
    memcpy(A.data, data, 16 * sizeof(f64));
    printf("%d\n", dense_matrix_positive_definite_s(&B));
    dense_matrix_cholesky_factorization(&B);
    dense_matrix_print(&B);
    printf("\n");
    dense_matrix_LU_decomposition_full_pivoting(&A, &v, &w);
    dense_matrix_permutation_from_array(&v, &P);
    dense_matrix_permutation_from_array(&w, &Q);
    dense_matrix_LU_solution_full_pivoting(&A, &P, &Q, &b, &x2);
    vector_print(&x2);
    return 0;
}
