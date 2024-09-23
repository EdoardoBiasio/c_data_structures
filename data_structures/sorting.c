#include "ds.h"
#include <string.h>
#include <stdlib.h>

void array_switch(array_t* array, int i, int j, void* temp) {
    memcpy(temp, (char*) array->data + j * array->type_size, array->type_size);
    memcpy((char*) array->data + j * array->type_size, (char*) array->data + i * array->type_size, array->type_size);
    memcpy((char*) array->data + i * array->type_size, temp, array->type_size);
}

int partition_lomuto(array_t* array, size_t lo, size_t hi) {
    int i, j;
    void* pivot,* temp;
    temp = malloc(array->type_size);
    pivot = (void*)((char*) array->data + hi * array->type_size);
    i = lo - 1;
    for (j = lo; j < hi; j++) {
	if (array->cmp((char*) array->data + j * array->type_size, pivot) <= 0) {
	    i++;
	    array_switch(array, i, j, temp);
	}
    }
    array_switch(array, i + 1, hi, temp);
    free(temp);
    return i + 1;
}

int partition_hoare(array_t* array, size_t lo, size_t hi) {
    int i, j;
    void *pivot, *temp;
    temp = malloc(array->type_size);
    pivot = (void*)((char*) array->data + lo * array->type_size);
    i = lo - 1;
    j = hi + 1;
    while (1) {
	do {
	    j--;
	} while (array->cmp((char*) array->data + j * array->type_size, pivot) > 0);
	do {
	    i++;
	} while (array->cmp((char*) array->data + i * array->type_size, pivot) < 0);
	if (i < j)
	    array_switch(array, i, j, temp);
	else {
	    free(temp);
	    return j;
	}
    }
}
#define QUICKSORT_HOARE
int array_quicksort(array_t* array, size_t lo, size_t hi) {
    int p;
    if (lo < hi) {
#ifdef QUICKSORT_LOMUTO
	p = partition_lomuto(array, lo, hi);
	array_quicksort(array, lo, p - 1);
	array_quicksort(array, p + 1, hi);
#else
	p = partition_hoare(array, lo, hi);
	array_quicksort(array, lo, p);
	array_quicksort(array, p + 1, hi);
#endif
    }
    return 0;
}

int array_insertionsort(array_t* array) {
    int i, j;
    void *temp;
    temp = malloc(array->type_size);
    for (i = 1; i < array->len; i++) {
	memcpy(temp, (char*) array->data + i * array->type_size, array->type_size);
	j = i - 1;
	while (j >= 0 && array->cmp((char*) array->data + j * array->type_size, temp) > 0) {
	    memcpy((char*) array->data + (j + 1) * array->type_size, (char*) array->data + j * array->type_size, array->type_size);
	    j--;
	}
	memcpy((char*) array->data + (j + 1) * array->type_size, temp, array->type_size);
    }
    free(temp);
    return 0;
}

int merge(array_t* array, int p, int q, int r) {
    int len1, len2, i, j, k;
    void *L, *R;
    len1 = q - p + 1; len2 = r - q;
    L = calloc(array->type_size, len1);
    R = calloc(array->type_size, len2);
    for (i = 0; i < len1; i++)
	memcpy((char*) L + i * array->type_size, (char*) array->data + (p + i) * array->type_size, array->type_size);
    for (i = 0; i < len2; i++)
	memcpy((char*) R + i * array->type_size, (char*) array->data + (q + 1 + i) * array->type_size, array->type_size);
    k = p;
    i = j = 0;
    while (i < len1 && j < len2 && k <= r) {
	if (array->cmp((char*) L + i * array->type_size, (char*) R + j * array->type_size) > 0) {
	    memcpy((char*) array->data + k * array->type_size, (char*) R + j * array->type_size, array->type_size);
	    j++;
	} else {
	    memcpy((char*) array->data + k * array->type_size, (char*) L + i * array->type_size, array->type_size);
	    i++;
	}
	k++;
    }
    
    while (i < len1) {
	memcpy((char*) array->data + k * array->type_size, (char*) L + i * array->type_size, array->type_size);
	i++;
	k++;
    }
    while (j < len2) {
	memcpy((char*) array->data + k * array->type_size, (char*) R + j * array->type_size, array->type_size);
	j++;
	k++;
    }
    free(L);
    free(R);
    if (k < p)
	printf("error!\n");
    
    return 0;
}

int array_mergesort(array_t* array, int p, int r) {
    int q;
    void *temp;
    temp = malloc(array->type_size);
    if (p < r - 1) {
	q = (p + r) / 2;
	array_mergesort(array, p, q);
	array_mergesort(array, q + 1, r);
	merge(array, p, q, r);
    } else if (array->cmp((char*) array->data + p * array->type_size, (char*) array->data + r * array->type_size) > 0) {
	array_switch(array, r, p, temp);

    }

    free(temp);
    return 0;
}
int array_selectionsort(array_t* array) {
    int i, j, imin;
    void *temp;
    temp = malloc(array->type_size);
    for (i = 0; i < array->len - 1; i++) {
	j = i + 1;
	imin = i;
	while (j < array->len) {
	    if (array->cmp((char*) array->data + j * array->type_size, (char*) array->data + imin * array->type_size) < 0)
		imin = j;
	    j++;
	}
	array_switch(array, i, imin, temp);
    }
    free(temp);
    return 0;
}
