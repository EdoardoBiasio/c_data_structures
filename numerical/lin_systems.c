#include "numerical.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>

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

f64 vector_norm(vector_t* v) {
    int i;
    f64 norm = 0;
    for (i = 0; i < v->n; i++)
	norm += fabs(v->data[i]) * fabs(v->data[i]);
    return sqrt(norm);
}

int vector_normalize(vector_t* v) {
    int i;
    f64 norm = vector_norm(v);
    for (i = 0; i < v->n; i++)
	v->data[i] /= norm;
    return 0;
}

int vector_copy(vector_t* dest, vector_t* src) {
    if (dest->n != src->n)
	dest->data = realloc(dest->data, sizeof(f64) * src->n);
    dest->n = src->n;
    memcpy(dest->data, src->data, sizeof(f64) * src->n);
    return 0;
}

int vector_subtract(vector_t* lhs, vector_t* rhs, vector_t* dest) {
    int i;
    if (!dest->data)
	vector_create_zeros(dest, lhs->n);
    for (i = 0; i < lhs->n; i++)
	dest->data[i] = lhs->data[i] - rhs->data[i];
    return 0;
}

f64 eigen_value(dense_matrix_t* A, vector_t* v) {
    vector_t Av;
    int i;
    f64 sum;
    vector_create_zeros(&Av, A->n);
    dense_matrix_multiply_vector(A, v, &Av);
    for (i = 0; i < A->n; i++) {
	sum += Av.data[i] / v->data[i]; 
    }
    return sum / A->n;
}

f64 dense_matrix_svd_power_iteration(dense_matrix_t* A, vector_t* b0, f64 tol, int maxiter, int* niter, vector_t* v) {
    int i;
    vector_t b, temp;
    f64 oldsv, sv;
    i = 0;
    temp.data = NULL;
    vector_create_zeros(&b, A->n);
    vector_create_zeros(&temp, A->n);
    if (!v->data)
	vector_create_zeros(v, A->n);
    dense_matrix_multiply_vector(A, b0, &b);
    vector_normalize(&b);
    sv = eigen_value(A, &b);
    do {
	oldsv =sv;
	dense_matrix_multiply_vector(A, &b, &temp);
	vector_copy(&b, &temp);
	vector_normalize(&b);
	sv = eigen_value(A, &b);
	
	i++;
    } while (fabs(sv - oldsv) > tol && i < maxiter);
    if (niter)
	*niter = i;
    printf("%d\n", i);
    vector_copy(v, &b);
    vector_close(&b);
    if (i >= maxiter)
	return -1;


    return sv;
}

void vector_randomize(vector_t* v) {
    int i;
    for (i = 0; i < v->n; i++) {
	v->data[i] = ((f64) rand() - INT_MAX / 2) * 2.0f / INT_MAX;
    }
}
void dense_matrix_multiply_constant(dense_matrix_t* A, f64 t) {
    int i, j;
    for (i = 0; i < A->n; i++)
	for (j = 0; j < A->m; j++)
	    A->data[i * A->m + j] *= t;
}

void dense_matrix_subtract(dense_matrix_t* dest, dense_matrix_t* src) {
    int i, j;
    for (i = 0; i < dest->n; i++)
	for (j = 0; j < dest->m; j++)
	    dest->data[i * dest->m + j] -= src->data[i * dest->m + j];
}

void vector_outer_product(vector_t* lhs, vector_t* rhs, dense_matrix_t* out)  {
    int i, j;
    if (!out->data)
	dense_matrix_create_zeros(out, lhs->n, rhs->n);
    for (i = 0; i < lhs->n; i++) {
	for (j = 0; j < rhs->n; j++) {
	    out->data[i * rhs->n + j] = lhs->data[i] * rhs->data[j];
	}
    }
}
void dense_matrix_copy(dense_matrix_t* dest, dense_matrix_t* src) {
   int i, j;
    for (i = 0; i < dest->n; i++)
	for (j = 0; j < dest->m; j++)
	    dest->data[i * dest->m + j] = src->data[i * dest->m + j];
}
								 

/*    builtin.com/articles/svd-algorithm */

int dense_matrix_transpose(dense_matrix_t* A) {
    int i, j;
    f64 temp;
    for (i = 0; i < A->n; i++) {
	for (j = i; j < A->m; j++) {
	    temp = A->data[i * A->m + j];
	    A->data[i * A->m + j] = A->data[j * A->m + i];
	    A->data[j * A->m + i] = temp;
	}
    }
    return 0;
}
int dense_matrix_svd(dense_matrix_t* A, f64 tol, int maxiter, dense_matrix_t* S, dense_matrix_t* U, dense_matrix_t* V) {
    dense_matrix_t At, B;
    int i;
    list_t list;
    list_node_t *t;
    singular_value_tuple_t *tt;
    dense_matrix_create_zeros(U, A->n, A->m);
    dense_matrix_create_zeros(V, A->n, A->m);
    dense_matrix_create_zeros(S, A->n, A->m);
    dense_matrix_create_zeros(&At, A->n, A->m);
    dense_matrix_copy(&At, A);
    dense_matrix_transpose(&At);
    dense_matrix_create_zeros(&B, A->n, A->m);
    dense_matrix_multiply(&At, A, &B);
    dense_matrix_svd_diagonalizable(&B, tol, maxiter, &list, V, NULL);
    dense_matrix_multiply(A, V, &B);
    dense_matrix_transpose(&B);
    t = list.head;
    i = 0;
    dense_matrix_close(&At);
    dense_matrix_create_zeros(&At, A->n, A->m);
    while (t) {
	
	tt = (singular_value_tuple_t*) list_node_data(t);
	S->data[i * S->m + i] = sqrt(tt->svalue);
	At.data[i * S->m + i] = 1.0f / sqrt(tt->svalue);
	t = t->next;
	i++;
    }
    dense_matrix_multiply(&At, &B, U);
    dense_matrix_transpose(U);
    dense_matrix_transpose(V);
    return 0;
}

int singular_value_tuple_cmp(void* lhs, void* rhs) {
    singular_value_tuple_t *L = lhs, *R = rhs;
    return L->svalue - R->svalue;
}

int dense_matrix_svd_diagonalizable(dense_matrix_t* A, f64 tol, int maxiter, list_t* out, dense_matrix_t* U, dense_matrix_t* V) {
    int k, i, j;
    dense_matrix_t temp, mout;
    list_t values;
    list_node_t* t;
    dense_matrix_t At;
    singular_value_tuple_t *tt, tnew;
    
    vector_t b, v, u;
    dense_matrix_create_zeros(&At, A->n, A->m);
    dense_matrix_create_zeros(&temp, A->n, A->m);
    dense_matrix_copy(&At, A);
    dense_matrix_transpose(&At);

    dense_matrix_multiply(A, &At, &temp);
    dense_matrix_copy(A, &temp);

    vector_create_zeros(&b, A->n);
    vector_create_zeros(&u, A->n);
    list_init(&values, sizeof(singular_value_tuple_t), singular_value_tuple_cmp);

    dense_matrix_create_zeros(&mout, A->n, A->m);
    k = (A->n < A->m) ? A->n : A->m;
    for (i = 0; i < k; i++) {
	dense_matrix_copy(&temp, A);
	if (values.head != NULL) {
	    t = values.head;
	    while (t) {
		tt = (singular_value_tuple_t*) list_node_data(t);
		vector_outer_product(&tt->u, &tt->v, &mout);
		dense_matrix_multiply_constant(&mout, tt->svalue);
		dense_matrix_subtract(&temp, &mout);
		t = t->next;
	    }
	}
	vector_randomize(&b);
	dense_matrix_svd_power_iteration(&temp, &b, tol, maxiter, NULL, &u);
	vector_create_zeros(&tnew.u, A->n);
	vector_create_zeros(&tnew.v, A->n);
	vector_copy(&tnew.u, &u);
	dense_matrix_transpose(A);
	dense_matrix_multiply_vector(A, &u, &v);
	dense_matrix_transpose(A);
	tnew.svalue = vector_norm(&v);
	vector_normalize(&v);
	vector_copy(&tnew.v, &v);
	list_push_back(&values, &tnew);
    }
    
    *out = values;
    i = 0;
    t = values.head;
    while (t && i < k) {
	tt = (singular_value_tuple_t*) list_node_data(t);
	tt->svalue = sqrt(tt->svalue);
	if (U)
	    for (j = 0; j < tt->u.n; j++)
		U->data[j * A->m + i] = tt->u.data[j];
	if (V)
	    for (j = 0; j < tt->v.n; j++)
		V->data[i * A->m + j] = tt->v.data[j];
	t = t->next;
	i++;
    }
    return 0;
}
f64 dense_matrix_condition(dense_matrix_t* A) {
dense_matrix_t At, B;
    int i;
    list_t list;
    singular_value_tuple_t tmin, tmax;
    f64 res;
    dense_matrix_create_zeros(&At, A->n, A->m);
    dense_matrix_copy(&At, A);
    dense_matrix_transpose(&At);
    dense_matrix_create_zeros(&B, A->n, A->m);
    dense_matrix_multiply(&At, A, &B);
    dense_matrix_svd_diagonalizable(&B, 1e-10, 1000000, &list, NULL, NULL);

    list_min(&list, &tmin);
    list_max(&list, &tmax);
    res = sqrt(tmax.svalue / tmin.svalue);
    return res;    
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

int sparse_matrix_coo_element_cmp(void* left, void* right) {
    sparse_matrix_coo_element_t* L, *R;
    L = left; R = right;
    if (L->col == R->col)
	return L->row - R->row;
    return L->col - R->col;
}

int sparse_matrix_coo_create(sparse_matrix_coo_t* matrix, int n, int m) {
    if (!matrix)
	return -1;
    if (array_init(&matrix->elements, sizeof(sparse_matrix_coo_element_t), 4, sparse_matrix_coo_element_cmp) < 0)
	return -1;
    matrix->n = n;
    matrix->m = m;
    return 0;
}

int sparse_matrix_coo_insert(sparse_matrix_coo_t* matrix, f64 v, int row, int col) {
    sparse_matrix_coo_element_t new;
    if (!matrix)
	return -1;
    new.value = v;
    new.row = row; new.col = col;
    array_push_back(&matrix->elements, &new);
    return 0;
}
void sparse_matrix_coo_print(sparse_matrix_coo_t* matrix) {
    int i = 0;
    sparse_matrix_coo_element_t temp;
    if (matrix) {

	for (i = 0; i < matrix->elements.len; i++) {
	    array_at(&matrix->elements, i, &temp);
	    printf("(%d, %d, v: %f)\n", temp.row, temp.col, temp.value);
	}
    }
}

int sparse_matrix_coo_close(sparse_matrix_coo_t* matrix) {
    if (!matrix)
	return -1;
    array_close(&matrix->elements);
    return 0;
}
int sparse_matrix_csc_create_from_coo(sparse_matrix_csc_t* new_matrix, sparse_matrix_coo_t* old_matrix) {
    int i, j;
    int old_col;
    sparse_matrix_coo_element_t temp;
    if (!new_matrix || !old_matrix)
	return -1;
    array_quicksort(&old_matrix->elements, 0, old_matrix->elements.len - 1);
    new_matrix->row_idx = calloc(sizeof(int), old_matrix->elements.len);
    new_matrix->col_ptr = calloc(sizeof(int), old_matrix->m + 1);
    new_matrix->elements = calloc(sizeof(f64), old_matrix->elements.len);
    new_matrix->m = old_matrix->m; new_matrix->n = old_matrix->n;
    new_matrix->nvalues = old_matrix->elements.len;
    old_col = -1;
    j = 0;
    for (i = 0; i < old_matrix->elements.len; i++) {
	array_at(&old_matrix->elements, i, &temp);
	if (temp.col > old_col) {
	    while (temp.col > old_col) {
		new_matrix->col_ptr[j] = i;
		old_col++;
		j++;
	    }
	}
	new_matrix->elements[i] = temp.value;
	new_matrix->row_idx[i] = temp.row;
    }
    new_matrix->col_ptr[j] = old_matrix->m + 1;
    return 0;
}
void sparse_matrix_csc_print(sparse_matrix_csc_t* matrix) {
    int i;
    printf("values: ");
    for (i = 0; i < matrix->nvalues; i++) {
	printf("%f ", matrix->elements[i]);
    }
    printf("\nrow indices: ");
    for (i = 0; i < matrix->nvalues; i++) {
	printf("%d ", matrix->row_idx[i]);
    }
    printf("\ncolumn ptrs: ");
    for (i = 0; i < matrix->m + 1; i++)
	printf("%d ", matrix->col_ptr[i]);
    printf("\n");
}
int sparse_matrix_csc_close(sparse_matrix_csc_t* matrix) {
    if (!matrix)
	return -1;
    free(matrix->elements);
    free(matrix->row_idx);
    free(matrix->col_ptr);
    matrix->n = matrix->m = matrix->nvalues = 0;
    return 0;
}

