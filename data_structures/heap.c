#include "ds.h"
#include <stdlib.h>
#include <string.h>

int heap_init(heap_t* heap, size_t height, size_t type_size, int (*cmp)(void*, void*), int type) {
    if (!heap || !cmp)
	return -1;
    heap->height = height;
    heap->type_size = type_size;
    heap->cmp = cmp;
    heap->size = 0;
    heap->capacity = (1 << heap->height) - 1;
    heap->data = calloc(heap->type_size, heap->capacity);
    return 0;
}

int heap_close(heap_t* heap) {
    if (!heap)
	return -1;
    free(heap->data);
    heap->data = NULL;
    heap->height = heap->type_size = heap->size = heap->capacity = 0;
    heap->cmp = NULL;
    return 0;
}

void heap_realloc(heap_t* heap) {
    heap->height++;
    heap->capacity = (1 << heap->height) - 1;
    heap->data = realloc(heap->data, heap->type_size * heap->capacity);
}

void heap_swap(heap_t* heap, int i, int j, void* temp) {
    memcpy(temp, (char*) heap->data + i * heap->type_size, heap->type_size);
    memcpy((char*) heap->data + i * heap->type_size, (char*) heap->data + j * heap->type_size, heap->type_size);
    memcpy((char*) heap->data + j * heap->type_size, temp, heap->type_size);
}

int heap_insert(heap_t* heap, void* data, int type) {
    int i, p;
    void *temp;
    if (!heap || !data)
	return -1;
    if (heap->size >= heap->capacity)
	heap_realloc(heap);
    memcpy((char*) heap->data + heap->type_size * heap->size, data, heap->type_size);
    heap->size++;
    if (type == min_heap_type) {
	i = heap->size - 1;
	p = heap_parent(i);
	temp = malloc(heap->type_size);
	while (i > 0 && heap->cmp((char*) heap->data + i * heap->type_size, (char*) heap->data + p * heap->type_size) < 0) {
	    heap_swap(heap, i, p, temp);
	    i = p;
	    p = heap_parent(i);
	}
	free(temp);
    } else {
	i = heap->size - 1;
	p = heap_parent(i);
	temp = malloc(heap->type_size);
	while (i > 0 && heap->cmp((char*) heap->data + i * heap->type_size, (char*) heap->data + p * heap->type_size) > 0) {
	    heap_swap(heap, i, p, temp);
	    i = p;
	    p = heap_parent(i);
	}
	free(temp);
    }
    return 0;
}

void heapify(heap_t* heap, int idx, int type) {
    int l, r, res, s;
    void *temp;
    temp = malloc(heap->type_size);
    l = heap_left(idx);
    r = heap_right(idx);
    if (type == min_heap_type) {
	if (l < heap->size) {
	    res = heap->cmp((char*) heap->data + idx * heap->type_size, (char*) heap->data + l * heap->type_size);
	    if (res > 0)
		s = l;
	    else
		s = idx;
	    if (r < heap->size) {
		res = heap->cmp((char*) heap->data + s * heap->type_size, (char*) heap->data + r * heap->type_size);
		if (res > 0)
		    s = r;
	    }
	    if (s != idx) {
		heap_swap(heap, idx, s, temp);
		heapify(heap, s, type);
	    }
	}
    } else {
	if (l < heap->size) {
	    res = heap->cmp((char*) heap->data + idx * heap->type_size, (char*) heap->data + l * heap->type_size);
	    if (res < 0)
		s = l;
	    else
		s = idx;
	    if (r < heap->size) {
		res = heap->cmp((char*) heap->data + s * heap->type_size, (char*) heap->data + r * heap->type_size);
		if (res < 0)
		    s = r;
	    }
	    if (s != idx) {
		heap_swap(heap, idx, s, temp);
		heapify(heap, s, type);
	    }
	}
    }
    free(temp);
}

int heap_pop_root(heap_t* heap, void* out, int type) {
    if (!heap || heap->size < 1)
	return -1;
    if (out)
	memcpy(out, heap->data, heap->type_size);
    memcpy(heap->data, (char*) heap->data + (heap->size - 1) * heap->type_size, heap->type_size);
    heap->size--;
    heapify(heap, 0, type);
    return 0;
}

int heap_peek_root(heap_t* heap, void* out, int type) {
    if (!heap || !out || heap->size < 1)
	return -1;
    memcpy(out, heap->data, heap->type_size);
    return 0;
}

void* heap_peek_root_ref(heap_t* heap, int type) {
    if (!heap || heap->size < 1)
	return NULL;
    return heap->data;
}
int heap_init_from_carray_consume(heap_t* heap, void* array, int n, int type_size, int (*cmp)(void*, void*), int type) {
    int i, j;
    if (!array || !heap || !cmp || n < 0)
	return -1;
    i = 1;
    j = 1;
    while (i < n) {
	j++;
	i = (1 << j) - 1;
    }

    heap->type_size = type_size;
    heap->capacity = i;
    heap->height = j;
    heap->cmp = cmp;
    heap->size = n;
    heap->data = array;
    i = heap->size / 2;
    while (i >= 0) {
	heapify(heap, i, type);
	i--;
    }
    return 0;
}

int heap_init_from_carray(heap_t* heap, void* array, int n, int type_size, int (*cmp)(void*, void*), int type) {
    int i, j;
    if (!array || !heap || !cmp || n < 0)
	return -1;
    i = 1;
    j = 1;
    while (i < n) {
	j++;
	i = (1 << j) - 1;
    }
	
    heap_init(heap, j,type_size, cmp, type);
    memcpy(heap->data, array, type_size * n);
    i = heap->size / 2;
    while (i >= 0) {
	heapify(heap, i, type);
	i--;
    }
    return 0;
}

int heap_init_from_array_consume(heap_t* heap, array_t* array, int type) {
    return heap_init_from_carray_consume(heap, array->data, array->len, array->type_size, array->cmp, type);
}

int heap_init_from_array(heap_t* heap, array_t* array, int type) {
    return heap_init_from_carray(heap, array->data, array->len, array->type_size, array->cmp, type);
}

int array_heap_sort(array_t* array) {
    heap_t h;
    int i;
    void* temp;
    temp = malloc(array->type_size);
    heap_init_from_array_consume(&h, array, max_heap_type);
    i = h.size - 1;
    while (i > 0) {
	heap_swap(&h, 0, i, temp);
	i--;
    }
    return 0;
}

int heap_is_empty(heap_t* heap) {
    return heap->size == 0;
}

							
heap_type_implementation(char, i8)
heap_type_implementation(unsigned char, u8)
heap_type_implementation(short, i16)
heap_type_implementation(unsigned short, u16)
heap_type_implementation(int, i32)
heap_type_implementation(unsigned int, u32)
heap_type_implementation(long, i64)
heap_type_implementation(unsigned long, u64)
heap_type_implementation(char, char)
heap_type_implementation(float, f32)
heap_type_implementation(double, f64)

