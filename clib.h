#ifndef CLIB_H
#define CLIB_H

#include <stddef.h>
#include <stdio.h>

typedef struct array_s {
    void* data;
    size_t len, capacity, type_size;    
    int (*cmp)(void*, void*); /* key - value ( greater > 0, equal 0, less < 0 ) */
} array_t;

int array_init(array_t* array, int type_size, int start_capacity, int (*cmp)(void*, void*));
int array_close(array_t* array);
int array_insert(array_t* array, int idx, void* element);
int array_at(array_t* array, int idx, void* out);
void* array_at_ref(array_t* array, int idx);
int array_delete(array_t* array, void* element);
int array_delete_index(array_t* array, int idx);
int array_print(array_t* array, void(*print)(void*));
int array_search(array_t* array, void* key, void* out);
int array_min(array_t* array, void* out);
int array_max(array_t* array, void* out);
int array_successor(array_t* array, int element, void *out);
int array_predecessor(array_t* array, int element, void *out);
int array_copy(array_t* src, array_t* dest);

int array_quicksort(array_t* array, size_t lo, size_t hi);
int array_insertionsort(array_t* array);
int array_mergesort(array_t* array, int p, int r);
int array_selectionsort(array_t* array);


#define array_push_front(a, el) array_insert(a, 0, el)
#define array_push_back(a, el) array_insert(a, (a)->len, el)


#define array_type_definition(type, name)				\
    int array_##name##_init(array_t* array, int start_capacity);	\
    int array_##name##_close(array_t* array);				\
    int array_##name##_insert(array_t* array, int idx, type element);	\
    type array_##name##_at(array_t* array, int idx);			\
    type* array_##name##_at_ref(array_t* array, int idx);		\
    int array_##name##_delete(array_t* array, type element);		\
    int array_##name##_delete_index(array_t* array, int idx);		\
    int array_##name##_print(array_t* array);				\
    int array_##name##_search(array_t* array, type element);		\
    int array_##name##_min(array_t* array, type* out);			\
    int array_##name##_max(array_t* array, type* out);			\
    int array_##name##_successor(array_t* array, int element, type* out); \
    int array_##name##_predecessor(array_t* array, int element, type* out);
#define util_type_definition(type, name) \
    int cmp_##name(void* key, void* el); \
    void print_##name(void* el); 

#define util_type_implementation(type, name) \
        int cmp_##name(void* key, void* el) {				\
	type l, r;							\
	l = *(type*) key;						\
	r = *(type*) el;						\
	if (l < r)							\
	    return -1;							\
	if (l > r)							\
	    return 1;							\
	return 0;							\
    }									

#define array_type_implementation(type, name)				\
    int array_##name##_init(array_t* array, int start_capacity) {	\
	return array_init(array, sizeof(type), start_capacity, cmp_##name); \
    }									\
    int array_##name##_close(array_t* array) {				\
	return array_close(array);					\
    }									\
    int array_##name##_insert(array_t* array, int idx, type element) {	\
	type n = element;						\
	return array_insert(array, idx, &n);				\
    }									\
    type array_##name##_at(array_t* array, int idx) {			\
	type res;							\
	array_at(array, idx, &res);					\
	return res;							\
    }									\
    type* array_##name##_at_ref(array_t* array, int idx) {		\
	return array_at_ref(array, idx);				\
    }									\
    int array_##name##_delete(array_t* array, type element) {		\
	type n = element;						\
	return array_delete(array, &n);					\
    }									\
    int array_##name##_delete_index(array_t* array, int idx) {		\
	return array_delete_index(array, idx);				\
    }									\
    int array_##name##_print(array_t* array) {				\
	return array_print(array, print_##name);			\
    }									\
    int array_##name##_search(array_t* array, type element) {		\
	type key = element;						\
	return array_search(array, &key, NULL);				\
    }									\
    int array_##name##_min(array_t* array, type* out) {			\
	return array_min(array, (void*) out);				\
    }									\
    int array_##name##_max(array_t* array, type* out) {			\
	return array_max(array, (void*) out);				\
    }									\
    int array_##name##_successor(array_t* array, int element, type* out) { \
	return array_successor(array, element, (void*) out);			\
    }									\
    int array_##name##_predecessor(array_t* array, int element, type* out) { \
	return array_predecessor(array, element, (void*) out);		\
    }


array_type_definition(char, i8)
array_type_definition(unsigned char, u8)
array_type_definition(short, i16)
array_type_definition(unsigned short, u16)
array_type_definition(int, i32)
array_type_definition(unsigned int, u32)
array_type_definition(long, i64)
array_type_definition(unsigned long, u64)
array_type_definition(char, char)
array_type_definition(float, f32)
array_type_definition(double, f64)

util_type_definition(char, i8)
util_type_definition(unsigned char, u8)
util_type_definition(short, i16)
util_type_definition(unsigned short, u16)
util_type_definition(int, i32)
util_type_definition(unsigned int, u32)
util_type_definition(long, i64)
util_type_definition(unsigned long, u64)
util_type_definition(char, char)
util_type_definition(float, f32)
util_type_definition(double, f64)

typedef array_t sorted_array_t;
#define sorted_array_init(a, ts, cap, cmp) array_init((array_t*)a, ts, cap, cmp)
#define sorted_array_at(a, idx, out) array_at((array_t*) a, idx, out)
#define sorted_array_at_ref(a, idx) array_at_ref((array_t*) a, idx)
#define sorted_array_close(a) array_close((array_t*)a)
int sorted_array_insert(sorted_array_t* array, void* element);
int sorted_array_bsearch(sorted_array_t* array, void* key, void* out);
#define sorted_array_print(a, print) array_print((array_t*)a, print)
int sorted_array_from_array(sorted_array_t* dest, array_t* src);

#define sorted_array_type_definition(type, name)			\
    int sorted_array_##name##_init(sorted_array_t* array, int capacity); \
    type sorted_array_##name##_at(sorted_array_t* array, int index);	\
    type* sorted_array_##name##_at_ref(sorted_array_t* array, int index); \
    int sorted_array_##name##_close(sorted_array_t* array);		\
    int sorted_array_##name##_insert(sorted_array_t* array, type element); \
    int sorted_array_##name##_bsearch(sorted_array_t* array, type element); \
    void sorted_array_##name##_print(sorted_array_t* array);		\
    int sorted_array_##name##_from_array(sorted_array_t* dest, array_t* src);

#define sorted_array_type_implementation(type, name) \
    int sorted_array_##name##_init(sorted_array_t* array, int capacity) { \
	return sorted_array_init(array, sizeof(type), capacity, cmp_##name); \
    }									\
    type sorted_array_##name##_at(sorted_array_t* array, int index) {	\
	type res;							\
	sorted_array_at(array, index, &res);				\
	return res;							\
    }									\
    type* sorted_array_##name##_at_ref(sorted_array_t* array, int index) { \
        return sorted_array_at_ref(array, index);			\
    }									\
    int sorted_array_##name##_close(sorted_array_t* array) {		\
	return sorted_array_close(array);				\
    }									\
    int sorted_array_##name##_insert(sorted_array_t* array, type element) { \
	type __n = element;						\
	return sorted_array_insert(array, &__n);			\
    }									\
    int sorted_array_##name##_bsearch(sorted_array_t* array, type element) { \
	type __n = element;						\
	return sorted_array_bsearch(array, &__n, NULL);			\
    }									\
    void sorted_array_##name##_print(sorted_array_t* array) {		\
	sorted_array_print(array, print_##name);			\
    }									\
    int sorted_array_##name##_from_array(sorted_array_t* dest, array_t* src) { \
	return sorted_array_from_array(dest, src);			\
    }
sorted_array_type_definition(char, i8)
sorted_array_type_definition(unsigned char, u8)
sorted_array_type_definition(short, i16)
sorted_array_type_definition(unsigned short, u16)
sorted_array_type_definition(int, i32)
sorted_array_type_definition(unsigned int, u32)
sorted_array_type_definition(long, i64)
sorted_array_type_definition(unsigned long, u64)
sorted_array_type_definition(char, char)
sorted_array_type_definition(float, f32)
sorted_array_type_definition(double, f64)
    

typedef struct list_node_s {
    struct list_node_s* next;
    /* data */
} list_node_t;
typedef struct list_s {
    struct list_node_s *head, *tail;
    size_t type_size, len;
    int (*cmp)(void*, void*);
} list_t;
#define list_node_data(el) ((char*)(el) + sizeof(struct list_node_s*))

int list_init(list_t* list, size_t type_size, int (*cmp)(void*, void*));
int list_close(list_t* list);
int list_is_empty(list_t* list);
int list_push_front(list_t* list, void* data);
int list_push_back(list_t* list, void* data);
int list_pop_front(list_t* list, void* out);
int list_pop_back(list_t* list, void* out);
int list_insert_index(list_t* list, int index, void* data);
int list_delete(list_t* list, void* element);
int list_delete_index(list_t* list, int index);
int list_print(list_t* list, void (*print)(void*));
int list_search(list_t* list, void* key, void* out);
list_node_t* list_search_node(list_t* list, void* key);
int list_min(list_t* list, void* out);
list_node_t* list_min_node(list_t* list);
int list_max(list_t* list, void* out);
list_node_t* list_max_node(list_t* list);
int list_successor(list_t* list, void* element, void* out);
list_node_t* list_successor_node(list_t* list, void* element);
int list_predecessor(list_t* list, void* element, void* out);
list_node_t* list_predecessor_node(list_t* list, void* element);

#define list_type_definition(type, name)				\
    int list_##name##_init(list_t* list);				\
    int list_##name##_close(list_t* list);				\
    int list_##name##_is_empty(list_t* list);				\
    int list_##name##_push_front(list_t* list, type data);		\
    int list_##name##_push_back(list_t* list, type data);		\
    type list_##name##_pop_front(list_t* list);				\
    type list_##name##_pop_back(list_t* list);				\
    int list_##name##_insert_index(list_t* list, int index, type data); \
    int list_##name##_delete(list_t* list, type element);		\
    int list_##name##_print(list_t* list);				\
    list_node_t* list_##name##_search(list_t* list, type element);	\
    type list_##name##_min(list_t* list);				\
    list_node_t* list_##name##_min_node(list_t* list);			\
    type list_##name##_max(list_t* list);				\
    list_node_t* list_##name##_max_node(list_t* list);			\
    type list_##name##_successor(list_t* list, type element);		\
    list_node_t* list_##name##_successor_node(list_t* list, type element); \
    type list_##name##_predecessor(list_t* list, type element);		\
    list_node_t* list_##name##_predecessor_node(list_t* list, type element);

#define list_type_implementation(type, name)			\
    int list_##name##_init(list_t* list) {			\
	return list_init(list, sizeof(type), cmp_##name);	\
    }								\
    int list_##name##_close(list_t* list) {			\
	return list_close(list);				\
    }								\
    int list_##name##_is_empty(list_t* list) {			\
	return list_is_empty(list);				\
    }								\
    int list_##name##_push_front(list_t* list, type data) {	\
	type __n = data;					\
	return list_push_front(list, &__n);			\
    }								\
    int list_##name##_push_back(list_t* list, type data) {	\
	type __n = data;					\
	return list_push_back(list, &__n);			\
    }								\
    type list_##name##_pop_front(list_t* list) {		\
	type res;						\
	list_pop_front(list, &res);				\
	return res;						\
    }								\
    type list_##name##_pop_back(list_t* list) {			\
	type res;						\
	list_pop_back(list, &res);				\
	return res;						\
    }								\
    int list_##name##_insert_index(list_t* list, int index, type data) { \
	type __n = data;						\
	return list_insert_index(list, index, &__n);				\
    }									\
    int list_##name##_delete(list_t* list, type element) {		\
	type __n = element;						\
	return list_delete(list, &__n);					\
    }									\
    int list_##name##_print(list_t* list) {				\
	return list_print(list, print_##name);				\
    }									\
    list_node_t* list_##name##_search(list_t* list, type element) {	\
	type __n = element;						\
	return list_search_node(list, &__n);				\
    }									\
    type list_##name##_min(list_t* list) {				\
	type res;							\
	list_min(list, &res);						\
	return res;							\
    }									\
    list_node_t* list_##name##_min_node(list_t* list) {			\
	return list_min_node(list);					\
    }									\
    type list_##name##_max(list_t* list) {				\
	type res;							\
	list_max(list, &res);						\
	return res;							\
    }									\
    list_node_t* list_##name##_max_node(list_t* list) {			\
	return list_max_node(list);					\
    }									\
    type list_##name##_successor(list_t* list, type element) {		\
	type __n = element, res;							\
	list_successor(list, &__n, &res);				\
	return res;							\
    }									\
    list_node_t* list_##name##_successor_node(list_t* list, type element){ \
	type __n = element;						\
	return list_successor_node(list, &__n);				\
    }									\
    type list_##name##_predecessor(list_t* list, type element) {	\
	type __n = element, res;					\
	list_predecessor(list, &__n, &res);				\
	return res;							\
    }									\
    list_node_t* list_##name##_predecessor_node(list_t* list, type element) { \
	type __n = element;						\
	return list_predecessor_node(list, &__n);			\
    }

list_type_definition(char, i8)
list_type_definition(unsigned char, u8)
list_type_definition(short, i16)
list_type_definition(unsigned short, u16)
list_type_definition(int, i32)
list_type_definition(unsigned int, u32)
list_type_definition(long, i64)
list_type_definition(unsigned long, u64)
list_type_definition(char, char)
list_type_definition(float, f32)
list_type_definition(double, f64)
							  

typedef struct dlist_node_s {
    struct dlist_node_s* prev, *next;
    /* data */
} dlist_node_t;
typedef struct dlist_s {
    dlist_node_t *head, *tail;
    size_t len, type_size;
    int (*cmp)(void*, void*);
} dlist_t;

#define dlist_node_data(el) ((char*)(el) + sizeof(struct dlist_node_s*))

int dlist_init(dlist_t* list, size_t type_size, int (*cmp)(void*, void*));
int dlist_close(dlist_t* list);
int dlist_is_empty(dlist_t* list);
int dlist_push_front(dlist_t* list, void* data);
int dlist_push_back(dlist_t* list, void* data);
int dlist_pop_front(dlist_t* list, void* out);
int dlist_pop_back(dlist_t* list, void* out);
int dlist_insert_index(dlist_t* list, int index, void* data);
int dlist_delete(dlist_t* list, void* element);
int dlist_delete_index(dlist_t* list, int index);
int dlist_print(dlist_t* list, void (*print)(void*));
int dlist_search(dlist_t* list, void* key, void* out);
dlist_node_t* dlist_search_node(dlist_t* list, void* key);
int dlist_min(dlist_t* list, void* out);
dlist_node_t* dlist_min_node(dlist_t* list);
int dlist_max(dlist_t* list, void* out);
dlist_node_t* dlist_max_node(dlist_t* list);
int dlist_successor(dlist_t* list, void* element, void* out);
dlist_node_t* dlist_successor_node(dlist_t* list, void* element);
int dlist_predecessor(dlist_t* list, void* element, void* out);
dlist_node_t* dlist_predecessor_node(dlist_t* list, void* element);

#define dlist_type_definition(type, name)				\
    int dlist_##name##_init(dlist_t* list);				\
    int dlist_##name##_close(dlist_t* list);				\
    int dlist_##name##_is_empty(dlist_t* list);				\
    int dlist_##name##_push_front(dlist_t* list, type data);		\
    int dlist_##name##_push_back(dlist_t* list, type data);		\
    type dlist_##name##_pop_front(dlist_t* list);				\
    type dlist_##name##_pop_back(dlist_t* list);				\
    int dlist_##name##_insert_index(dlist_t* list, int index, type data); \
    int dlist_##name##_delete(dlist_t* list, type element);		\
    int dlist_##name##_print(dlist_t* list);				\
    dlist_node_t* dlist_##name##_search(dlist_t* list, type element);	\
    type dlist_##name##_min(dlist_t* list);				\
    dlist_node_t* dlist_##name##_min_node(dlist_t* list);			\
    type dlist_##name##_max(dlist_t* list);				\
    dlist_node_t* dlist_##name##_max_node(dlist_t* list);			\
    type dlist_##name##_successor(dlist_t* list, type element);		\
    dlist_node_t* dlist_##name##_successor_node(dlist_t* list, type element); \
    type dlist_##name##_predecessor(dlist_t* list, type element);		\
    dlist_node_t* dlist_##name##_predecessor_node(dlist_t* list, type element);

#define dlist_type_implementation(type, name)			\
    int dlist_##name##_init(dlist_t* list) {			\
	return dlist_init(list, sizeof(type), cmp_##name);	\
    }								\
    int dlist_##name##_close(dlist_t* list) {			\
	return dlist_close(list);				\
    }								\
    int dlist_##name##_is_empty(dlist_t* list) {			\
	return dlist_is_empty(list);				\
    }								\
    int dlist_##name##_push_front(dlist_t* list, type data) {	\
	type __n = data;					\
	return dlist_push_front(list, &__n);			\
    }								\
    int dlist_##name##_push_back(dlist_t* list, type data) {	\
	type __n = data;					\
	return dlist_push_back(list, &__n);			\
    }								\
    type dlist_##name##_pop_front(dlist_t* list) {		\
	type res;						\
	dlist_pop_front(list, &res);				\
	return res;						\
    }								\
    type dlist_##name##_pop_back(dlist_t* list) {			\
	type res;						\
	dlist_pop_back(list, &res);				\
	return res;						\
    }								\
    int dlist_##name##_insert_index(dlist_t* list, int index, type data) { \
	type __n = data;						\
	return dlist_insert_index(list, index, &__n);				\
    }									\
    int dlist_##name##_delete(dlist_t* list, type element) {		\
	type __n = element;						\
	return dlist_delete(list, &__n);					\
    }									\
    int dlist_##name##_print(dlist_t* list) {				\
	return dlist_print(list, print_##name);				\
    }									\
    dlist_node_t* dlist_##name##_search(dlist_t* list, type element) {	\
	type __n = element;						\
	return dlist_search_node(list, &__n);				\
    }									\
    type dlist_##name##_min(dlist_t* list) {				\
	type res;							\
	dlist_min(list, &res);						\
	return res;							\
    }									\
    dlist_node_t* dlist_##name##_min_node(dlist_t* list) {			\
	return dlist_min_node(list);					\
    }									\
    type dlist_##name##_max(dlist_t* list) {				\
	type res;							\
	dlist_max(list, &res);						\
	return res;							\
    }									\
    dlist_node_t* dlist_##name##_max_node(dlist_t* list) {			\
	return dlist_max_node(list);					\
    }									\
    type dlist_##name##_successor(dlist_t* list, type element) {		\
	type __n = element, res;							\
	dlist_successor(list, &__n, &res);				\
	return res;							\
    }									\
    dlist_node_t* dlist_##name##_successor_node(dlist_t* list, type element){ \
	type __n = element;						\
	return dlist_successor_node(list, &__n);				\
    }									\
    type dlist_##name##_predecessor(dlist_t* list, type element) {	\
	type __n = element, res;					\
	dlist_predecessor(list, &__n, &res);				\
	return res;							\
    }									\
    dlist_node_t* dlist_##name##_predecessor_node(dlist_t* list, type element) { \
	type __n = element;						\
	return dlist_predecessor_node(list, &__n);			\
    }

dlist_type_definition(char, i8)
dlist_type_definition(unsigned char, u8)
dlist_type_definition(short, i16)
dlist_type_definition(unsigned short, u16)
dlist_type_definition(int, i32)
dlist_type_definition(unsigned int, u32)
dlist_type_definition(long, i64)
dlist_type_definition(unsigned long, u64)
dlist_type_definition(char, char)
dlist_type_definition(float, f32)
dlist_type_definition(double, f64)

typedef struct lstack_s {
    list_t elements;
} lstack_t;

int lstack_init(lstack_t* stack, int type_size, int (*cmp)(void*, void*));
int lstack_close(lstack_t* stack);
int lstack_is_empty(lstack_t* stack);
int lstack_peek(lstack_t* stack, void* out);
int lstack_push(lstack_t* stack, void* in);
int lstack_pop(lstack_t* stack, void* out);
void lstack_print(lstack_t* stack, void (*print)(void*));

typedef struct astack_s {
    array_t elements;
} astack_t;

int astack_init(astack_t* stack, int type_size, int capacity, int (*cmp)(void*, void*));
int astack_close(astack_t* stack);
int astack_is_empty(astack_t* stack);
int astack_peek(astack_t* stack, void* out);
int astack_push(astack_t* stack, void* in);
int astack_pop(astack_t* stack, void* out);
void astack_print(astack_t* stack, void (*print)(void*));

typedef struct lqueue_s {
    list_t elements;
} lqueue_t;

int lqueue_init(lqueue_t* queue, int type_size, int (*cmp)(void*, void*));
int lqueue_close(lqueue_t* queue);
int lqueue_is_empty(lqueue_t* queue);
int lqueue_peek(lqueue_t* queue, void* out);
int lqueue_enqueue(lqueue_t* queue, void* in);
int lqueue_dequeue(lqueue_t* queue, void* out);
void lqueue_print(lqueue_t* queue, void (*print)(void*));

typedef struct aqueue_s {
    void* data;
    size_t head, tail, capacity, len, type_size;
} aqueue_t;

int aqueue_init(aqueue_t* queue, int type_size, int capacity);
int aqueue_close(aqueue_t* queue);
int aqueue_is_empty(aqueue_t* queue);
int aqueue_peek(aqueue_t* queue, void* out);
int aqueue_enqueue(aqueue_t* queue, void* in);
int aqueue_dequeue(aqueue_t* queue, void* out);
void aqueue_print(aqueue_t* queue, void (*print)(void*));

typedef struct ldeque_s {
    dlist_t elements;
} ldeque_t;

int ldeque_init(ldeque_t* deque, int type_size, int (*cmp)(void*, void*));
int ldeque_close(ldeque_t* deque);
int ldeque_is_empty(ldeque_t* deque);
int ldeque_peek_front(ldeque_t* deque, void* out);
int ldeque_peek_back(ldeque_t* deque, void* out);
int ldeque_push_front(ldeque_t* deque, void* in);
int ldeque_push_back(ldeque_t* deque, void* in);
int ldeque_pop_front(ldeque_t* deque, void* out);
int ldeque_pop_back(ldeque_t* deque, void* out);
void ldeque_print(ldeque_t* deque, void (*print)(void*));

typedef struct adeque_s {
    void* data;
    size_t type_size, head, tail, len, capacity;
} adeque_t;

int adeque_init(adeque_t* deque, int type_size, int capacity);
int adeque_close(adeque_t* deque);
int adeque_is_empty(adeque_t* deque);
int adeque_peek_front(adeque_t* deque, void* out);
int adeque_peek_back(adeque_t* deque, void* out);
int adeque_push_front(adeque_t* deque, void* in);
int adeque_push_back(adeque_t* deque, void* in);
int adeque_pop_front(adeque_t* deque, void* out);
int adeque_pop_back(adeque_t* deque, void* out);
void adeque_print(adeque_t* deque, void (*print)(void*));

typedef struct oh_hashmap_s {
    list_t *data;
    size_t type_size, bucket_count, full_buckets, size;
    size_t (*hash)(void*, size_t);
    int (*cmp)(void*, void*);
} oh_hashmap_t;

int oh_hashmap_init(oh_hashmap_t* map, int type_size, int capacity, size_t (*hash)(void*, size_t), int (*cmp)(void*, void*));
int oh_hashmap_close(oh_hashmap_t* map);
int oh_hashmap_insert(oh_hashmap_t* map, void* in);
int oh_hashmap_delete(oh_hashmap_t* map, void* element);
int oh_hashmap_search(oh_hashmap_t* map, void* key, void* out);
void* oh_hashmap_search_ref(oh_hashmap_t*, void* key);

typedef struct lbinary_tree_node_s {
    struct lbinary_tree_node_s *left, *right, *p;
    /* data */
} lbinary_tree_node_t;

typedef struct lbinary_tree_s {
    lbinary_tree_node_t* root;
    size_t size, type_size;
    int (*cmp)(void*, void*);
} lbinary_tree_t;

#define lbinary_tree_node_data(node) ((char*) (node)) + sizeof(lbinary_tree_node_t)

int lbinary_tree_init(lbinary_tree_t* tree, int type_size, int (*cmp)(void*, void*));
int lbinary_tree_close(lbinary_tree_t* tree);
void lbinary_tree_visit_in_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data);
void lbinary_tree_visit_pre_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data);
void lbinary_tree_visit_post_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data);

void lbinary_tree_print_func(lbinary_tree_node_t* node, void* data);
#define lbinary_tree_print_in_order(tree, print) lbinary_tree_visit_in_order(tree, lbinary_tree_print_func, print)
#define lbinary_tree_print_pre_order(tree, print) lbinary_tree_visit_pre_order(tree, lbinary_tree_print_func, print)
#define lbinary_tree_print_post_order(tree, print) lbinary_tree_visit_post_order(tree, lbinary_tree_print_func, print)

typedef struct abinary_tree_s {
    void *data;
    size_t size, type_size, capacity;
    int (*cmp)(void*, void*);
} abinary_tree_t;

#define abinary_tree_node_left(node) (2 * (node) + 1)
#define abinary_tree_node_right(node) (2 * (node) + 2)
#define abinary_tree_node_parent(node) (((node) - 1) / 2)

int abinary_tree_init(abinary_tree_t* tree, int type_size, int capacity, int (*cmp)(void*, void*));
int abinary_tree_close(abinary_tree_t* tree);
void abinary_tree_visit_in_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data);
void abinary_tree_visit_pre_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data);
void abinary_tree_visit_post_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data);

void abinary_tree_print_func(abinary_tree_t* node, int i, void*);
#define abinary_tree_print_in_order(tree, print) abinary_tree_visit_in_order(tree, 0, abinary_tree_print_func, print)
#define abinary_tree_print_pre_order(tree, print) abinary_tree_visit_pre_order(tree, 0, abinary_tree_print_func, print)
#define abinary_tree_print_post_order(tree, print) abinary_tree_visit_post_order(tree, 0, abinary_tree_print_func, print)

typedef lbinary_tree_t binary_search_tree_t;
typedef lbinary_tree_node_t binary_search_tree_node_t;

#define binary_search_tree_init(tree, ts, cmp) lbinary_tree_init(tree, ts, cmp)
#define binary_search_tree_close(tree) lbinary_tree_close(tree)
#define binary_search_tree_visit_in_order(node, func, data) lbinary_tree_visit_in_order(node, func, data)
#define binary_search_tree_visit_pre_order(node, func, data) lbinary_tree_visit_pre_order(node, func, data)
#define binary_search_tree_visit_post_order(node, func, data) lbinary_tree_visit_post_order(node, func, data)
#define binary_search_tree_print_in_order(node, print) lbinary_tree_print_in_order(node, print)
#define binary_search_tree_print_pre_order(node, print) lbinary_tree_print_pre_order(node, print)
#define binary_search_tree_print_post_order(node, print) lbinary_tree_print_post_order(node, print)
int binary_search_tree_insert(binary_search_tree_t* bst, void* element);
int binary_search_tree_delete(binary_search_tree_t* bst, void* element);
int binary_search_tree_search(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* key, void* out);
void* binary_search_tree_search_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* key);
binary_search_tree_node_t* binary_search_tree_search_node(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* key);
int binary_search_tree_min(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out);
int binary_search_tree_max(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out);
int binary_search_tree_successor(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out);
int binary_search_tree_predecessor(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out);
void* binary_search_tree_min_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node);
void* binary_search_tree_max_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node);
void* binary_search_tree_successor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node);
void* binary_search_tree_predecessor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node);
binary_search_tree_node_t* binary_search_tree_min_node(binary_search_tree_t* bst, binary_search_tree_node_t* node);
binary_search_tree_node_t* binary_search_tree_max_node(binary_search_tree_t* bst, binary_search_tree_node_t* node);
binary_search_tree_node_t* binary_search_tree_successor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node);
binary_search_tree_node_t* binary_search_tree_predecessor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node);

typedef binary_search_tree_node_t rb_tree_node_t;
typedef binary_search_tree_t rb_tree_t;

#define rb_tree_red 1
#define rb_tree_black 0
#define rb_tree_node_ptr(node) ((rb_tree_node_t*)(((size_t)node) & (~(0x1))))
#define rb_tree_node_color(node) (((size_t)node) & 0x1)
#define rb_tree_node_set_red(node) (node) = (rb_tree_node_t*)(((size_t)node) | 0x1)
#define rb_tree_node_set_black(node) (node) = (rb_tree_node_t*)(((size_t)node) & (~(0x1)))
#define rb_tree_node_data(node) lbinary_tree_node_data(rb_tree_node_ptr(node))

#define rb_tree_init(tree, ts, cmp) lbinary_tree_init(tree, ts, cmp)
#define rb_tree_close(tree) lbinary_tree_close(tree)
int rb_tree_visit_in_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data);
void rb_tree_print_func(rb_tree_node_t* node, void* data);
#define rb_tree_visit_pre_order(node, func, data) lbinary_tree_visit_pre_order(node, func, data)
#define rb_tree_visit_post_order(node, func, data) lbinary_tree_visit_post_order(node, func, data)
#define rb_tree_print_in_order(node, print) rb_tree_visit_in_order(node, rb_tree_print_func, print)
#define rb_tree_print_pre_order(node, print) lbinary_tree_print_pre_order(node, print)
#define rb_tree_print_post_order(node, print) lbinary_tree_print_post_order(node, print)

int rb_tree_insert(rb_tree_t* tree, void* element);
int rb_tree_delete(rb_tree_t* tree, void* element);
rb_tree_node_t* rb_tree_search_node(rb_tree_t* tree, rb_tree_node_t* node, void* element);
rb_tree_node_t* rb_tree_min_node(rb_tree_t* tree, rb_tree_node_t* node);
rb_tree_node_t* rb_tree_successor_node(rb_tree_t* tree, rb_tree_node_t* node);

#endif /* CLIB_H */
