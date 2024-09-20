#include "clib.h"
#include <string.h>
#include <stdlib.h>

int ldeque_init(ldeque_t* deque, int type_size, int (*cmp)(void*, void*)) {
    if (!deque)
	return -1;
    return dlist_init(&deque->elements, type_size, cmp);
}

int ldeque_close(ldeque_t* deque) {
    if (!deque)
	return -1;
    return dlist_close(&deque->elements);
}

int ldeque_is_empty(ldeque_t* deque) {
    if (!deque)
	return -1;
    return dlist_is_empty(&deque->elements);
}

int ldeque_peek_front(ldeque_t* deque, void* out) {
    if (!deque || !out)
	return -1;
    memcpy(out, dlist_node_data(deque->elements.head), deque->elements.type_size);
    return 0;
}

int ldeque_peek_back(ldeque_t* deque, void* out) {
    if (!deque || !out)
	return -1;
    memcpy(out, dlist_node_data(deque->elements.tail), deque->elements.type_size);
    return 0;
}

void* ldeque_peek_front_ref(ldeque_t* deque) {
    if (!deque)
	return NULL;
    return dlist_node_data(deque->elements.head);
}

void* ldeque_peek_back_ref(ldeque_t* deque) {
    if (!deque)
	return NULL;
    return dlist_node_data(deque->elements.tail);
}

int ldeque_push_front(ldeque_t* deque, void* in) {
    if (!deque || !in)
	return -1;
    return dlist_push_front(&deque->elements, in);
}

int ldeque_push_back(ldeque_t* deque, void* in) {
    if (!deque || !in)
	return -1;
    return dlist_push_back(&deque->elements, in);
}

int ldeque_pop_front(ldeque_t* deque, void* out) {
    if (!deque)
	return -1;
    return dlist_pop_front(&deque->elements, out);
}

int ldeque_pop_back(ldeque_t* deque, void* out) {
    if (!deque)
	return -1;
    return dlist_pop_back(&deque->elements, out);
}

void ldeque_print(ldeque_t* deque, void (*print)(void*)) {
    if (!deque)
	return;
    dlist_print(&deque->elements, print);
}

int adeque_init(adeque_t* deque, int type_size, int capacity) {
    if (!deque)
	return -1;
    deque->capacity = capacity;
    deque->type_size = type_size;
    deque->len = 0;
    deque->head = deque->tail = 0;
    deque->data = calloc(deque->type_size, deque->capacity);
    return 0;
}

int adeque_close(adeque_t* deque) {
    if (!deque)
	return -1;
    free(deque->data);
    deque->data = NULL;
    deque->capacity= deque->type_size = deque->len = deque->head = deque->tail = 0;
    return 0;
}

int adeque_is_empty(adeque_t* deque) {
    if (!deque)
	return -1;
    return deque->len == 0;
}

int adeque_peek_front(adeque_t* deque, void* out) {
    if (!deque || !out)
	return -1;

    memcpy(out, (char*) deque->data + deque->head * deque->type_size, deque->type_size);
    return 0;
}

int adeque_peek_back(adeque_t* deque, void* out) {
    if (!deque || !out)
	return -1;

    memcpy(out, (char*) deque->data + deque->tail * deque->type_size, deque->type_size);
    return 0;
}
void* adeque_peek_front_ref(adeque_t* deque) {
    if (!deque)
	return NULL;
    return (char*) deque->data + deque->head * deque->type_size;
}

void* adeque_peek_back_ref(adeque_t* deque) {
    if (!deque)
	return NULL;
    return (char*) deque->data + deque->tail * deque->type_size;
}

void adeque_realloc(adeque_t* deque) {
    size_t idx, i;
    void* new;
    new = calloc(deque->type_size, deque->capacity * 2);
    for (i = 0; i < deque->len; i++) {
	idx = (i + deque->head) % deque->capacity;
	memcpy((char*) new + i * deque->type_size, (char*) deque->data + idx * deque->type_size, deque->type_size);
    }
    free(deque->data);
    deque->data = new;
    deque->head = 0;
    deque->tail = i - 1;
}


int adeque_push_front(adeque_t* deque, void* in) {
    int head;
    if (!deque || !in)
	return -1;
    
    if (deque->len >= deque->capacity)
	adeque_realloc(deque);
    deque->len++;
    head = (int) deque->head;
    head--;
    if (head < 0)
	head = deque->capacity - 1;
    deque->head = head;
    memcpy((char*) deque->data + deque->head * deque->type_size, in, deque->type_size);
    return 0;
}

int adeque_push_back(adeque_t* deque, void* in) {
    if (!deque || !in)
	return -1;

    if (deque->len >= deque->capacity)
	adeque_realloc(deque);
    deque->len++;
    deque->tail++;
    deque->tail %= deque->capacity;
    memcpy((char*) deque->data + deque->tail * deque->type_size, in, deque->type_size);
    return 0;
}

int adeque_pop_front(adeque_t* deque, void* out) {
    if (!deque || deque->len <= 0)
	return -1;

    deque->len--;
    if (out)
	memcpy(out, (char*) deque->data + deque->head * deque->type_size, deque->type_size);
    
    deque->head++;
    deque->head %= deque->capacity;
    
    return 0;
}

int adeque_pop_back(adeque_t* deque, void* out) {
    int tail;
	
    if (!deque || deque->len <= 0)
	return -1;
    deque->len--;
    if (out)
	memcpy(out, (char*) deque->data + deque->tail * deque->type_size, deque->type_size);
    tail = (int) deque->tail;
    tail--;
    if (tail < 0)
	tail = deque->capacity - 1;
    deque->tail = tail;
    return 0;
}

void adeque_print(adeque_t* deque, void (*print)(void*)) {
    size_t i, idx;
    if (!deque || !print)
	return;
    for (i = 0; i < deque->len; i++) {
	idx = (i + deque->head) % deque->capacity;
	print((char*) deque->data + idx * deque->type_size);
    }
}
ldeque_type_implementation(char, i8)
ldeque_type_implementation(unsigned char, u8)
ldeque_type_implementation(short, i16)
ldeque_type_implementation(unsigned short, u16)
ldeque_type_implementation(int, i32)
ldeque_type_implementation(unsigned int, u32)
ldeque_type_implementation(long, i64)
ldeque_type_implementation(unsigned long, u64)
ldeque_type_implementation(char, char)
ldeque_type_implementation(float, f32)
ldeque_type_implementation(double, f64)

adeque_type_implementation(char, i8)
adeque_type_implementation(unsigned char, u8)
adeque_type_implementation(short, i16)
adeque_type_implementation(unsigned short, u16)
adeque_type_implementation(int, i32)
adeque_type_implementation(unsigned int, u32)
adeque_type_implementation(long, i64)
adeque_type_implementation(unsigned long, u64)
adeque_type_implementation(char, char)
adeque_type_implementation(float, f32)
adeque_type_implementation(double, f64)

