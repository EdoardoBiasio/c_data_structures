#include "numerical/numerical.h"
#include "visualization/visualization.h"
#include <time.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

f64 quad(f64 x) {
    return 10 * tan(x)+ 50;
}

f64 logarithm(f64 x) {
    return 10 * log(x);
}

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
    sparse_matrix_coo_t smcoo;
    sparse_matrix_csc_t smcsc;
    list_t list;
    list_node_t *el;
    singular_value_tuple_t* tt;
    dense_matrix_t S, U, V;
    plot_2D_t plot;
    f64 zero;
    plot_2D_create(&plot, 500, 400);
    plot_2D_set_x_axis(&plot, 0, 10, 0.01, 0);
    plot_2D_set_y_axis(&plot, 0, 100, 1, 0);
    plot_2D_set_x_axis_label(&plot, "x");
    plot_2D_set_y_axis_label(&plot, "y");
    plot_2D_show_x_axis_limits(&plot);
    plot_2D_show_y_axis_limits(&plot);
    
    plot_2D_add_function(&plot, quad, 0);
    plot_2D_add_function(&plot, logarithm, 0x0010FF);
    plot_2D_add_function_legend(&plot, "tan(x)", "ln(x)");
    function_zeros_chords_method_residue(logarithm, 0.001, 10, 4.4, 100000000, 0.00001, &zero, NULL);
    printf("(%f, %f)\n", zero, logarithm(zero));
    plot_2D_add_point(&plot, zero, logarithm(zero), 0xFF0000);
    plot_2D_display(&plot);    
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
    srand(time(NULL));
    memcpy(B.data, bdata, 9 * sizeof(f64));
    memcpy(A.data, data, 16 * sizeof(f64));
    dense_matrix_svd(&A, 0.000001, 10000000, &S, &U, &V);
    printf("S: \n");
    dense_matrix_print(&S);
    printf("U: \n");
    dense_matrix_print(&U);
    printf("V: \n");
    dense_matrix_print(&V);

    printf("condition: %f\n", dense_matrix_condition(&A));
   
    printf("%d\n", dense_matrix_positive_definite_s(&B));
    dense_matrix_cholesky_factorization(&B);
    dense_matrix_print(&B);
    printf("\n");
    dense_matrix_LU_decomposition_full_pivoting(&A, &v, &w);
    dense_matrix_permutation_from_array(&v, &P);
    dense_matrix_permutation_from_array(&w, &Q);
    dense_matrix_LU_solution_full_pivoting(&A, &P, &Q, &b, &x2);
    vector_print(&x2);

    sparse_matrix_coo_create(&smcoo, 5, 4);
    sparse_matrix_coo_insert(&smcoo, 0.542, 2, 0);
    sparse_matrix_coo_insert(&smcoo, 2.532, 1, 3);
    sparse_matrix_coo_insert(&smcoo, -2.21, 3, 3);
    sparse_matrix_coo_insert(&smcoo, 0.23, 3, 0);
    sparse_matrix_coo_insert(&smcoo, 542.23, 4, 1);
    sparse_matrix_coo_insert(&smcoo, -431.0, 0, 2);
    sparse_matrix_coo_print(&smcoo);
    sparse_matrix_csc_create_from_coo(&smcsc, &smcoo);
    sparse_matrix_coo_close(&smcoo);
    sparse_matrix_csc_print(&smcsc);
    sparse_matrix_csc_close(&smcsc);

    return 0;
}
