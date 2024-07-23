#include "clib.h"
#include <string.h>
#include <stdlib.h>

int lqueue_init(lqueue_t* queue, int type_size, int (*cmp)(void*, void*)) {
    if (!queue || !cmp)
	return -1;
    return list_init(&queue->elements, type_size, cmp);
}

int lqueue_close(lqueue_t* queue) {
    if (!queue)
	return -1;
    return list_close(&queue->elements);
}

int lqueue_is_empty(lqueue_t* queue) {
    if (!queue)
	return -1;
    return list_is_empty(&queue->elements);
}

int lqueue_peek(lqueue_t* queue, void* out) {
    if (!queue || !out || lqueue_is_empty(queue))
	return -1;
    memcpy(out, list_node_data(queue->elements.head), queue->elements.type_size);
    return 0;
}

void* lqueue_peek_ref(lqueue_t* queue) {
   if (!queue || lqueue_is_empty(queue))
	return NULL;
   return list_node_data(queue->elements.head);
}

int lqueue_enqueue(lqueue_t* queue, void* in) {
    if (!queue || !in)
	return -1;
    return list_push_back(&queue->elements, in);
}

int lqueue_dequeue(lqueue_t* queue, void* out) {
    if (!queue)
	return -1;
    return list_pop_front(&queue->elements, out);
}

void lqueue_print(lqueue_t* queue, void (*print)(void*)) {
    if (queue)
	list_print(&queue->elements, print);
}


int aqueue_init(aqueue_t* queue, int type_size, int capacity) {
    if (!queue)
	return -1;
    queue->type_size = type_size;
    queue->capacity = capacity;
    queue->head = queue->tail = queue->len = 0;
    queue->data = calloc(queue->type_size, queue->capacity);
    return 0;
}

int aqueue_close(aqueue_t* queue) {
    if (!queue)
	return -1;
    free(queue->data);
    queue->capacity = queue->head = queue->tail = queue->len = 0;
    queue->data = NULL;
    return 0;
}

int aqueue_is_empty(aqueue_t* queue) {
    if (!queue)
	return -1;
    return queue->len == 0;
}

int aqueue_peek(aqueue_t* queue, void* out) {
    if (!queue)
	return -1;
    if (aqueue_is_empty(queue))
	return -1;
    memcpy(out, (char*)queue->data + queue->head * queue->type_size, queue->type_size);
    return 0;
}

void* aqueue_peek_ref(aqueue_t* queue) {
    if (!queue || aqueue_is_empty(queue))
	return NULL;
    return (char*) queue->data + queue->head * queue->type_size;
}

void aqueue_realloc(aqueue_t* queue) {
    void* new;
    size_t i, newcap, idx;
    newcap = queue->capacity * 2;
    new = calloc(queue->type_size, newcap);

    for (i = 0; i < queue->len; i++) {
	idx = (i + queue->head) % queue->capacity;
	memcpy((char*) new + i * queue->type_size, (char*) queue->data + idx * queue->type_size, queue->type_size);
    }
    queue->capacity = newcap;
    queue->head = 0;
    queue->tail = i - 1;
    free(queue->data);
    queue->data = new;
}

int aqueue_enqueue(aqueue_t* queue, void* in) {
    if (!queue || !in)
	return -1;
    if (queue->len >= queue->capacity - 1)
	aqueue_realloc(queue);
    
    queue->tail++;
    queue->tail %= queue->capacity;
    queue->len++;
    memcpy((char*) queue + queue->tail * queue->type_size, in, queue->type_size);
    return 0;
}

int aqueue_dequeue(aqueue_t* queue, void* out) {
    if (!queue || aqueue_is_empty(queue))
	return -1;

    if (out)
	memcpy(out, (char*) queue + queue->head * queue->type_size, queue->type_size);
    queue->head++;
    queue->head %= queue->capacity;
    queue->len--;
    return 0;
}

void aqueue_print(aqueue_t* queue, void (*print)(void*)) {
    size_t i, idx;
    if (!queue || !print)
	return;
    for (i = 0; i < queue->len; i++) {
	idx = (i + queue->head) % queue->capacity;
	print((char*) queue->data + idx * queue->type_size);
    }
}

lqueue_type_implementation(char, i8)
lqueue_type_implementation(unsigned char, u8)
lqueue_type_implementation(short, i16)
lqueue_type_implementation(unsigned short, u16)
lqueue_type_implementation(int, i32)
lqueue_type_implementation(unsigned int, u32)
lqueue_type_implementation(long, i64)
lqueue_type_implementation(unsigned long, u64)
lqueue_type_implementation(char, char)
lqueue_type_implementation(float, f32)
lqueue_type_implementation(double, f64)
    
aqueue_type_implementation(char, i8)
aqueue_type_implementation(unsigned char, u8)
aqueue_type_implementation(short, i16)
aqueue_type_implementation(unsigned short, u16)
aqueue_type_implementation(int, i32)
aqueue_type_implementation(unsigned int, u32)
aqueue_type_implementation(long, i64)
aqueue_type_implementation(unsigned long, u64)
aqueue_type_implementation(char, char)
aqueue_type_implementation(float, f32)
aqueue_type_implementation(double, f64)

