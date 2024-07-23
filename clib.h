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
int array_copy(array_t* dest, array_t* src);
int array_concat(array_t* dest, array_t* src);
void* array_to_carray_consume(array_t* src, int* out_len);
void* array_to_carray(array_t* src, int* out_len);

int array_quicksort(array_t* array, size_t lo, size_t hi);
int array_insertionsort(array_t* array);
int array_mergesort(array_t* array, int p, int r);
int array_selectionsort(array_t* array);

#define array_push_front(a, el) array_insert(a, 0, el)
#define array_push_back(a, el) array_insert(a, (a)->len, el)

typedef array_t sorted_array_t;
#define sorted_array_init(a, ts, cap, cmp) array_init((array_t*)a, ts, cap, cmp)
#define sorted_array_at(a, idx, out) array_at((array_t*) a, idx, out)
#define sorted_array_at_ref(a, idx) array_at_ref((array_t*) a, idx)
#define sorted_array_close(a) array_close((array_t*)a)
int sorted_array_insert(sorted_array_t* array, void* element);
int sorted_array_bsearch(sorted_array_t* array, void* key, void* out);
#define sorted_array_print(a, print) array_print((array_t*)a, print)
int sorted_array_from_array(sorted_array_t* dest, array_t* src);
    

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


typedef struct lstack_s {
    list_t elements;
} lstack_t;

int lstack_init(lstack_t* stack, int type_size, int (*cmp)(void*, void*));
int lstack_close(lstack_t* stack);
int lstack_is_empty(lstack_t* stack);
int lstack_peek(lstack_t* stack, void* out);
void* lstack_peek_ref(lstack_t* stack);
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
void* astack_peek_ref(astack_t* stack);
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
void* lqueue_peek_ref(lqueue_t* queue);
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
void* aqueue_peek_ref(aqueue_t* queue);
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
void* ldeque_peek_front_ref(ldeque_t* deque);
void* ldeque_peek_back_ref(ldeque_t* deque);
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
void* adeque_peek_front_ref(adeque_t* deque);
void* adeque_peek_back_ref(adeque_t* deque);
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
int rb_tree_close(rb_tree_t* tree);
int rb_tree_visit_in_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data);
int rb_tree_visit_pre_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data);
int rb_tree_visit_post_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data);
void rb_tree_print_func(rb_tree_node_t* node, void* data);
#define rb_tree_print_in_order(node, print) rb_tree_visit_in_order(node, rb_tree_print_func, print)
#define rb_tree_print_pre_order(node, print) rb_tree_visit_pre_order(node, rb_tree_print_func, print)
#define rb_tree_print_post_order(node, print) rb_tree_visit_post_order(node, rb_tree_print_func, print)
int rb_tree_insert(rb_tree_t* tree, void* element);
int rb_tree_delete(rb_tree_t* tree, void* element);
int rb_tree_search(rb_tree_t* tree, rb_tree_node_t* node, void* element, void* out);
void* rb_tree_search_ref(rb_tree_t* tree, rb_tree_node_t* node, void* element);
rb_tree_node_t* rb_tree_search_node(rb_tree_t* tree, rb_tree_node_t* node, void* element);
rb_tree_node_t* rb_tree_min_node(rb_tree_t* tree, rb_tree_node_t* node);
rb_tree_node_t* rb_tree_max_node(rb_tree_t* tree, rb_tree_node_t* node);
rb_tree_node_t* rb_tree_successor_node(rb_tree_t* tree, rb_tree_node_t* node);
rb_tree_node_t* rb_tree_predecessor_node(rb_tree_t* tree, rb_tree_node_t* node);
void* rb_tree_min_ref(rb_tree_t* tree, rb_tree_node_t* node);
void* rb_tree_max_ref(rb_tree_t* tree, rb_tree_node_t* node);
void* rb_tree_successor_ref(rb_tree_t* tree, rb_tree_node_t* node);
void* rb_tree_predecessor_ref(rb_tree_t* tree, rb_tree_node_t* node);
int rb_tree_min(rb_tree_t* tree, rb_tree_node_t* node, void* out);
int rb_tree_max(rb_tree_t* tree, rb_tree_node_t* node, void* out);
int rb_tree_successor(rb_tree_t* tree, rb_tree_node_t* node, void* out);
int rb_tree_predecessor(rb_tree_t* tree, rb_tree_node_t* node, void* out);

typedef struct heap_s {
    void* data;
    size_t size, capacity, height, type_size;
    int (*cmp)(void*, void*);
} heap_t;

typedef heap_t min_heap_t;
typedef heap_t max_heap_t;
#define min_heap_type 0
#define max_heap_type 1

#define heap_left(i) ((i * 2) + 1)
#define heap_right(i) ((i * 2) + 2)
#define heap_parent(i) (int)((i - 1) / 2)

int heap_init(heap_t* heap, size_t height, size_t type_size, int (*cmp)(void*, void*), int type);
int heap_close(heap_t* heap);
int heap_insert(heap_t* heap, void* data, int type);
int heap_pop_root(heap_t* heap, void* out, int type);
int heap_peek_root(heap_t* heap, void* out, int type);
void* heap_peek_root_ref(heap_t* heap, int type);
int heap_init_from_carray(heap_t* heap, void* array, int n, int type_size, int (*cmp)(void*, void*), int type);
int heap_init_from_array(heap_t* heap, array_t* array, int type);
int heap_is_empty(heap_t* heap);

#define max_heap_init(heap, height, ts, cmp) heap_init(heap, height, ts, cmp, max_heap_type)
#define max_heap_close(heap) heap_close(heap)
#define max_heap_insert(heap, data) heap_insert(heap, data, max_heap_type)
#define max_heap_pop_max(heap, out) heap_pop_root(heap, out, max_heap_type)
#define max_heap_max(heap, out) heap_peek_root(heap, out, max_heap_type)
#define max_heap_max_ref(heap) heap_peek_root_ref(heap, max_heap_type)
#define max_heap_init_from_carray(heap, arr, n, ts, cmp) heap_init_from_carray(heap, arr, n, ts, cmp, max_heap_type)
#define max_heap_init_from_array(heap, arr) heap_init_from_array(heap, arr, max_heap_type)

#define min_heap_init(heap, height, ts, cmp) heap_init(heap, height, ts, cmp, min_heap_type)
#define min_heap_close(heap) heap_close(heap)
#define min_heap_insert(heap, data) heap_insert(heap, data, min_heap_type)
#define min_heap_pop_min(heap, out) heap_pop_root(heap, out, min_heap_type)
#define min_heap_min(heap, out) heap_peek_root(heap, out, min_heap_type)
#define min_heap_min_ref(heap) heap_peek_root_ref(heap, min_heap_type)
#define min_heap_init_from_carray(heap, arr, n, ts, cmp) heap_init_from_carray(heap, arr, n, ts, cmp, min_heap_type)
#define min_heap_init_from_array(heap, arr) heap_init_from_array(heap, arr, min_heap_type)

int array_heap_sort(array_t* array);

#define graph_directed (1 << 0)
#define graph_connected (1 << 1)
#define graph_completely_connected (1 << 2)

#define graph_vertex_visited (1 << 0)

typedef struct mgraph_vertex_s {
    int tag;
    unsigned int flags;
    float dist;
    long int pred, id;
    /* vertex data */
    
} mgraph_vertex_t;

typedef struct mgraph_s {
    void* adjacency_matrix;
    array_t vertices;
    size_t vertex_data_size;
    int flags;
    int (*cmp)(void*, void*);
} mgraph_t;

typedef long int mgraph_node_t;
#define mgraph_node_invalid -1
#define mgraph_vertex_tag(gptr, i) (((mgraph_vertex_t*)array_at_ref(&(gptr)->vertices, i))->tag)
#define mgraph_vertex_flags(gptr, i) (((mgraph_vertex_t*)array_at_ref(&(gptr)->vertices, i))->flags)
#define mgraph_vertex_dist(gptr, i) (((mgraph_vertex_t*)array_at_ref(&(gptr)->vertices, i))->dist)
#define mgraph_vertex_data(gptr, i) (((char*)array_at_ref(&(gptr)->vertices, i)) + sizeof(mgraph_vertex_t))

int mgraph_init(mgraph_t* g, int vertex_size, int flags, int (*cmp)(void*, void*));
int mgraph_close(mgraph_t* g);
mgraph_node_t mgraph_add_vertex(mgraph_t* g, void* vertex);
mgraph_node_t mgraph_add_vertices_carray(mgraph_t* g, void* vertices, int n);
mgraph_node_t mgraph_add_vertices_array(mgraph_t* g, array_t* vertices);
int mgraph_add_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r);
int mgraph_remove_vertex(mgraph_t* g, mgraph_node_t v);
int mgraph_remove_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r);
int mgraph_connected(mgraph_t* g, mgraph_node_t l, mgraph_node_t r);
int mgraph_breadth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t*, mgraph_node_t, void*), void* data);
int mgraph_depth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t*, mgraph_node_t, void*), void* data);
void mgraph_print_adjacency_matrix(mgraph_t* g, void (*print)(void*));
int mgraph_connected_components(mgraph_t* g);
int mgraph_topological_ordering(mgraph_t* g, list_t* out);
int mgraph_dijkstra(mgraph_t* g, mgraph_node_t node, float (*weight)(mgraph_node_t, mgraph_node_t));
int mgraph_floyd(mgraph_t* g, mgraph_node_t s, int* out_T, int* out_C);

typedef struct lgraph_vertex_s {
    int tag;
    unsigned int flags;
    float dist;
    long int pred, id;
    list_t adjacency_list;
    /* data */
} lgraph_vertex_t;

typedef struct lgraph_s {
    array_t vertices;
    size_t vertex_data_size;
    int flags;
    int (*cmp)(void*, void*);
} lgraph_t;

typedef long int lgraph_node_t;

int lgraph_init(lgraph_t* g, int vertex_size, int flags, int (*cmp)(void*, void*));
int lgraph_close(lgraph_t* g);
lgraph_node_t lgraph_add_vertex(lgraph_t* g, void* vertex);
lgraph_node_t lgraph_add_vertices_carray(lgraph_t* g, void* vertices, int n);
lgraph_node_t lgraph_add_vertices_array(lgraph_t* g, array_t* vertices);
int lgraph_add_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r);
int lgraph_remove_vertex(lgraph_t* g, lgraph_node_t v);
int lgraph_remove_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r);
int lgraph_connected(lgraph_t* g, lgraph_node_t l, lgraph_node_t r);
int lgraph_breadth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data);
int lgraph_depth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data);
int lgraph_connected_components(lgraph_t* g);
int lgraph_topological_ordering(lgraph_t* g, list_t* out);
int lgraph_dijkstra(lgraph_t* g, lgraph_node_t node, float (*weight)(lgraph_node_t, lgraph_node_t));
int lgraph_floyd(lgraph_t* g, lgraph_node_t s, int* out_T, int* out_C);
void lgraph_adjacency_lists_print(lgraph_t* g, void (*print)(void*));

#include "macros.h"

#endif /* CLIB_H */
