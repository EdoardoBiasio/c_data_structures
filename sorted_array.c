#include "clib.h"
#include <string.h>

int sorted_array_insert(sorted_array_t* array, void* element) {
    int r, l, i,  j;
    int res;
    if (!array || !element)
	return -1;
    l = 0;
    r = array->len - 1;
    if (array->len == 0) {
	array_insert((array_t*) array, 0, element);
	return 0;
    }
    res = array->cmp(element, array->data);
    if (res < 0) {
	array_insert((array_t*)array, 0, element);
	return 0;
    }
    res = array->cmp(element, (char*) array->data + r * array->type_size);
    if (res > 0) {
	array_insert((array_t*)array, array->len, element);
	return 0;
    }
    
    while (l <= r && l >= 0 && r < array->len) {
	i = (l + r) / 2;
	res = array->cmp(element, (char*) array->data + i * array->type_size);
	if (res == 0) {
	    j = i;
	    while (j < array->len && array->cmp(element, (char*) array->data + j * array->type_size) == 0)
		j++;
	    array_insert((array_t*) array, j - 1, element);
	    return 0;
	}
	if (res > 0)
	    l = i+ 1;
	else
	    r = i - 1;
    }

    array_insert((array_t*)array, l, element);
    return 0;
}

int sorted_array_bsearch(sorted_array_t* array, void* key, void* out) {
    size_t  r, l, i;
    int res;
    l = 0;

    r = array->len - 1;
    while (l <= r) {
	i = (l + r) / 2;
	res = array->cmp(key, (char*) array->data + i * array->type_size);
	if (res == 0) {
	    memcpy(out, (char*) array->data + i * array->type_size, array->type_size);
	    return 0;
	}
	if (res > 0)
	    l = i + 1;
	else
	    r = i - 1;
    }
    return -1;
}

int sorted_array_from_array(sorted_array_t* dest, array_t* src) {
    
    size_t i;
    if (!dest || !src)
	return -1;
    sorted_array_init(dest, src->type_size, src->len, src->cmp);
    for (i = 0; i < src->len; i++) {
	sorted_array_insert(dest, (char*) src->data + i * src->type_size);
    }
    return 0;
}

sorted_array_type_implementation(char, i8)
sorted_array_type_implementation(unsigned char, u8)
sorted_array_type_implementation(short, i16)
sorted_array_type_implementation(unsigned short, u16)
sorted_array_type_implementation(int, i32)
sorted_array_type_implementation(unsigned int, u32)
sorted_array_type_implementation(long, i64)
sorted_array_type_implementation(unsigned long, u64)
sorted_array_type_implementation(char, char)
sorted_array_type_implementation(float, f32)
sorted_array_type_implementation(double, f64)

