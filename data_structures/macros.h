#ifndef MACROS_H
#define MACROS_H


#define array_type_definition(type, name)				\
    int array_##name##_init(array_t* array, int start_capacity);	\
    int array_##name##_close(array_t* array);				\
    int array_##name##_insert(array_t* array, int idx, type element);	\
    int array_##name##_push_back(array_t* array, type element);		\
    int array_##name##_push_front(array_t* array, type element);	\
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
    int array_##name##_push_back(array_t* array, type element) {	\
	type n = element;						\
	return array_push_back(array, &n);				\
    }									\
    int array_##name##_push_front(array_t* array, type element) {	\
	type n = element;						\
	return array_push_front(array, &n);				\
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
    list_node_t* list_##name##_predecessor_node(list_t* list, type element); \
    type list_##name##_node_data(list_node_t* node);

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
    } \
    type list_##name##_node_data(list_node_t* node) {	\
	return *(type*) list_node_data(node);		\
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
    dlist_node_t* dlist_##name##_predecessor_node(dlist_t* list, type element); \
    type dlist_##name##_node_data(dlist_node_t* node);			

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
    } \
    type dlist_##name##_node_data(dlist_node_t* node) { \
	return *(type*) dlist_node_data(node);		\
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

#define lstack_type_definition(type, name)		\
    int lstack_##name##_init(lstack_t* stack);		\
    int lstack_##name##_close(lstack_t* stack);		\
    int lstack_##name##_is_empty(lstack_t* stack);	\
    type lstack_##name##_peek(lstack_t* stack);		\
    type* lstack_##name##_peek_ref(lstack_t* stack);	\
    int lstack_##name##_push(lstack_t* stack, type in);	\
    type lstack_##name##_pop(lstack_t* stack);		\
    void lstack_##name##_print(lstack_t* stack);

#define lstack_type_implementation(type, name)			\
    int lstack_##name##_init(lstack_t* stack) {			\
	return lstack_init(stack, sizeof(type), cmp_##name);	\
    }								\
    int lstack_##name##_close(lstack_t* stack) {		\
	return lstack_close(stack);				\
    }								\
    int lstack_##name##_is_empty(lstack_t* stack) {		\
	return lstack_is_empty(stack);				\
    }								\
    type lstack_##name##_peek(lstack_t* stack) {			\
	type __n = 0;							\
	lstack_peek(stack, &__n);					\
	return __n;							\
    }									\
    type* lstack_##name##_peek_ref(lstack_t* stack) {			\
	return lstack_peek_ref(stack);					\
    }									\
    int lstack_##name##_push(lstack_t* stack, type in) {		\
	type __n = in;							\
	return lstack_push(stack, &__n);				\
    }									\
    type lstack_##name##_pop(lstack_t* stack) {				\
	type __n = 0;							\
	lstack_pop(stack, &__n);					\
	return __n;							\
    }									\
    void lstack_##name##_print(lstack_t* stack) {			\
	lstack_print(stack, print_##name);				\
    }

lstack_type_definition(char, i8)
lstack_type_definition(unsigned char, u8)
lstack_type_definition(short, i16)
lstack_type_definition(unsigned short, u16)
lstack_type_definition(int, i32)
lstack_type_definition(unsigned int, u32)
lstack_type_definition(long, i64)
lstack_type_definition(unsigned long, u64)
lstack_type_definition(char, char)
lstack_type_definition(float, f32)
lstack_type_definition(double, f64)

#define astack_type_definition(type, name)		\
    int astack_##name##_init(astack_t* stack, int capacity);	\
    int astack_##name##_close(astack_t* stack);		\
    int astack_##name##_is_empty(astack_t* stack);	\
    type astack_##name##_peek(astack_t* stack);		\
    type* astack_##name##_peek_ref(astack_t* stack);	\
    int astack_##name##_push(astack_t* stack, type in);	\
    type astack_##name##_pop(astack_t* stack);		\
    void astack_##name##_print(astack_t* stack);

#define astack_type_implementation(type, name)			\
    int astack_##name##_init(astack_t* stack, int capacity) {		\
	return astack_init(stack, sizeof(type), capacity, cmp_##name);	\
    }								\
    int astack_##name##_close(astack_t* stack) {		\
	return astack_close(stack);				\
    }								\
    int astack_##name##_is_empty(astack_t* stack) {		\
	return astack_is_empty(stack);				\
    }								\
    type astack_##name##_peek(astack_t* stack) {			\
	type __n = 0;							\
	astack_peek(stack, &__n);					\
	return __n;							\
    }									\
    type* astack_##name##_peek_ref(astack_t* stack) {			\
	return astack_peek_ref(stack);					\
    }									\
    int astack_##name##_push(astack_t* stack, type in) {		\
	type __n = in;							\
	return astack_push(stack, &__n);				\
    }									\
    type astack_##name##_pop(astack_t* stack) {				\
	type __n = 0;							\
	astack_pop(stack, &__n);					\
	return __n;							\
    }									\
    void astack_##name##_print(astack_t* stack) {			\
	astack_print(stack, print_##name);				\
    }

astack_type_definition(char, i8)
astack_type_definition(unsigned char, u8)
astack_type_definition(short, i16)
astack_type_definition(unsigned short, u16)
astack_type_definition(int, i32)
astack_type_definition(unsigned int, u32)
astack_type_definition(long, i64)
astack_type_definition(unsigned long, u64)
astack_type_definition(char, char)
astack_type_definition(float, f32)
astack_type_definition(double, f64)

#define lqueue_type_definition(type, name)				\
    int lqueue_##name##_init(lqueue_t* queue);				\
    int lqueue_##name##_close(lqueue_t* queue);				\
    int lqueue_##name##_is_empty(lqueue_t* queue);			\
    type lqueue_##name##_peek(lqueue_t* queue);				\
    type* lqueue_##name##_peek_ref(lqueue_t* queue);			\
    int lqueue_##name##_enqueue(lqueue_t* queue, type in);		\
    type lqueue_##name##_dequeue(lqueue_t* queue);			\
    void lqueue_##name##_print(lqueue_t* queue);
#define lqueue_type_implementation(type, name)				\
    int lqueue_##name##_init(lqueue_t* queue) {				\
	return lqueue_init(queue, sizeof(type), cmp_##name);		\
    }									\
    int lqueue_##name##_close(lqueue_t* queue) {			\
	return lqueue_close(queue);					\
    }									\
    int lqueue_##name##_is_empty(lqueue_t* queue) {			\
	return lqueue_is_empty(queue);					\
    }									\
    type lqueue_##name##_peek(lqueue_t* queue) {			\
	type __n = 0;							\
	lqueue_peek(queue, &__n);					\
	return __n;							\
    }									\
    type* lqueue_##name##_peek_ref(lqueue_t* queue) {			\
	return lqueue_peek_ref(queue);					\
    }									\
    int lqueue_##name##_enqueue(lqueue_t* queue, type in) {		\
	type __n = in;							\
	return lqueue_enqueue(queue, &__n);				\
    }									\
    type lqueue_##name##_dequeue(lqueue_t* queue) {			\
	type __n = 0;							\
	lqueue_dequeue(queue, &__n);					\
	return __n;							\
    }									\
    void lqueue_##name##_print(lqueue_t* queue) {			\
	lqueue_print(queue, print_##name);				\
    }

lqueue_type_definition(char, i8)
lqueue_type_definition(unsigned char, u8)
lqueue_type_definition(short, i16)
lqueue_type_definition(unsigned short, u16)
lqueue_type_definition(int, i32)
lqueue_type_definition(unsigned int, u32)
lqueue_type_definition(long, i64)
lqueue_type_definition(unsigned long, u64)
lqueue_type_definition(char, char)
lqueue_type_definition(float, f32)
lqueue_type_definition(double, f64)

#define aqueue_type_definition(type, name)				\
    int aqueue_##name##_init(aqueue_t* queue, int capacity);		\
    int aqueue_##name##_close(aqueue_t* queue);				\
    int aqueue_##name##_is_empty(aqueue_t* queue);			\
    type aqueue_##name##_peek(aqueue_t* queue);				\
    type* aqueue_##name##_peek_ref(aqueue_t* queue);			\
    int aqueue_##name##_enqueue(aqueue_t* queue, type in);		\
    type aqueue_##name##_dequeue(aqueue_t* queue);			\
    void aqueue_##name##_print(aqueue_t* queue);
#define aqueue_type_implementation(type, name)				\
    int aqueue_##name##_init(aqueue_t* queue, int capacity) {		\
	return aqueue_init(queue, sizeof(type), capacity);		\
    }									\
    int aqueue_##name##_close(aqueue_t* queue) {			\
	return aqueue_close(queue);					\
    }									\
    int aqueue_##name##_is_empty(aqueue_t* queue) {			\
	return aqueue_is_empty(queue);					\
    }									\
    type aqueue_##name##_peek(aqueue_t* queue) {			\
	type __n = 0;							\
	aqueue_peek(queue, &__n);					\
	return __n;							\
    }									\
    type* aqueue_##name##_peek_ref(aqueue_t* queue) {			\
	return aqueue_peek_ref(queue);					\
    }									\
    int aqueue_##name##_enqueue(aqueue_t* queue, type in) {		\
	type __n = in;							\
	return aqueue_enqueue(queue, &__n);				\
    }									\
    type aqueue_##name##_dequeue(aqueue_t* queue) {			\
	type __n = 0;							\
	aqueue_dequeue(queue, &__n);					\
	return __n;							\
    }									\
    void aqueue_##name##_print(aqueue_t* queue) {			\
	aqueue_print(queue, print_##name);				\
    }

aqueue_type_definition(char, i8)
aqueue_type_definition(unsigned char, u8)
aqueue_type_definition(short, i16)
aqueue_type_definition(unsigned short, u16)
aqueue_type_definition(int, i32)
aqueue_type_definition(unsigned int, u32)
aqueue_type_definition(long, i64)
aqueue_type_definition(unsigned long, u64)
aqueue_type_definition(char, char)
aqueue_type_definition(float, f32)
aqueue_type_definition(double, f64)

#define ldeque_type_definition(type, name)				\
    int ldeque_##name##_init(ldeque_t* deque);				\
    int ldeque_##name##_close(ldeque_t* deque);				\
    int ldeque_##name##_is_empty(ldeque_t* deque);			\
    type ldeque_##name##_peek_front(ldeque_t* deque);			\
    type ldeque_##name##_peek_back(ldeque_t* deque);			\
    type* ldeque_##name##_peek_front_ref(ldeque_t* deque);		\
    type* ldeque_##name##_peek_back_ref(ldeque_t* deque);		\
    int ldeque_##name##_push_front(ldeque_t* deque, type in);		\
    int ldeque_##name##_push_back(ldeque_t* deque, type in);		\
    type ldeque_##name##_pop_front(ldeque_t* deque);			\
    type ldeque_##name##_pop_back(ldeque_t* deque);			\
    void ldeque_##name##_print(ldeque_t* deque);
#define ldeque_type_implementation(type, name)			\
    int ldeque_##name##_init(ldeque_t* deque) {			\
	return ldeque_init(deque, sizeof(type), cmp_##name);	\
    }								\
    int ldeque_##name##_close(ldeque_t* deque) {		\
	return ldeque_close(deque);				\
    }								\
    int ldeque_##name##_is_empty(ldeque_t* deque) {		\
	return ldeque_is_empty(deque);				\
    }								\
    type ldeque_##name##_peek_front(ldeque_t* deque) {		\
        type __n = 0;						\
	ldeque_peek_front(deque, &__n);				\
	return __n;						\
    }								\
    type ldeque_##name##_peek_back(ldeque_t* deque) {		\
        type __n = 0;						\
	ldeque_peek_back(deque, &__n);				\
	return __n;						\
    }								\
    type* ldeque_##name##_peek_front_ref(ldeque_t* deque) {	\
	return ldeque_peek_front_ref(deque);			\
    }								\
    type* ldeque_##name##_peek_back_ref(ldeque_t* deque) {	\
	return ldeque_peek_front_ref(deque);			\
    }								\
    int ldeque_##name##_push_front(ldeque_t* deque, type in) {		\
	type __n = in;							\
	return ldeque_push_front(deque, &__n);				\
    }									\
    int ldeque_##name##_push_back(ldeque_t* deque, type in) {		\
	type __n = in;							\
	return ldeque_push_back(deque, &__n);				\
    }									\
    type ldeque_##name##_pop_front(ldeque_t* deque) {			\
	type __n = 0;							\
	ldeque_pop_front(deque, &__n);					\
	return __n;							\
    }									\
    type ldeque_##name##_pop_back(ldeque_t* deque) {			\
	type __n = 0;							\
	ldeque_pop_back(deque, &__n);					\
	return __n;							\
    }									\
    void ldeque_##name##_print(ldeque_t* deque) {			\
	ldeque_print(deque, print_##name);				\
    }

ldeque_type_definition(char, i8)
ldeque_type_definition(unsigned char, u8)
ldeque_type_definition(short, i16)
ldeque_type_definition(unsigned short, u16)
ldeque_type_definition(int, i32)
ldeque_type_definition(unsigned int, u32)
ldeque_type_definition(long, i64)
ldeque_type_definition(unsigned long, u64)
ldeque_type_definition(char, char)
ldeque_type_definition(float, f32)
ldeque_type_definition(double, f64)

#define adeque_type_definition(type, name)				\
    int adeque_##name##_init(adeque_t* deque, int capacity);		\
    int adeque_##name##_close(adeque_t* deque);				\
    int adeque_##name##_is_empty(adeque_t* deque);			\
    type adeque_##name##_peek_front(adeque_t* deque);			\
    type adeque_##name##_peek_back(adeque_t* deque);			\
    type* adeque_##name##_peek_front_ref(adeque_t* deque);		\
    type* adeque_##name##_peek_back_ref(adeque_t* deque);		\
    int adeque_##name##_push_front(adeque_t* deque, type in);		\
    int adeque_##name##_push_back(adeque_t* deque, type in);		\
    type adeque_##name##_pop_front(adeque_t* deque);			\
    type adeque_##name##_pop_back(adeque_t* deque);			\
    void adeque_##name##_print(adeque_t* deque);
#define adeque_type_implementation(type, name)			\
    int adeque_##name##_init(adeque_t* deque, int capacity) {	\
	return adeque_init(deque, sizeof(type), capacity);	\
    }								\
    int adeque_##name##_close(adeque_t* deque) {		\
	return adeque_close(deque);				\
    }								\
    int adeque_##name##_is_empty(adeque_t* deque) {		\
	return adeque_is_empty(deque);				\
    }								\
    type adeque_##name##_peek_front(adeque_t* deque) {		\
        type __n = 0;						\
	adeque_peek_front(deque, &__n);				\
	return __n;						\
    }								\
    type adeque_##name##_peek_back(adeque_t* deque) {		\
        type __n = 0;						\
	adeque_peek_back(deque, &__n);				\
	return __n;						\
    }								\
    type* adeque_##name##_peek_front_ref(adeque_t* deque) {	\
	return adeque_peek_front_ref(deque);			\
    }								\
    type* adeque_##name##_peek_back_ref(adeque_t* deque) {	\
	return adeque_peek_front_ref(deque);			\
    }								\
    int adeque_##name##_push_front(adeque_t* deque, type in) {		\
	type __n = in;							\
	return adeque_push_front(deque, &__n);				\
    }									\
    int adeque_##name##_push_back(adeque_t* deque, type in) {		\
	type __n = in;							\
	return adeque_push_back(deque, &__n);				\
    }									\
    type adeque_##name##_pop_front(adeque_t* deque) {			\
	type __n = 0;							\
	adeque_pop_front(deque, &__n);					\
	return __n;							\
    }									\
    type adeque_##name##_pop_back(adeque_t* deque) {			\
	type __n = 0;							\
	adeque_pop_back(deque, &__n);					\
	return __n;							\
    }									\
    void adeque_##name##_print(adeque_t* deque) {			\
	adeque_print(deque, print_##name);				\
    }

adeque_type_definition(char, i8)
adeque_type_definition(unsigned char, u8)
adeque_type_definition(short, i16)
adeque_type_definition(unsigned short, u16)
adeque_type_definition(int, i32)
adeque_type_definition(unsigned int, u32)
adeque_type_definition(long, i64)
adeque_type_definition(unsigned long, u64)
adeque_type_definition(char, char)
adeque_type_definition(float, f32)
adeque_type_definition(double, f64)

#define oh_hashmap_type_definition(type, name)				\
    int oh_hashmap_##name##_init(oh_hashmap_t* map, int capacity, size_t (*hash)(void*, size_t)); \
    int oh_hashmap_##name##_close(oh_hashmap_t* map);			\
    int oh_hashmap_##name##_insert(oh_hashmap_t* map, type in);		\
    int oh_hashmap_##name##_delete(oh_hashmap_t* map, type element);	\
    type* oh_hashmap_##name##_search_ref(oh_hashmap_t* map, type key);
#define oh_hashmap_type_implementation(type, name)	\
    int oh_hashmap_##name##_init(oh_hashmap_t* map, int capacity, size_t (*hash)(void*, size_t)) { \
         return oh_hashmap_init(map, sizeof(type), capacity, hash, cmp_##name); \
    }									\
    int oh_hashmap_##name##_close(oh_hashmap_t* map) {			\
	return oh_hashmap_close(map);					\
    }									\
    int oh_hashmap_##name##_insert(oh_hashmap_t* map, type in) {	\
	type __n = in;							\
	return oh_hashmap_insert(map, &__n);				\
    }									\
    int oh_hashmap_##name##_delete(oh_hashmap_t* map, type element) {	\
	type __n = element;						\
	return oh_hashmap_delete(map, &__n);				\
    }									\
    type* oh_hashmap_##name##_search_ref(oh_hashmap_t* map, type key) { \
	type __n = key;							\
	return oh_hashmap_search_ref(map, &__n);			\
    }								


oh_hashmap_type_definition(char, i8)
oh_hashmap_type_definition(unsigned char, u8)
oh_hashmap_type_definition(short, i16)
oh_hashmap_type_definition(unsigned short, u16)
oh_hashmap_type_definition(int, i32)
oh_hashmap_type_definition(unsigned int, u32)
oh_hashmap_type_definition(long, i64)
oh_hashmap_type_definition(unsigned long, u64)
oh_hashmap_type_definition(char, char)
oh_hashmap_type_definition(float, f32)
oh_hashmap_type_definition(double, f64)

#define lbinary_tree_type_definition(type, name)			\
    type *lbinary_tree_##name##_node_data(lbinary_tree_node_t* node);	\
    int lbinary_tree_##name##_init(lbinary_tree_t* tree);		\
    int lbinary_tree_##name##_close(lbinary_tree_t* tree);		\
    void lbinary_tree_##name##_visit_in_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data); \
    void lbinary_tree_##name##_visit_pre_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data); \
    void lbinary_tree_##name##_visit_post_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data); \
    void lbinary_tree_##name##_print_in_order(lbinary_tree_node_t* node);	\
    void lbinary_tree_##name##_print_pre_order(lbinary_tree_node_t* node);	\
    void lbinary_tree_##name##_print_post_order(lbinary_tree_node_t* node);
#define lbinary_tree_type_implementation(type, name)			\
    type *lbinary_tree_##name##_node_data(lbinary_tree_node_t* node) {	\
	return (type*)lbinary_tree_node_data(node);			\
    }									\
    int lbinary_tree_##name##_init(lbinary_tree_t* tree) {		\
	return lbinary_tree_init(tree, sizeof(type), cmp_##name);	\
    }									\
    int lbinary_tree_##name##_close(lbinary_tree_t* tree) {		\
	return lbinary_tree_close(tree);				\
    }									\
    void lbinary_tree_##name##_visit_in_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data) { \
	lbinary_tree_visit_in_order(node, func, data);			\
    }									\
    void lbinary_tree_##name##_visit_pre_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data) { \
	lbinary_tree_visit_pre_order(node, func, data);			\
    }									\
    void lbinary_tree_##name##_visit_post_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data) { \
	lbinary_tree_visit_post_order(node, func, data);		\
    }									\
    void lbinary_tree_##name##_print_in_order(lbinary_tree_node_t* node) { \
	lbinary_tree_print_in_order(node, print_##name);		\
    }									\
    void lbinary_tree_##name##_print_pre_order(lbinary_tree_node_t* node) { \
	lbinary_tree_print_pre_order(node, print_##name);		\
    }									\
    void lbinary_tree_##name##_print_post_order(lbinary_tree_node_t* node) { \
	lbinary_tree_print_post_order(node, print_##name);		\
    }									
lbinary_tree_type_definition(char, i8)
lbinary_tree_type_definition(unsigned char, u8)
lbinary_tree_type_definition(short, i16)
lbinary_tree_type_definition(unsigned short, u16)
lbinary_tree_type_definition(int, i32)
lbinary_tree_type_definition(unsigned int, u32)
lbinary_tree_type_definition(long, i64)
lbinary_tree_type_definition(unsigned long, u64)
lbinary_tree_type_definition(char, char)
lbinary_tree_type_definition(float, f32)
lbinary_tree_type_definition(double, f64)


#define abinary_tree_type_definition(type, name)			\
    type* abinary_tree_##name##_node_data(abinary_tree_t* tree, int i);	\
    int abinary_tree_##name##_init(abinary_tree_t* tree, int capacity);	\
    int abinary_tree_##name##_close(abinary_tree_t* tree);		\
    void abinary_tree_##name##_visit_in_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data); \
    void abinary_tree_##name##_visit_pre_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data); \
    void abinary_tree_##name##_visit_post_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data); \
    void abinary_tree_##name##_print_in_order(abinary_tree_t* tree);		\
    void abinary_tree_##name##_print_pre_order(abinary_tree_t* tree);		\
    void abinary_tree_##name##_print_post_order(abinary_tree_t* tree); 

#define abinary_tree_type_implementation(type, name)			\
    type* abinary_tree_##name##_node_data(abinary_tree_t* tree, int i) { \
	return (type*) abinary_tree_node_data(tree, i);			\
    }									\
    int abinary_tree_##name##_init(abinary_tree_t* tree, int capacity) { \
	return abinary_tree_init(tree, sizeof(type), capacity, cmp_##name); \
    }									\
    int abinary_tree_##name##_close(abinary_tree_t* tree) {		\
	return abinary_tree_close(tree);				\
    }									\
    void abinary_tree_##name##_visit_in_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data) { \
	abinary_tree_visit_in_order(tree, i, func, data);		\
    }									\
    void abinary_tree_##name##_visit_pre_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data) { \
	abinary_tree_visit_pre_order(tree, i, func, data);		\
    }									\
    void abinary_tree_##name##_visit_post_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data) { \
	abinary_tree_visit_post_order(tree, i, func, data);		\
    }									\
    void abinary_tree_##name##_print_in_order(abinary_tree_t* tree) {	\
	abinary_tree_print_in_order(tree, print_##name);		\
    }									\
    void abinary_tree_##name##_print_pre_order(abinary_tree_t* tree) {	\
	abinary_tree_print_pre_order(tree, print_##name);		\
    }									\
    void abinary_tree_##name##_print_post_order(abinary_tree_t* tree) { \
	abinary_tree_print_post_order(tree, print_##name);		\
    }
abinary_tree_type_definition(char, i8)
abinary_tree_type_definition(unsigned char, u8)
abinary_tree_type_definition(short, i16)
abinary_tree_type_definition(unsigned short, u16)
abinary_tree_type_definition(int, i32)
abinary_tree_type_definition(unsigned int, u32)
abinary_tree_type_definition(long, i64)
abinary_tree_type_definition(unsigned long, u64)
abinary_tree_type_definition(char, char)
abinary_tree_type_definition(float, f32)
abinary_tree_type_definition(double, f64)

#define binary_search_tree_type_definition(type, name)			\
    int binary_search_tree_##name##_init(binary_search_tree_t* bst);	\
    int binary_search_tree_##name##_close(binary_search_tree_t* bst);	\
    void binary_search_tree_##name##_visit_in_order(binary_search_tree_node_t* node, void (*func)(binary_search_tree_node_t*, void*), void* data); \
    void binary_search_tree_##name##_visit_pre_order(binary_search_tree_node_t* node, void (*func)(binary_search_tree_node_t*, void*), void* data); \
    void binary_search_tree_##name##_visit_post_order(binary_search_tree_node_t* node, void (*func)(binary_search_tree_node_t*, void*), void* data); \
    void binary_search_tree_##name##_print_in_order(binary_search_tree_node_t* node); \
    void binary_search_tree_##name##_print_pre_order(binary_search_tree_node_t* node); \
    void binary_search_tree_##name##_print_post_order(binary_search_tree_node_t* node); \
    int binary_search_tree_##name##_insert(binary_search_tree_t* bst, type element); \
    int binary_search_tree_##name##_delete(binary_search_tree_t* bst, type element); \
    type* binary_search_tree_##name##_search_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node, type key); \
    binary_search_tree_node_t* binary_search_tree_##name##_search_node(binary_search_tree_t* bst, binary_search_tree_node_t* node, type key); \
    type binary_search_tree_##name##_min(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    type binary_search_tree_##name##_max(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    type binary_search_tree_##name##_successor(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    type binary_search_tree_##name##_predecessor(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    type* binary_search_tree_##name##_min_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    type* binary_search_tree_##name##_max_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    type* binary_search_tree_##name##_successor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    type* binary_search_tree_##name##_predecessor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    binary_search_tree_node_t* binary_search_tree_##name##_min_node(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    binary_search_tree_node_t* binary_search_tree_##name##_max_node(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    binary_search_tree_node_t* binary_search_tree_##name##_successor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node); \
    binary_search_tree_node_t* binary_search_tree_##name##_predecessor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node);

#define binary_search_tree_type_implementation(type, name)		\
    int binary_search_tree_##name##_init(binary_search_tree_t* bst) {	\
	return binary_search_tree_init(bst, sizeof(type), cmp_##name);	\
    }									\
    int binary_search_tree_##name##_close(binary_search_tree_t* bst) {	\
	return binary_search_tree_close(bst);				\
    }									\
    void binary_search_tree_##name##_visit_in_order(binary_search_tree_node_t* node, void (*func)(binary_search_tree_node_t*, void*), void* data) { \
	binary_search_tree_visit_in_order(node, func, data);		\
    }									\
    void binary_search_tree_##name##_visit_pre_order(binary_search_tree_node_t* node, void (*func)(binary_search_tree_node_t*, void*), void* data) { \
	binary_search_tree_visit_pre_order(node, func, data);		\
    }									\
    void binary_search_tree_##name##_visit_post_order(binary_search_tree_node_t* node, void (*func)(binary_search_tree_node_t*, void*), void* data) { \
	binary_search_tree_visit_post_order(node, func, data);		\
    }									\
    void binary_search_tree_##name##_print_in_order(binary_search_tree_node_t* node) { \
	binary_search_tree_print_in_order(node, print_##name);		\
    }									\
    void binary_search_tree_##name##_print_pre_order(binary_search_tree_node_t* node) { \
	binary_search_tree_print_pre_order(node, print_##name);		\
    }									\
    void binary_search_tree_##name##_print_post_order(binary_search_tree_node_t* node) { \
	binary_search_tree_print_post_order(node, print_##name);	\
    }									\
    int binary_search_tree_##name##_insert(binary_search_tree_t* bst, type element) { \
	type __n = element;						\
	return binary_search_tree_insert(bst, &__n);			\
    }									\
    int binary_search_tree_##name##_delete(binary_search_tree_t* bst, type element) { \
	type __n = element;						\
	return binary_search_tree_delete(bst, &__n);			\
    }									\
    type* binary_search_tree_##name##_search_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node, type key) { \
	type __n = key;							\
	return binary_search_tree_search_ref(bst, node, &__n);		\
    }									\
    binary_search_tree_node_t* binary_search_tree_##name##_search_node(binary_search_tree_t* bst, binary_search_tree_node_t* node, type key) { \
	type __n = key;							\
	return binary_search_tree_search_node(bst, node, &__n);		\
    }									\
    type binary_search_tree_##name##_min(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	type __n = 0;							\
	binary_search_tree_min(bst, node, &__n);			\
	return __n;							\
    }									\
    type binary_search_tree_##name##_max(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	type __n = 0;							\
	binary_search_tree_max(bst, node, &__n);			\
	return __n;							\
    }									\
    type binary_search_tree_##name##_successor(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	type __n = 0;							\
	binary_search_tree_successor(bst, node, &__n);			\
	return __n;							\
    }									\
    type binary_search_tree_##name##_predecessor(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	type __n = 0;							\
	binary_search_tree_predecessor(bst, node, &__n);		\
	return __n;							\
    }									\
    type* binary_search_tree_##name##_min_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_min_ref(bst, node);			\
    }									\
    type* binary_search_tree_##name##_max_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_max_ref(bst, node);			\
    }									\
    type* binary_search_tree_##name##_successor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_successor_ref(bst, node);		\
    }									\
    type* binary_search_tree_##name##_predecessor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_predecessor_ref(bst, node);		\
    }									\
    binary_search_tree_node_t* binary_search_tree_##name##_min_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_min_node(bst, node);			\
    }									\
    binary_search_tree_node_t* binary_search_tree_##name##_max_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_max_node(bst, node);			\
    }									\
    binary_search_tree_node_t* binary_search_tree_##name##_successor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_successor_node(bst, node);		\
    }									\
    binary_search_tree_node_t* binary_search_tree_##name##_predecessor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) { \
	return binary_search_tree_predecessor_node(bst, node);		\
    }

binary_search_tree_type_definition(char, i8)
binary_search_tree_type_definition(unsigned char, u8)
binary_search_tree_type_definition(short, i16)
binary_search_tree_type_definition(unsigned short, u16)
binary_search_tree_type_definition(int, i32)
binary_search_tree_type_definition(unsigned int, u32)
binary_search_tree_type_definition(long, i64)
binary_search_tree_type_definition(unsigned long, u64)
binary_search_tree_type_definition(char, char)
binary_search_tree_type_definition(float, f32)
binary_search_tree_type_definition(double, f64)

#define rb_tree_type_definition(type, name)				\
    type* rb_tree_##name##_node_data(rb_tree_node_t* node);		\
    int rb_tree_##name##_init(rb_tree_t* tree);				\
    int rb_tree_##name##_close(rb_tree_t* tree);			\
    int rb_tree_##name##_visit_in_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data); \
    int rb_tree_##name##_visit_pre_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data); \
    int rb_tree_##name##_visit_post_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data); \
    void rb_tree_##name##_print_in_order(rb_tree_node_t* node);		\
    void rb_tree_##name##_print_pre_order(rb_tree_node_t* node);	\
    void rb_tree_##name##_print_post_order(rb_tree_node_t* node);	\
    int rb_tree_##name##_insert(rb_tree_t* tree, type element);		\
    int rb_tree_##name##_delete(rb_tree_t* tree, type element);		\
    type* rb_tree_##name##_search_ref(rb_tree_t* tree, rb_tree_node_t* node, type element); \
    rb_tree_node_t* rb_tree_##name##_search_node(rb_tree_t* tree, rb_tree_node_t* node, type element); \
    rb_tree_node_t* rb_tree_##name##_min_node(rb_tree_t* tree, rb_tree_node_t* node); \
    rb_tree_node_t* rb_tree_##name##_max_node(rb_tree_t* tree, rb_tree_node_t* node); \
    rb_tree_node_t* rb_tree_##name##_successor_node(rb_tree_t* tree, rb_tree_node_t* node); \
    rb_tree_node_t* rb_tree_##name##_predecessor_node(rb_tree_t* tree, rb_tree_node_t* node); \
    type* rb_tree_##name##_min_ref(rb_tree_t* tree, rb_tree_node_t* node); \
    type* rb_tree_##name##_max_ref(rb_tree_t* tree, rb_tree_node_t* node); \
    type* rb_tree_##name##_successor_ref(rb_tree_t* tree, rb_tree_node_t* node); \
    type* rb_tree_##name##_predecessor_ref(rb_tree_t* tree, rb_tree_node_t* node); \
    type rb_tree_##name##_min(rb_tree_t* tree, rb_tree_node_t* node);	\
    type rb_tree_##name##_max(rb_tree_t* tree, rb_tree_node_t* node);	\
    type rb_tree_##name##_successor(rb_tree_t* tree, rb_tree_node_t* node); \
    type rb_tree_##name##_predecessor(rb_tree_t* tree, rb_tree_node_t* node);

#define rb_tree_type_implementation(type, name)				\
    type* rb_tree_##name##_node_data(rb_tree_node_t* node) {		\
	return (type*) rb_tree_node_data(node);				\
    }									\
    int rb_tree_##name##_init(rb_tree_t* tree) {			\
	return rb_tree_init(tree, sizeof(type), cmp_##name);		\
    }									\
    int rb_tree_##name##_close(rb_tree_t* tree) {			\
	return rb_tree_close(tree);					\
    }									\
    int rb_tree_##name##_visit_in_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data) { \
	return rb_tree_visit_in_order(node, func, data);		\
    }									\
    int rb_tree_##name##_visit_pre_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data) { \
	return rb_tree_visit_pre_order(node, func, data);		\
    }									\
    int rb_tree_##name##_visit_post_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data) { \
	return rb_tree_visit_post_order(node, func, data);		\
    }									\
    void rb_tree_##name##_print_in_order(rb_tree_node_t* node) {	\
	rb_tree_print_in_order(node, print_##name);			\
    }									\
    void rb_tree_##name##_print_pre_order(rb_tree_node_t* node) {	\
	rb_tree_print_pre_order(node, print_##name);			\
    }									\
    void rb_tree_##name##_print_post_order(rb_tree_node_t* node) {	\
	rb_tree_print_post_order(node, print_##name);			\
    }									\
    int rb_tree_##name##_insert(rb_tree_t* tree, type element) {	\
	type __n = element;						\
	return rb_tree_insert(tree, &__n);				\
    }									\
    int rb_tree_##name##_delete(rb_tree_t* tree, type element) {	\
	type __n = element;						\
	return rb_tree_delete(tree, &__n);				\
    }									\
    type* rb_tree_##name##_search_ref(rb_tree_t* tree, rb_tree_node_t* node, type element) { \
	type __n = element;						\
	return rb_tree_search_ref(tree, node, &__n);			\
    }									\
    rb_tree_node_t* rb_tree_##name##_search_node(rb_tree_t* tree, rb_tree_node_t* node, type element) { \
	type __n = element;						\
	return rb_tree_search_node(tree, node, &__n);			\
    }									\
    rb_tree_node_t* rb_tree_##name##_min_node(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_min_node(tree, node);				\
    }									\
    rb_tree_node_t* rb_tree_##name##_max_node(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_max_node(tree, node);				\
    }									\
    rb_tree_node_t* rb_tree_##name##_successor_node(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_successor_node(tree, node);				\
    }									\
    rb_tree_node_t* rb_tree_##name##_predecessor_node(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_predecessor_node(tree, node);			\
    }									\
    type* rb_tree_##name##_min_ref(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_min_ref(tree, node);				\
    }									\
    type* rb_tree_##name##_max_ref(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_max_ref(tree, node);				\
    }									\
    type* rb_tree_##name##_successor_ref(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_successor_ref(tree, node);				\
    }									\
    type* rb_tree_##name##_predecessor_ref(rb_tree_t* tree, rb_tree_node_t* node) { \
	return rb_tree_predecessor_ref(tree, node);				\
    }									\
    type rb_tree_##name##_min(rb_tree_t* tree, rb_tree_node_t* node) {	\
	type __n = 0;							\
	rb_tree_min(tree, node, &__n);					\
	return __n;							\
    }									\
    type rb_tree_##name##_max(rb_tree_t* tree, rb_tree_node_t* node) {	\
	type __n = 0;							\
	rb_tree_max(tree, node, &__n);					\
	return __n;							\
    }									\
    type rb_tree_##name##_successor(rb_tree_t* tree, rb_tree_node_t* node) { \
	type __n = 0;							\
	rb_tree_successor(tree, node, &__n);				\
	return __n;							\
    }									\
    type rb_tree_##name##_predecessor(rb_tree_t* tree, rb_tree_node_t* node) { \
	type __n = 0;							\
	rb_tree_predecessor(tree, node, &__n);				\
	return __n;							\
    }									
rb_tree_type_definition(char, i8)
rb_tree_type_definition(unsigned char, u8)
rb_tree_type_definition(short, i16)
rb_tree_type_definition(unsigned short, u16)
rb_tree_type_definition(int, i32)
rb_tree_type_definition(unsigned int, u32)
rb_tree_type_definition(long, i64)
rb_tree_type_definition(unsigned long, u64)
rb_tree_type_definition(char, char)
rb_tree_type_definition(float, f32)
rb_tree_type_definition(double, f64)

#define heap_type_definition(type, name)				\
    type* max_heap_##name##_data(heap_t* heap, int i);			\
    int max_heap_##name##_init(heap_t* heap, size_t height);		\
    int max_heap_##name##_close(heap_t* heap);				\
    int max_heap_##name##_insert(heap_t* heap, type data);		\
    type max_heap_##name##_pop_max(heap_t* heap);			\
    type max_heap_##name##_max(heap_t* heap);			\
    type* max_heap_##name##_max_ref(heap_t* heap);		\
    int max_heap_##name##_init_from_carray(heap_t* heap, type* array, int n); \
    int max_heap_##name##_init_from_array(heap_t* heap, array_t* array); \
    int max_heap_##name##_is_empty(heap_t* heap);			\
    type* min_heap_##name##_data(heap_t* heap, int i);			\
    int min_heap_##name##_init(heap_t* heap, size_t height);		\
    int min_heap_##name##_close(heap_t* heap);				\
    int min_heap_##name##_insert(heap_t* heap, type data);			\
    type min_heap_##name##_pop_min(heap_t* heap);				\
    type min_heap_##name##_min(heap_t* heap);				\
    type* min_heap_##name##_min_ref(heap_t* heap);			\
    int min_heap_##name##_init_from_carray(heap_t* heap, type* array, int n); \
    int min_heap_##name##_init_from_array(heap_t* heap, array_t* array);	\
    int min_heap_##name##_is_empty(heap_t* heap);

#define heap_type_implementation(type, name)	\
    type* max_heap_##name##_data(heap_t* heap, int i) {			\
	return (type*)heap_data(heap, i);				\
    }									\
    int max_heap_##name##_init(heap_t* heap, size_t height) {		\
	return max_heap_init(heap, height, sizeof(type), cmp_##name);	\
    }									\
    int max_heap_##name##_close(heap_t* heap) {				\
	return max_heap_close(heap);					\
    }									\
    int max_heap_##name##_insert(heap_t* heap, type data) {		\
        type __n = data;						\
	return max_heap_insert(heap, &__n);				\
    }									\
    type max_heap_##name##_pop_max(heap_t* heap) {			\
	type __n = 0;							\
	max_heap_pop_max(heap, &__n);					\
	return __n;							\
    }									\
    type max_heap_##name##_max(heap_t* heap) {				\
    type __n = 0;							\
    max_heap_max(heap, &__n);						\
    return __n;								\
    }									\
     type* max_heap_##name##_max_ref(heap_t* heap) {			\
	 return max_heap_max_ref(heap);					\
     }									\
     int max_heap_##name##_init_from_carray(heap_t* heap, type* array, int n) { \
	 return max_heap_init_from_carray(heap, array, n, sizeof(type), cmp_##name); \
     }									\
     int max_heap_##name##_init_from_array(heap_t* heap, array_t* array) { \
	 return max_heap_init_from_array(heap, array);			\
     }									\
     int max_heap_##name##_is_empty(heap_t* heap) {			\
	 return heap_is_empty(heap);					\
     }									\
     type* min_heap_##name##_data(heap_t* heap, int i) {		\
	 return (type*) heap_data(heap, i);				\
     }									\
     int min_heap_##name##_init(heap_t* heap, size_t height) {		\
	 return min_heap_init(heap, height, sizeof(type), cmp_##name);	\
     }									\
     int min_heap_##name##_close(heap_t* heap){				\
	 return min_heap_close(heap);					\
     }									\
     int min_heap_##name##_insert(heap_t* heap, type data) {		\
	 type __n = 0;							\
	 return min_heap_insert(heap, &__n);				\
     }									\
     type min_heap_##name##_pop_min(heap_t* heap) {			\
	 type __n = 0;							\
	 min_heap_pop_min(heap, &__n);					\
	 return __n;							\
     }									\
     type min_heap_##name##_min(heap_t* heap) {				\
	 type __n = 0;							\
	 min_heap_min(heap, &__n);					\
	 return __n;							\
     }									\
     type* min_heap_##name##_min_ref(heap_t* heap) {			\
	 return min_heap_min_ref(heap);					\
     }									\
     int min_heap_##name##_init_from_carray(heap_t* heap, type* array, int n) { \
	 return min_heap_init_from_carray(heap, array, n, sizeof(type), cmp_##name); \
     }									\
     int min_heap_##name##_init_from_array(heap_t* heap, array_t* array) { \
	 return min_heap_init_from_array(heap, array);			\
     }									\
     int min_heap_##name##_is_empty(heap_t* heap) {			\
	 return heap_is_empty(heap);					\
     }									
heap_type_definition(char, i8)
heap_type_definition(unsigned char, u8)
heap_type_definition(short, i16)
heap_type_definition(unsigned short, u16)
heap_type_definition(int, i32)
heap_type_definition(unsigned int, u32)
heap_type_definition(long, i64)
heap_type_definition(unsigned long, u64)
heap_type_definition(char, char)
heap_type_definition(float, f32)
heap_type_definition(double, f64)

#define mgraph_type_definition(type, name)				\
    type* mgraph_##name##_vertex_data(mgraph_t* gptr, int i);		\
    int mgraph_##name##_init(mgraph_t* g, int flags);			\
    int mgraph_##name##_close(mgraph_t* g);				\
    mgraph_node_t mgraph_##name##_add_vertex(mgraph_t* g, type vertex); \
    mgraph_node_t mgraph_##name##_add_vertices_carray(mgraph_t* g, type* vertices, int n); \
    mgraph_node_t mgraph_##name##_add_vertices_array(mgraph_t* g, array_t* vertices); \
    int mgraph_##name##_add_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r); \
    int mgraph_##name##_remove_vertex(mgraph_t* g, mgraph_node_t v);	\
    int mgraph_##name##_remove_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r); \
    int mgraph_##name##_connected(mgraph_t* g, mgraph_node_t l, mgraph_node_t r); \
    int mgraph_##name##_breadth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t*, mgraph_node_t, void*), void* data); \
    int mgraph_##name##_depth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t*, mgraph_node_t, void*), void* data); \
    void mgraph_##name##_print_adjacency_matrix(mgraph_t* g);		\
    int mgraph_##name##_connected_components(mgraph_t* g);		\
    int mgraph_##name##_topological_ordering(mgraph_t* g, list_t* out); \
    int mgraph_##name##_dijkstra(mgraph_t* g, mgraph_node_t node, float (*weight)(mgraph_node_t, mgraph_node_t)); \
    int mgraph_##name##_floyd(mgraph_t* g, mgraph_node_t s, int* out_T, int* out_C); \

#define mgraph_type_implementation(type, name)	\
    type* mgraph_##name##_vertex_data(mgraph_t* gptr, int i) {		\
	return (type*) mgraph_vertex_data(gptr, i);			\
    }									\
    int mgraph_##name##_init(mgraph_t* g, int flags) {			\
	return mgraph_init(g, sizeof(type), flags, cmp_##name);		\
    }									\
    int mgraph_##name##_close(mgraph_t* g) {				\
	return mgraph_close(g);						\
    }									\
    mgraph_node_t mgraph_##name##_add_vertex(mgraph_t* g, type vertex) { \
        type __n = vertex;						\
	return mgraph_add_vertex(g, &__n);				\
    }									\
    mgraph_node_t mgraph_##name##_add_vertices_carray(mgraph_t* g, type* vertices, int n) { \
	return mgraph_add_vertices_carray(g, (void*) vertices, n);	\
    }									\
    mgraph_node_t mgraph_##name##_add_vertices_array(mgraph_t* g, array_t* vertices) { \
	return mgraph_add_vertices_array(g, vertices);			\
    }									\
    int mgraph_##name##_add_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r) { \
	return mgraph_add_edge(g, l, r);				\
    }									\
    int mgraph_##name##_remove_vertex(mgraph_t* g, mgraph_node_t v) {	\
	return mgraph_remove_vertex(g, v);				\
    }									\
    int mgraph_##name##_remove_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r) { \
	return mgraph_remove_edge(g, l, r);				\
    }									\
    int mgraph_##name##_connected(mgraph_t* g, mgraph_node_t l, mgraph_node_t r) { \
	return mgraph_connected(g, l, r);				\
    }									\
    int mgraph_##name##_breadth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t*, mgraph_node_t, void*), void* data) { \
	return mgraph_breadth_first_search(g, node, func, data);	\
    }									\
    int mgraph_##name##_depth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t*, mgraph_node_t, void*), void* data) { \
	return mgraph_depth_first_search(g, node, func, data);		\
    }									\
    void mgraph_##name##_print_adjacency_matrix(mgraph_t* g) {		\
	return mgraph_print_adjacency_matrix(g, print_##name);		\
    }									\
    int mgraph_##name##_connected_components(mgraph_t* g) {		\
	return mgraph_connected_components(g);				\
    }									\
    int mgraph_##name##_topological_ordering(mgraph_t* g, list_t* out) { \
	return mgraph_topological_ordering(g, out);			\
    }									\
    int mgraph_##name##_dijkstra(mgraph_t* g, mgraph_node_t node, float (*weight)(mgraph_node_t, mgraph_node_t)) { \
	return mgraph_dijkstra(g, node, weight);			\
    }									\
    int mgraph_##name##_floyd(mgraph_t* g, mgraph_node_t s, int* out_T, int* out_C) { \
	return mgraph_floyd(g, s, out_T, out_C);			\
    }
mgraph_type_definition(char, i8)
mgraph_type_definition(unsigned char, u8)
mgraph_type_definition(short, i16)
mgraph_type_definition(unsigned short, u16)
mgraph_type_definition(int, i32)
mgraph_type_definition(unsigned int, u32)
mgraph_type_definition(long, i64)
mgraph_type_definition(unsigned long, u64)
mgraph_type_definition(char, char)
mgraph_type_definition(float, f32)
mgraph_type_definition(double, f64)

#define lgraph_type_definition(type, name)				\
    type* lgraph_##name##_vertex_data(lgraph_t* gptr, int i);		\
    int lgraph_##name##_init(lgraph_t* g, int flags);			\
    int lgraph_##name##_close(lgraph_t* g);				\
    lgraph_node_t lgraph_##name##_add_vertex(lgraph_t* g, type vertex); \
    lgraph_node_t lgraph_##name##_add_vertices_carray(lgraph_t* g, type* vertices, int n); \
    lgraph_node_t lgraph_##name##_add_vertices_array(lgraph_t* g, array_t* vertices); \
    int lgraph_##name##_add_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r); \
    int lgraph_##name##_remove_vertex(lgraph_t* g, lgraph_node_t v);	\
    int lgraph_##name##_remove_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r); \
    int lgraph_##name##_connected(lgraph_t* g, lgraph_node_t l, lgraph_node_t r); \
    int lgraph_##name##_breadth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data); \
    int lgraph_##name##_depth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data); \
    int lgraph_##name##_connected_components(lgraph_t* g);		\
    int lgraph_##name##_topological_ordering(lgraph_t* g, list_t* out); \
    int lgraph_##name##_dijkstra(lgraph_t* g, lgraph_node_t node, float (*weight)(lgraph_node_t, lgraph_node_t)); \
    int lgraph_##name##_floyd(lgraph_t* g, lgraph_node_t s, int* out_T, int* out_C); \
    void lgraph_##name##_adjacency_lists_print(lgraph_t* g);

#define lgraph_type_implementation(type, name)	\
    type* lgraph_##name##_vertex_data(lgraph_t* gptr, int i) {		\
	return (type*) lgraph_vertex_data(gptr, i);			\
    }									\
    int lgraph_##name##_init(lgraph_t* g, int flags) {			\
	return lgraph_init(g, sizeof(type), flags, cmp_##name);		\
    }									\
    int lgraph_##name##_close(lgraph_t* g) {				\
	return lgraph_close(g);						\
    }									\
    lgraph_node_t lgraph_##name##_add_vertex(lgraph_t* g, type vertex) { \
        type __n = vertex;						\
	return lgraph_add_vertex(g, &__n);				\
    }									\
    lgraph_node_t lgraph_##name##_add_vertices_carray(lgraph_t* g, type* vertices, int n) { \
	return lgraph_add_vertices_carray(g, (void*) vertices, n);	\
    }									\
    lgraph_node_t lgraph_##name##_add_vertices_array(lgraph_t* g, array_t* vertices) { \
	return lgraph_add_vertices_array(g, vertices);			\
    }									\
    int lgraph_##name##_add_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r) { \
	return lgraph_add_edge(g, l, r);				\
    }									\
    int lgraph_##name##_remove_vertex(lgraph_t* g, lgraph_node_t v) {	\
	return lgraph_remove_vertex(g, v);				\
    }									\
    int lgraph_##name##_remove_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r) { \
	return lgraph_remove_edge(g, l, r);				\
    }									\
    int lgraph_##name##_connected(lgraph_t* g, lgraph_node_t l, lgraph_node_t r) { \
	return lgraph_connected(g, l, r);				\
    }									\
    int lgraph_##name##_breadth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data) { \
	return lgraph_breadth_first_search(g, node, func, data);	\
    }									\
    int lgraph_##name##_depth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data) { \
	return lgraph_depth_first_search(g, node, func, data);		\
    }									\
    int lgraph_##name##_connected_components(lgraph_t* g) {		\
	return lgraph_connected_components(g);				\
    }									\
    int lgraph_##name##_topological_ordering(lgraph_t* g, list_t* out) { \
	return lgraph_topological_ordering(g, out);			\
    }									\
    int lgraph_##name##_dijkstra(lgraph_t* g, lgraph_node_t node, float (*weight)(lgraph_node_t, lgraph_node_t)) { \
	return lgraph_dijkstra(g, node, weight);			\
    }									\
    int lgraph_##name##_floyd(lgraph_t* g, lgraph_node_t s, int* out_T, int* out_C) { \
	return lgraph_floyd(g, s, out_T, out_C);			\
    }									\
    void lgraph_##name##_adjacency_lists_print(lgraph_t* g) {		\
	lgraph_adjacency_lists_print(g, print_##name);			\
    }

lgraph_type_definition(char, i8)
lgraph_type_definition(unsigned char, u8)
lgraph_type_definition(short, i16)
lgraph_type_definition(unsigned short, u16)
lgraph_type_definition(int, i32)
lgraph_type_definition(unsigned int, u32)
lgraph_type_definition(long, i64)
lgraph_type_definition(unsigned long, u64)
lgraph_type_definition(char, char)
lgraph_type_definition(float, f32)
lgraph_type_definition(double, f64)
#endif
