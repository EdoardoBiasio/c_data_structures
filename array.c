#include "clib.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

int array_init(array_t* array, int type_size, int start_capacity, int (*cmp)(void*, void*)) {
    if (!array)
        return -1;
    array->type_size = type_size;
    array->capacity = start_capacity;
    array->len = 0;
    array->data = calloc(type_size, array->capacity);
    array->cmp = cmp;
    return 0;
}

int array_close(array_t* array) {
    if (!array)
        return -1;
    array->type_size = array->capacity = array->len = 0;
    free(array->data);
    array->data = NULL;
    return 0;
}

int array_insert(array_t* array, int idx, void* element) {
    void *temp;
    if (!array)
        return -1;
    array->len++;
    if (array->len >= array->capacity) {
        array->capacity *= 2;
        array->data = realloc(array->data, array->type_size * array->capacity);
    }
    if (idx >= array->len - 1) {
        memcpy(((char*) array->data) + (array->len - 1) * array->type_size, element, array->type_size);
    } else {
        temp = calloc(array->type_size, array->capacity);
        memcpy(temp, array->data, array->type_size * idx);
        memcpy(((char*)temp) + idx * array->type_size, element, array->type_size);
        memcpy(((char*)temp) + (idx + 1) * array->type_size, ((char*) array->data) + idx * array->type_size, array->type_size * (array->len - idx - 1));
        free(array->data);
        array->data = temp;
    }
    return 0; 
}

int array_at(array_t* array, int idx, void* out) {
    if (!array || idx >= array->len || !out)
        return -1;
    memcpy(out, ((char*)array->data) + idx * array->type_size, array->type_size);
    return 0;
}

void* array_at_ref(array_t* array, int idx) {
    if (!array || idx >= array->len || idx < 0)
	return NULL;
    return ((char*) array->data) + idx * array->type_size;
}

int array_delete(array_t* array, void* element) {
    size_t i;
    if (!array || !element)
        return -1;
    for (i = 0; i < array->len; i++) {
        if(memcmp((char*)array->data + i * array->type_size, element, array->type_size) == 0) {
            array_delete_index(array, i);
            return 0;
        }
    }
    return -2; 
}

int array_delete_index(array_t* array, int idx) {
    size_t i;
    if (!array || idx >= array->len)
        return -1;
    for (i = idx; i < array->len - 1; i++) {
        memcpy((char*)array->data + i * array->type_size, (char*) array->data + (i + 1) * array->type_size, array->type_size);
    }
    array->len--;
    return 0;
}

int array_print(array_t* array, void(*print)(void*)) {
    size_t i;
    if (!array || !print)
        return -1;
    print(array->data);
    for (i = 1; i < array->len; i++) {
        printf(", ");
        print((void*)((char*)array->data + i * array->type_size));
        
    }
    printf("\n");
    return 0;
}

int array_search(array_t* array, void* key, void* out) {
    size_t i;
    if (!array || !key)
	return -1;
    for (i = 0; i < array->len; i++) {
	if (array->cmp(key, (char*)array->data + i * array->type_size) == 0) {
	    if (out)
		memcpy(out, (char*) array->data + i * array->type_size, array->type_size);
	    return i;
	}
    }
    return -1;
}

int array_min(array_t* array, void* out) {
    size_t i, imin;
    if (!array)
	return -1;
    imin = 0;
    for (i = 0; i < array->len; i++) {
	if (array->cmp((char*) array->data + i * array->type_size, (char*) array->data + imin * array->type_size) < 0)
	    imin = i;
    }
    if (out)
	memcpy(out, (char*) array->data + imin * array->type_size, array->type_size);
    return imin;
}

int array_max(array_t* array, void* out) {
    size_t i, imax;
    if (!array)
	return -1;
    imax = 0;
    for (i = 0; i < array->len; i++) {
	if (array->cmp((char*) array->data + i * array->type_size, (char*) array->data + imax * array->type_size) > 0)
	    imax = i;
    }
    if (out)
	memcpy(out, (char*) array->data + imax * array->type_size, array->type_size);
    return imax;
}

int array_successor(array_t* array, int element, void *out) {
    size_t i, succ;
    if (!array || element >= array->len || element < 0)
	return -1;
    for (i = 0; i < array->len; i++) {
	if (i == element)
	    continue;
	if (array->cmp((char*) array->data + i * array->type_size, (char*) array->data + element * array->type_size) > 0) {
	    succ = i;
	    break;
	}
    }
    for (; i < array->len; i++) {
	if (i == element)
	    continue;
	if (array->cmp((char*) array->data + i * array->type_size, (char*) array->data + element * array->type_size) > 0 && array->cmp((char*) array->data + i * array->type_size, (char*) array->data + succ * array->type_size) < 0) {
	    succ = i;
	}
    }
    if (out)
	memcpy(out, (char*) array->data + succ * array->type_size, array->type_size);
    return succ;
}

int array_predecessor(array_t* array, int element, void *out) {
    size_t i, prev;
    if (!array || element >= array->len || element < 0)
	return -1;
    for (i = 0; i < array->len; i++) {
	if (i == element)
	    continue;
	if (array->cmp((char*) array->data + i * array->type_size, (char*) array->data + element * array->type_size) < 0) {
	    prev = i;
	    break;
	}
    }
    for (; i < array->len; i++) {
	if (i == element)
	    continue;
	if (array->cmp((char*) array->data + i * array->type_size, (char*) array->data + element * array->type_size) < 0 && array->cmp((char*) array->data + i * array->type_size, (char*) array->data + prev * array->type_size) > 0) {
	    prev = i;
	}
    }
    if (out)
	memcpy(out, (char*) array->data + prev * array->type_size, array->type_size);
    return prev;
}

int array_copy(array_t* src, array_t* dest) {
    if (!src || !dest)
	return -1;
    dest->data = calloc(src->type_size, src->capacity);
    memcpy(dest->data, src->data, src->type_size * src->capacity);
    dest->type_size = src->type_size;
    dest->capacity = src->capacity;
    dest->len = src->len;
    dest->cmp = src->cmp;
    return 0;
}


array_type_implementation(char, i8)
array_type_implementation(unsigned char, u8)
array_type_implementation(short, i16)
array_type_implementation(unsigned short, u16)
array_type_implementation(int, i32)
array_type_implementation(unsigned int, u32)
array_type_implementation(long, i64)
array_type_implementation(unsigned long, u64)
array_type_implementation(char, char)
array_type_implementation(float, f32)
array_type_implementation(double, f64)
