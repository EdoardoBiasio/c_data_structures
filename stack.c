#include "clib.h"
#include <string.h>

int lstack_init(lstack_t* stack, int type_size, int (*cmp)(void*, void*)) {
    if (!stack)
	return -1;
    list_init(&stack->elements, type_size, cmp);
    return 0;
}

int lstack_close(lstack_t* stack) {
    if (!stack)
	return -1;
    list_close(&stack->elements);
    return 0;
}

int lstack_is_empty(lstack_t* stack) {
    if (!stack)
	return -1;
    return list_is_empty(&stack->elements);
}

int lstack_peek(lstack_t* stack, void* out) {
    if (!stack || !out)
	return -1;
    if (!lstack_is_empty(stack)) {
	memcpy(out, list_node_data(stack->elements.head), stack->elements.type_size);
	return 0;
    }
    return -1;
}

void* lstack_peek_ref(lstack_t* stack) {
    if (!stack)
	return NULL;
    if (!lstack_is_empty(stack)) {
	return list_node_data(stack->elements.head);
    }
    return NULL;
}

int lstack_push(lstack_t* stack, void* in) {
    if (!stack || !in)
	return -1;
    return list_push_front(&stack->elements, in);
}

int lstack_pop(lstack_t* stack, void* out) {
    if (!stack)
	return -1;
    return list_pop_front(&stack->elements, out);
}

void lstack_print(lstack_t* stack, void (*print)(void*)) {
    if (!stack)
	return;
    list_print(&stack->elements, print);
}


int astack_init(astack_t* stack, int type_size, int capacity, int (*cmp)(void*, void*)) {
    if (!stack)
	return -1;
    return array_init(&stack->elements, type_size, capacity, cmp);
}

int astack_close(astack_t* stack) {
    if (!stack)
	return -1;
    return array_close(&stack->elements);
}

int astack_is_empty(astack_t* stack) {
    if (!stack)
	return -1;
    return stack->elements.len == 0;
}

int astack_peek(astack_t* stack, void* out) {
    if (!stack || !out)
	return -1;
    return array_at(&stack->elements, stack->elements.len - 1, out);
}

void* astack_peek_ref(astack_t* stack) {
    if (!stack)
	return NULL;
    return array_at_ref(&stack->elements, stack->elements.len - 1);
}

int astack_push(astack_t* stack, void* in) {
    if (!stack || !in)
	return -1;
    return array_insert(&stack->elements, stack->elements.len, in);
}

int astack_pop(astack_t* stack, void* out) {
    if (!stack)
	return -1;
    if (out)
	astack_peek(stack, out);
    return array_delete_index(&stack->elements, stack->elements.len - 1);
}

void astack_print(astack_t* stack, void (*print)(void*)) {
    if (!stack)
	return;
    array_print(&stack->elements, print);
    
}

lstack_type_implementation(char, i8)
lstack_type_implementation(unsigned char, u8)
lstack_type_implementation(short, i16)
lstack_type_implementation(unsigned short, u16)
lstack_type_implementation(int, i32)
lstack_type_implementation(unsigned int, u32)
lstack_type_implementation(long, i64)
lstack_type_implementation(unsigned long, u64)
lstack_type_implementation(char, char)
lstack_type_implementation(float, f32)
lstack_type_implementation(double, f64)

astack_type_implementation(char, i8)
astack_type_implementation(unsigned char, u8)
astack_type_implementation(short, i16)
astack_type_implementation(unsigned short, u16)
astack_type_implementation(int, i32)
astack_type_implementation(unsigned int, u32)
astack_type_implementation(long, i64)
astack_type_implementation(unsigned long, u64)
astack_type_implementation(char, char)
astack_type_implementation(float, f32)
astack_type_implementation(double, f64)
    
