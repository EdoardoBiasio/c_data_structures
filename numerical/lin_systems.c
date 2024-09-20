#include "numerical.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int vector_create_zeros(vector_t* v, int n) {
    v->n = n;
    v->data = calloc(sizeof(f64), n);
    return 0;
}

int dense_matrix_create_zeros(dense_matrix_t* A, int n, int m) {
    A->data = calloc(sizeof(f64), n * m);
    A->n = n;
    A->m = m;
    return 0;
}
int dense_matrix_create_identity(dense_matrix_t* A, int n, int m) {
    int min, i;
    A->data = calloc(sizeof(f64), n * m);
    A->n = n;
    A->m = m;
    min = (n < m) ? n : m;
    for (i = 0; i < min; i++)
	A->data[i * A->m + i] = 1;
    return 0;
}
int dense_matrix_switch_rows(dense_matrix_t* A, int i, int j) {
    int k;
    f64 temp;
    if (i > A->n || j > A->n)
	return -1;
    for (k = 0; k < A->m; k++) {
	temp = A->data[i * A->m + k];
	A->data[i * A->m + k] = A->data[j * A->m + k];
	A->data[j * A->m + k] = temp;
    }
    return 0;
}

int dense_matrix_switch_columns(dense_matrix_t* A, int i, int j) {
    int k;
    f64 temp;
    if (i > A->m || j > A->m)
	return -1;
    for (k = 0; k < A->n; k++) {
	temp = A->data[k * A->m + i];
	A->data[k * A->m + i] = A->data[k * A->m + j];
	A->data[k * A->m + j] = temp;
    }
    return 0;
}
void vector_close(vector_t* v) {
    if (!v)
	return;
    free(v->data);
}
void dense_matrix_print(dense_matrix_t* A) {
    int i, j;
    for (i = 0; i < A->n; i++) {
	for (j = 0; j < A->m; j++) {
	    printf("%f ", A->data[i * A->n + j]);
	}
	printf("\n");
    }
}
void vector_print(vector_t* v) {
    int i;
    for (i = 0; i < v->n; i++) {
	printf("%f ", v->data[i]);
    }
    printf("\n");
}
void dense_matrix_close(dense_matrix_t* A) {
    free(A->data);
}

int dense_matrix_LU_decomposition(dense_matrix_t* A, array_t* p) {
    int i, j, k, off, line, maxl;
    int n;
    f64 l, max, temp;
    if (A->n != A->m)
	return -1;
    n = A->n;
    array_i32_init(p, n);
    for (i = 0; i < n; i++)
	array_i32_push_back(p, i);
    for (i = 0; i < n; i++) {
	max = fabs(A->data[i * n + i]);
	maxl = i;
	for (line = i; line < n; line++)
	    if (fabs(A->data[line * n + i]) > max) {
		max = fabs(A->data[line * n + i]);
		maxl = line;
	    }
	if (f64_equal(max, 0))
	    continue;
	if (maxl != i) {
	    dense_matrix_switch_rows(A, i, maxl);
	    temp = array_i32_at(p, i);
	    *((int*) array_i32_at_ref(p, i)) = array_i32_at(p, maxl);
	    *((int*) array_i32_at_ref(p, maxl)) = temp;
	}
	
	for (j = i + 1; j < n; j++) {
	    l = A->data[j * n + i] / A->data[i * n + i];
	    A->data[j * n + i] = l;
	    for (k = i + 1; k < n; k++) {
		A->data[j * n + k] -= A->data[i * n + k] * l;
	    }
	}
    }

    return 0;
}

int dense_matrix_LU_decomposition_full_pivoting(dense_matrix_t* A, array_t* p, array_t* q) {
    int i, j, k, off, line, col, maxl, maxc;
    int n;
    f64 l, max, temp;
    if (A->n != A->m)
	return -1;
    n = A->n;
    array_i32_init(p, n);
    for (i = 0; i < n; i++)
	array_i32_push_back(p, i);
    array_i32_init(q, n);
    for (i = 0; i < n; i++)
	array_i32_push_back(q, i);
    for (i = 0; i < n; i++) {
	max = fabs(A->data[i * n + i]);
	maxl = i;
	maxc = i;
	for (line = i; line < n; line++)
	    for (col = i; col < n; col++)
		if (fabs(A->data[line * n + col]) > max) {
		    max = fabs(A->data[line * n + col]);
		    maxl = line;
		    maxc = col;
		}
	if (f64_equal(max, 0))
	    break;
	if (maxl != i) {
	    dense_matrix_switch_rows(A, i, maxl);
	    	temp = array_i32_at(p, i);
		*((int*) array_i32_at_ref(p, i)) = array_i32_at(p, maxl);
		*((int*) array_i32_at_ref(p, maxl)) = temp;
	}
	if (maxc != i) {
	    dense_matrix_switch_columns(A, i, maxc);
	    temp = array_i32_at(q, i);
	    *((int*) array_i32_at_ref(q, i)) = array_i32_at(q, maxc);
	    *((int*) array_i32_at_ref(q, maxc)) = temp;	    
	}

	for (j = i + 1; j < n; j++) {
	    l = A->data[j * n + i] / A->data[i * n + i];
	    A->data[j * n + i] = l;
	    for (k = i + 1; k < n; k++) {
		A->data[j * n + k] -= A->data[i * n + k] * l;
	    }
	}
    }

    return 0;
}
int dense_matrix_cholesky_factorization(dense_matrix_t* A) {

    int i, j, k;
    f64 sum;
    for (i = 0; i < A->n; i++) {
	sum = 0;
	for (k = 0; k < i; k++)
	    sum += (A->data[k * A->m + i]) * (A->data[k * A->m + i]);
	sum = A->data[i * A->m + i] - sum;
	if (sum < 0)
	    return -1;
	A->data[i * A->m + i] = sqrt(sum);
	for (j = 0; j < i; j++)
	    A->data[i * A->m + j] = 0;
	for (j = i + 1; j < A->m; j++) {
	    sum = 0;
	    for (k = 0; k < i; k++) {
		sum += A->data[k * A->m + i] * A->data[k * A->m + j];
	    }
	    sum = A->data[i * A->m + j] - sum;
	    A->data[i * A->m + j] = sum / A->data[i * A->m + i];
	}
    }
    return 0;
}

int dense_matrix_LU_solution(dense_matrix_t* LU, dense_matrix_t* P, vector_t* b, vector_t* x) {
    vector_t b1, x1;
    if (dense_matrix_multiply_vector(P, b, &b1) < 0) {
	printf("error\n");
	return 1;
    }
    dense_matrix_forward_substitution(LU, &b1, &x1);
    dense_matrix_backward_substitution(LU, &x1, x);
    vector_close(&b1);
    vector_close(&x1);
    return 0;
}
int dense_matrix_LU_solution_full_pivoting(dense_matrix_t* LU, dense_matrix_t* P, dense_matrix_t* Q, vector_t* b, vector_t* x) {
    vector_t b1, x1, x2;
    if (dense_matrix_multiply_vector(P, b, &b1) < 0) {
	printf("error\n");
	return 1;
    }
    dense_matrix_forward_substitution(LU, &b1, &x1);
    dense_matrix_backward_substitution(LU, &x1, &x2);
    dense_matrix_multiply_vector(Q, &x2, x);
    vector_close(&x2);
    vector_close(&b1);
    vector_close(&x1);
    return 0;
}

int dense_matrix_permutation_from_array(array_t* p, dense_matrix_t* P) {
    int n, i;
    if (!p)
	return -1;
    n = p->len;
    dense_matrix_create_zeros(P, n, n);
    for (i = 0; i < n; i++) {
	P->data[i * n + array_i32_at(p, i)] = 1;
    }
    return 0;
}

int dense_matrix_multiply_vector(dense_matrix_t* A, vector_t* b, vector_t* c) {
    int i, k;
    f64 tot;
    if (A->m != b->n)
	return -1;
    vector_create_zeros(c, A->n);
    for (i = 0; i < A->n; i++) {
	tot = 0;
	for (k = 0; k < A->m; k++)
	    tot += A->data[i * A->n + k] * b->data[k];
	c->data[i] = tot;
    }
    return 0;
}

int dense_matrix_multiply(dense_matrix_t* A, dense_matrix_t* B, dense_matrix_t* C) {
    int i, j, k;
    f64 tot;
    if (A->m != B->n)
	return -1;
    dense_matrix_create_zeros(C, A->n, B->m);
    for (i = 0; i < A->n; i++) {
	for (j = 0; j < B->m; j++) {
	    tot = 0;
	    for (k = 0; k < A->m; k++)
		tot += A->data[i * A->n + k] * B->data[k * B->n + j];
	    C->data[i * A->n + j] = tot;
	}
    }
    return 0;
}

int dense_matrix_forward_substitution(dense_matrix_t* L, vector_t* b, vector_t* x) {
    int i, j;
    f64 sum;
    x->n = L->n;
    x->data = calloc(sizeof(f64), x->n);
    for (i = 0; i < x->n; i++) {
	sum = 0;
	for (j = 0; j < i; j++)
	    sum += L->data[i * L->n + j] * x->data[j];
	x->data[i] = (b->data[i] - sum); /*/ L->data[i * L->n + i]; */
    }
    return 0;
}

int dense_matrix_backward_substitution(dense_matrix_t* U, vector_t* b, vector_t* x) {
    int i, j;
    f64 sum;
    x->n = U->n;
    x->data = calloc(sizeof(f64), x->n);
    for (i = x->n - 1; i >= 0; i--) {
	sum = 0;
	for (j = i; j < x->n; j++)
	    sum += U->data[i * U->n + j] * x->data[j];
	x->data[i] = (b->data[i] - sum) / U->data[i * U->n + i];
    }
    return 0;
}
int dense_matrix_strict_diagonal_dominance(dense_matrix_t* A, int type) {
    int i, j;
    f64 sum;
    if (type == DENSE_MATRIX_STRICT_ROW_DIAGONAL_DOMINANCE) {
	for (i = 0; i < A->n; i++) {
	    sum = 0;
	    for (j = 0; j < A->m; j++) {
		if (j == i)
		    continue;
		sum += fabs(A->data[i * A->m + j]);
	    }
	    if (sum >= fabs(A->data[i * A->m + i]))
		return -1;
	}
    } else {
	for (i = 0; i < A->m; i++) {
	    sum = 0;
	    for (j = 0; j < A->n; j++) {
		if (j == i)
		    continue;
		sum += fabs(A->data[j * A->m + i]);
	    }
	    if (sum >= fabs(A->data[i * A->m + i]))
		return -1;
	}	
    }
    return 1;
}

int dense_matrix_symmetric(dense_matrix_t* A) {
    int i, j;
    if (A->n != A->m)
	return -1;
    for (i = 0; i < A->n; i++)
	for (j = 0; j < A->n; j++)
	    if (A->data[i * A->n + j] != A->data[j * A->n + i])
		return -1;
    return 1;
}

int dense_matrix_positive_definite_s(dense_matrix_t* A) {
    dense_matrix_t copy;
    int res;
    copy.n = A->n;
    copy.m = A->m;
    copy.data = calloc(sizeof(f64), A->n * A->m);
    memcpy(copy.data, A->data, sizeof(f64) * A->n * A->m);
    res = (dense_matrix_cholesky_factorization(&copy) == 0) ? 1 : -1;
    dense_matrix_close(&copy);
    return res;
}

int dense_matrix_ndiagonal(dense_matrix_t* A, int n) {
    int i, j;
    for (i = 0; i < A->n; i++) {
	for (j = 0; j < A->m; j++) {
	    if (abs(i - j) < n - 1)
		continue;
	    if (!f64_equal(A->data[i * A->m + j], 0))
		return -1;
	}
    }
    return 1;
}

