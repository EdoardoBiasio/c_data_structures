#include "clib.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <float.h>

size_t round_up_to_byte(int nbits) {
    int r = nbits % 8;
    return nbits + r;
}
size_t round_down_to_byte(int nbits) {
    int r = nbits % 8;
    return nbits - r;
}

int mgraph_init(mgraph_t* g, int vertex_size, int flags, int (*cmp)(void*, void*)) {
    if (!g)
	return -1;
    g->flags = flags;
    g->adjacency_matrix = NULL;
    g->vertex_data_size = vertex_size;
    array_init(&g->vertices, vertex_size + sizeof(mgraph_vertex_t), 8, cmp);
    g->cmp = cmp;
    return 0;
}

int mgraph_close(mgraph_t* g) {
    if (!g)
	return -1;
    if (g->adjacency_matrix)
	free(g->adjacency_matrix);
    g->adjacency_matrix = NULL;
    array_close(&g->vertices);
    g->vertex_data_size = 0;
    g->flags = 0;
    return 0;
}

mgraph_vertex_t* mgraph_vertex_alloc(mgraph_t* g, void* data) {
    mgraph_vertex_t* new = malloc(g->vertex_data_size + sizeof(mgraph_vertex_t));
    new->tag = new->flags = 0;
    memcpy((char*) new + sizeof(mgraph_vertex_t), data, g->vertex_data_size);
    return new;
}

int mgraph_connected(mgraph_t* g, mgraph_node_t l, mgraph_node_t r) {
    int idx, rem;
    if (!g || l >= g->vertices.len || r >= g->vertices.len)
	return -1;
    idx = l * g->vertices.len + r;
    rem = idx % 8;
    
    return (*((char*) g->adjacency_matrix + round_down_to_byte(idx)) & (0x1 << rem)) >> rem;
}

void mgraph_realloc_adjacency_matrix(mgraph_t* g, size_t old_vc) {
    size_t size = round_up_to_byte(g->vertices.len * g->vertices.len);
    void *new_matrix;
    size_t i, j, idx, r;
    new_matrix = malloc(size);
    memset(new_matrix, 0, size);
    for (i = 0; i < old_vc; i++) {
	for (j = 0; j < old_vc; j++) {
	    idx = i * old_vc + j;
	    r = idx % 8;
	    if (((*((char*) g->adjacency_matrix + round_down_to_byte(idx)) & (0x1 << r)) >> r)) {
		idx = i * g->vertices.len + j;
		r = idx % 8;
		*((char*) g->adjacency_matrix + round_down_to_byte(idx)) |= 0x1 << r;
	    }
	}
    }
    free(g->adjacency_matrix);
    g->adjacency_matrix = new_matrix;
}

mgraph_node_t mgraph_add_vertex(mgraph_t* g, void* vertex) {
    mgraph_vertex_t *new_vertex;
    size_t old_vc;
    
    if (!g || !vertex)
	return -1;
    new_vertex = mgraph_vertex_alloc(g, vertex);
    old_vc = g->vertices.len;
    array_push_back(&g->vertices, new_vertex);
    free(new_vertex);
    mgraph_realloc_adjacency_matrix(g, old_vc);
    return old_vc;
}

mgraph_node_t mgraph_add_vertices_carray(mgraph_t* g, void* vertices, int n) {
    array_t new;
    size_t i, old_vc;
    mgraph_vertex_t *temp = malloc(sizeof(mgraph_vertex_t) + g->vertex_data_size);
    array_init(&new, g->vertex_data_size + sizeof(mgraph_vertex_t), n, g->cmp);
    for (i = 0; i < n; i++) {
	temp->flags = temp->tag = 0;
	memcpy((char*) temp + sizeof(mgraph_vertex_t), (char*) vertices + i * g->vertex_data_size, g->vertex_data_size);
	array_push_back(&new, temp);
    }
    free(temp);
    old_vc = g->vertices.len;
    array_concat(&g->vertices, &new);
    array_close(&new);
    mgraph_realloc_adjacency_matrix(g, old_vc);
    return old_vc;
}

mgraph_node_t mgraph_add_vertices_array(mgraph_t* g, array_t *vertices) {
    int len;
    void* temp = array_to_carray(vertices, &len);
    return mgraph_add_vertices_carray(g, temp, len);
}

int mgraph_add_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r) {
    int idx, rem;
    if (!g)
	return -1;
    
    idx = l * g->vertices.len + r;
    rem = idx % 8;
    *((char*) g->adjacency_matrix + round_down_to_byte(idx)) |= 0x1 << rem;
    if (!(g->flags & graph_directed)) {
	idx = r * g->vertices.len + l;
	rem = idx % 8;
	*((char*) g->adjacency_matrix + round_down_to_byte(idx)) |= 0x1 << rem;
    }
    return 0;
}

int mgraph_remove_vertex(mgraph_t* g, mgraph_node_t v) {
    size_t i, old_vc;
    for (i = 0; i < g->vertices.len; i++) {
	mgraph_remove_edge(g, v, i);
	mgraph_remove_edge(g, i, v);
    }
    old_vc = g->vertices.len;
    array_delete_index(&g->vertices, v);
    mgraph_realloc_adjacency_matrix(g, old_vc);
    return 0;
}

int mgraph_remove_edge(mgraph_t* g, mgraph_node_t l, mgraph_node_t r) {
    int idx, rem;
    if (!g)
	return -1;
    idx = l * g->vertices.len + r;
    rem = idx % 8;
    *((char*) g->adjacency_matrix + round_down_to_byte(idx)) &= ~((char)0x1 << rem);
    if (!(g->flags & graph_directed)) {
	idx = r * g->vertices.len + l;
	rem = idx % 8;
	*((char*) g->adjacency_matrix + round_down_to_byte(idx)) &= ~((char)0x1 << rem);
    }
    return 0;
}

int mgraph_breadth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t* g, mgraph_node_t, void*), void* data) {
    lqueue_t queue;
    mgraph_node_t i, curr;
    mgraph_vertex_t* t;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++) {
	if (i != node) {
	    t = (mgraph_vertex_t*)array_at_ref(&g->vertices, i);
	    t->flags &= ~graph_vertex_visited;
	    t->dist = FLT_MAX;
	}
    }
    t = (mgraph_vertex_t*) array_at_ref(&g->vertices, node);
    t->flags |= graph_vertex_visited;
    lqueue_init(&queue, sizeof(mgraph_node_t), cmp_i64);
    lqueue_enqueue(&queue, &node);
    while (!lqueue_is_empty(&queue)) {
	lqueue_dequeue(&queue, &curr);
	func(g, curr, data);
	for (i = 0; i < g->vertices.len; i++) {
	    t = (mgraph_vertex_t*) array_at_ref(&g->vertices, i);

	    if (mgraph_connected(g, curr, i) && !(t->flags & graph_vertex_visited)) {
		t->flags |= graph_vertex_visited;
		t->dist = ((mgraph_vertex_t*) array_at_ref(&g->vertices, curr))->dist + 1;
		lqueue_enqueue(&queue, &i);
	    }
	}	
    }
    return 0;
}

int mgraph_depth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_t* g, mgraph_node_t, void*), void* data) {
    lstack_t stack;
    mgraph_node_t i, curr;
    mgraph_vertex_t* t;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++) {
	if (i != node) {
	    t = (mgraph_vertex_t*)array_at_ref(&g->vertices, i);
	    t->flags &= ~graph_vertex_visited;
	    t->dist = FLT_MAX;
	}
    }
    t = (mgraph_vertex_t*) array_at_ref(&g->vertices, node);
    t->flags |= graph_vertex_visited;
    lstack_init(&stack, sizeof(mgraph_node_t), cmp_i64);
    lstack_push(&stack, &node);
    while (!lstack_is_empty(&stack)) {
	lstack_pop(&stack, &curr);
	func(g, curr, data);
	for (i = 0; i < g->vertices.len; i++) {
	    t = (mgraph_vertex_t*) array_at_ref(&g->vertices, i);

	    if (mgraph_connected(g, curr, i) && !(t->flags & graph_vertex_visited)) {
		t->flags |= graph_vertex_visited;
		t->dist = ((mgraph_vertex_t*) array_at_ref(&g->vertices, curr))->dist + 1;
		lstack_push(&stack, &i);
	    }
	}      
    }
    
    return 0;
}

void mgraph_print_adjacency_matrix(mgraph_t* g, void (*print)(void*)) {
    mgraph_node_t i, j;
    if (!g)
	return;
    for (i = 0; i < g->vertices.len; i++) {
	print((char*) array_at_ref(&g->vertices, i) + sizeof(mgraph_vertex_t));
	printf(": ");
	for (j = 0; j < g->vertices.len; j++) {
	    if (mgraph_connected(g, i, j))
		printf("1");
	    else
		printf("0");
	    printf(" ");
	}
	printf("\n");
    }
    
}

void visit_and_tag(mgraph_t* g, mgraph_node_t node, void* data) {
    int tag = *(int*) data;
    mgraph_vertex_tag(g, node) = tag;
}

int mgraph_connected_components(mgraph_t* g) {
    size_t i;
    mgraph_vertex_t *t;
    int tag;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	t->tag = -1;
    }
    tag = 0;
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	if (t->tag < 0) {
	    mgraph_breadth_first_search(g, i, visit_and_tag, &tag);
	    tag++;
	}
    }
    return tag;
}

void mgraph_depth_first_search_topological_ordering(mgraph_t* g, mgraph_node_t node, list_t* out) {
    mgraph_vertex_t* t;
    size_t i;
    t = array_at_ref(&g->vertices, node);
    t->flags |= graph_vertex_visited;
    for (i = 0; i < g->vertices.len; i++) {
	if (mgraph_connected(g, node, i)) {
	    if (!((mgraph_vertex_flags(g, i)) & graph_vertex_visited))
		mgraph_depth_first_search_topological_ordering(g, i, out);
	}
    }
    list_i64_push_front(out, node);
}

int mgraph_topological_ordering(mgraph_t* g, list_t* out) {
    mgraph_vertex_t *t;
    size_t i;
    if (!g || !(g->flags & graph_directed))
	return -1;
    list_i64_init(out);
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	t->flags &= ~(graph_vertex_visited);
    }
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	if (!(t->flags & graph_vertex_visited)) {
	    mgraph_depth_first_search_topological_ordering(g, i, out);
	}
    }
    return 0;
}

int cmp_graph_vertex(void* l, void* r) {
    mgraph_vertex_t* left, * right;
    left = l;
    right = r;
    if (left->dist < right->dist)
	return -1;
    if (left->dist > right->dist)
	return 1;
    return 0;
}

int mgraph_dijkstra(mgraph_t* g, mgraph_node_t node, float weight(mgraph_node_t, mgraph_node_t)) {
    heap_t q;
    size_t i, j;
    int ndist;
    mgraph_vertex_t *v, u;
    if (!g || !(g->flags & graph_directed))
	return -1;
    min_heap_init(&q, 3, sizeof(mgraph_vertex_t), cmp_graph_vertex);
    mgraph_vertex_dist(g, node) = 0;
    for (i = 0; i < g->vertices.len; i++) {
	v = array_at_ref(&g->vertices, i);
	v->id = i;
	if (i != node) {
	    v->dist = FLT_MAX;
	    v->pred = -1;
	}
	min_heap_insert(&q, v);
    }

    while (!heap_is_empty(&q)) {
	min_heap_pop_min(&q, &u);
	for (i = 0; i < g->vertices.len; i++) {
	    if (mgraph_connected(g, u.id, i)) {
		v = array_at_ref(&g->vertices, i);
		ndist = u.dist + weight(u.id, i);
		if (v->dist > ndist) {
		    v->dist = ndist;
		    v->pred = u.id;
		    for (j = 0; j < q.size; j++) {
			if (cmp_graph_vertex((mgraph_vertex_t*) q.data + j, v) == 0)
			    ((mgraph_vertex_t*) q.data + j)->dist = ndist;
		    }
		}
	    }
	}
    }
    min_heap_close(&q);
    return 0;
}

mgraph_node_t mgraph_find_succ(mgraph_t* g, mgraph_node_t s) {
    size_t i;
    for (i = 0; i < g->vertices.len; i++) {
	if (mgraph_connected(g, s, i))
	    return i;
    }
    return -1;
}

int mgraph_floyd(mgraph_t* g, mgraph_node_t s, int* out_T, int* out_C) {
    mgraph_node_t t, h;
    int T, C;
    if (!g || !(g->flags & graph_directed))
	return -1;
    t = mgraph_find_succ(g, s);
    h = mgraph_find_succ(g, mgraph_find_succ(g, s));
    while (t != h) {
	t = mgraph_find_succ(g, t);
	h = mgraph_find_succ(g, mgraph_find_succ(g, h));
    }
    if (out_T) {
	T = 0;
	t = s;
	while (t != h) {
	    t = mgraph_find_succ(g, t);
	    h = mgraph_find_succ(g, h);
	    T++;
	}
	*out_T = T;
	if (out_C) {
	    h = t;
	    C = 0;
	    while (h != t) {
		h = mgraph_find_succ(g, h);
		C++;
	    }
	    *out_C = C;
	}
    }
    return 0;
}

int lgraph_init(lgraph_t* g, int vertex_size, int flags, int (*cmp)(void*, void*)) {
    if (!g)
	return -1;
    g->flags = flags;
    g->cmp = cmp;
    g->vertex_data_size = vertex_size;
    array_init(&g->vertices, vertex_size + sizeof(lgraph_vertex_t), 8, cmp); 
    return 0;
}

void lgraph_vertex_close(lgraph_t* g, size_t i) {
    lgraph_vertex_t *v = array_at_ref(&g->vertices, i);
    list_close(&v->adjacency_list);
}

int lgraph_close(lgraph_t* g) {
    size_t i;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++)
	lgraph_vertex_close(g, i);
    array_close(&g->vertices);
    g->vertex_data_size = 0;
    g->flags = 0;
    g->cmp = NULL;
    return 0;    
}

lgraph_vertex_t* lgraph_vertex_alloc(lgraph_t* g, void* data) {
    lgraph_vertex_t* new;
    new = malloc(sizeof(lgraph_vertex_t) + g->vertex_data_size);
    new->tag = new->flags = new->dist = new->pred = 0;
    new->id = 0;
    list_i64_init(&new->adjacency_list);
    return new;
}

lgraph_node_t lgraph_add_vertex(lgraph_t* g, void* vertex) {
    lgraph_vertex_t *new_vertex;
    
    if (!g || !vertex)
	return -1;
    new_vertex = lgraph_vertex_alloc(g, vertex);
    array_push_back(&g->vertices, new_vertex);
    free(new_vertex);
    return g->vertices.len - 1;
}

lgraph_node_t lgraph_add_vertices_carray(lgraph_t* g, void* vertices, int n) {
    array_t new;
    size_t i, old_vc;
    lgraph_vertex_t *temp = malloc(sizeof(lgraph_vertex_t) + g->vertex_data_size);
    array_init(&new, g->vertex_data_size + sizeof(lgraph_vertex_t), n, g->cmp);
    for (i = 0; i < n; i++) {
	temp->flags = temp->tag = temp->dist = temp->pred = temp->id = 0;
	list_i64_init(&temp->adjacency_list);
	memcpy((char*) temp + sizeof(lgraph_vertex_t), (char*) vertices + i * g->vertex_data_size, g->vertex_data_size);
	array_push_back(&new, temp);
    }
    free(temp);
    old_vc = g->vertices.len;
    array_concat(&g->vertices, &new);
    array_close(&new);
    return old_vc;    
}

lgraph_node_t lgraph_add_vertices_array(lgraph_t* g, array_t* vertices) {
    int len;
    void* temp = array_to_carray(vertices, &len);
    return lgraph_add_vertices_carray(g, temp, len);    
}

int lgraph_add_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r) {
    lgraph_vertex_t* lv, *rv;
    if (!g)
	return -1;
    
    lv = array_at_ref(&g->vertices, l);
    list_i64_push_front(&lv->adjacency_list, r);
    if (!(g->flags & graph_directed)) {
	rv = array_at_ref(&g->vertices, r);
	list_i64_push_front(&rv->adjacency_list, r);
    }
    return 0;
}

int lgraph_remove_vertex(lgraph_t* g, lgraph_node_t v) {
    size_t i, old_vc, j;
    for (i = 0; i < g->vertices.len; i++) {
	if (i != v) {
	    lgraph_remove_edge(g, v, i);
	    lgraph_remove_edge(g, i, v);
	}
    }
    
    old_vc = g->vertices.len;
    array_delete_index(&g->vertices, v);
    for (i = 0; i < g->vertices.len; i++) {
	for (j = v + 1; j < g->vertices.len + 1; j++) {
	    if (lgraph_connected(g, i, j)) {
		lgraph_remove_edge(g, i, j);
		lgraph_add_edge(g, i, j - 1);
	    }
	}
    }
    return 0;    
}

int lgraph_remove_edge(lgraph_t* g, lgraph_node_t l, lgraph_node_t r) {
    lgraph_vertex_t* lv,* rv;
    if (!g)
	return -1;
    lv = array_at_ref(&g->vertices, l);
    list_i64_delete(&lv->adjacency_list, r);
    if (!(g->flags & graph_directed)) {
	rv = array_at_ref(&g->vertices, r);
	list_i64_delete(&rv->adjacency_list, l);
    }
    return 0;
}

int lgraph_connected(lgraph_t* g, lgraph_node_t l, lgraph_node_t r) {
    lgraph_vertex_t* lv;
    if (!g || l >= g->vertices.len || r >= g->vertices.len)
	return -1;
    lv = array_at_ref(&g->vertices, l);
    return list_i64_search(&lv->adjacency_list, r) != NULL;
}

int lgraph_breadth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data) {
    lqueue_t queue;
    lgraph_node_t i, curr;
    lgraph_vertex_t* t;
    list_node_t* h;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++) {
	if (i != node) {
	    t = (lgraph_vertex_t*)array_at_ref(&g->vertices, i);
	    t->flags &= ~graph_vertex_visited;
	    t->dist = FLT_MAX;
	}
    }
    t = (lgraph_vertex_t*) array_at_ref(&g->vertices, node);
    t->flags |= graph_vertex_visited;
    lqueue_init(&queue, sizeof(lgraph_node_t), cmp_i64);
    lqueue_enqueue(&queue, &node);
    while (!lqueue_is_empty(&queue)) {
	lqueue_dequeue(&queue, &curr);
	func(g, curr, data);
	h = ((lgraph_vertex_t*)array_at_ref(&g->vertices, curr))->adjacency_list.head;
	
	while (h) {
	    t = array_at_ref(&g->vertices, *(long int*)list_node_data(h));
	    if (!(t->flags & graph_vertex_visited)) {
		t->flags |= graph_vertex_visited;
		t->dist = ((lgraph_vertex_t*) array_at_ref(&g->vertices, curr))->dist + 1;
		lqueue_enqueue(&queue, list_node_data(h));
	    }
	    h = h->next;
	}	
    }
    return 0;    
}

int lgraph_depth_first_search(lgraph_t* g, lgraph_node_t node, void (*func)(lgraph_t*, lgraph_node_t, void*), void* data) {
    lstack_t stack;
    lgraph_node_t i, curr;
    lgraph_vertex_t* t;
    list_node_t* h;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++) {
	if (i != node) {
	    t = (lgraph_vertex_t*)array_at_ref(&g->vertices, i);
	    t->flags &= ~graph_vertex_visited;
	    t->dist = FLT_MAX;
	}
    }
    t = (lgraph_vertex_t*) array_at_ref(&g->vertices, node);
    t->flags |= graph_vertex_visited;
    lstack_init(&stack, sizeof(lgraph_node_t), cmp_i64);
    lstack_push(&stack, &node);
    while (!lstack_is_empty(&stack)) {
	lstack_pop(&stack, &curr);
	func(g, curr, data);
	h = ((lgraph_vertex_t*) array_at_ref(&g->vertices, curr))->adjacency_list.head;

	while (h) {
	    t = (lgraph_vertex_t*) array_at_ref(&g->vertices, *(long int*)list_node_data(h));

	    if (!(t->flags & graph_vertex_visited)) {
		t->flags |= graph_vertex_visited;
		t->dist = ((mgraph_vertex_t*) array_at_ref(&g->vertices, curr))->dist + 1;
		lstack_push(&stack, list_node_data(h));
	    }
	    h = h->next;
	}      
    }
    
    return 0;
}


void lvisit_and_tag(lgraph_t* g, lgraph_node_t node, void* data) {
    int tag = *(int*) data;
    lgraph_vertex_t* v = array_at_ref(&g->vertices, node);
    v->tag = tag;
}

int lgraph_connected_components(lgraph_t* g) {
    size_t i;
    lgraph_vertex_t *t;
    int tag;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	t->tag = -1;
    }
    tag = 0;
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	if (t->tag < 0) {
	    lgraph_breadth_first_search(g, i, lvisit_and_tag, &tag);
	    tag++;
	}
    }
    return tag;
}
void lgraph_depth_first_search_topological_ordering(lgraph_t* g, lgraph_node_t node, list_t* out) {
    lgraph_vertex_t* t, *v;
    list_node_t *h;
    t = array_at_ref(&g->vertices, node);
    h = t->adjacency_list.head;
    t->flags |= graph_vertex_visited;
    while (h) {
	v = array_at_ref(&g->vertices, *(long int*) list_node_data(h)); 
	if (!(v->flags & graph_vertex_visited))
	    lgraph_depth_first_search_topological_ordering(g, *(long int*) list_node_data(h), out);
	h = h->next;
    }
    list_i64_push_front(out, node);
}
int lgraph_topological_ordering(lgraph_t* g, list_t* out) {
    lgraph_vertex_t *t;
    size_t i;
    if (!g || !(g->flags & graph_directed))
	return -1;
    list_i64_init(out);
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	t->flags &= ~(graph_vertex_visited);
    }
    for (i = 0; i < g->vertices.len; i++) {
	t = array_at_ref(&g->vertices, i);
	if (!(t->flags & graph_vertex_visited)) {
	    lgraph_depth_first_search_topological_ordering(g, i, out);
	}
    }
    return 0;
}

int lgraph_dijkstra(lgraph_t* g, lgraph_node_t node, float (*weight)(lgraph_node_t, lgraph_node_t)) {
    heap_t q;
    size_t i, j;
    int ndist;
    lgraph_vertex_t *v, u;
    list_node_t* h;
    if (!g || !(g->flags & graph_directed))
	return -1;
    min_heap_init(&q, 3, sizeof(lgraph_vertex_t), cmp_graph_vertex);
    /*lgraph_vertex_dist(g, node) = 0;*/
    v = array_at_ref(&g->vertices, node);
    v->dist = 0;
    for (i = 0; i < g->vertices.len; i++) {
	v = array_at_ref(&g->vertices, i);
	v->id = i;
	if (i != node) {
	    v->dist = FLT_MAX;
	    v->pred = -1;
	}
	min_heap_insert(&q, v);
    }

    while (!heap_is_empty(&q)) {
	min_heap_pop_min(&q, &u);
	h = u.adjacency_list.head;
	while (h) {
	    i = *(long int*) list_node_data(h);
	    v = array_at_ref(&g->vertices, i);
	    ndist = u.dist + weight(u.id, i);
	    if (v->dist > ndist) {
		v->dist = ndist;
		v->pred = u.id;
		for (j = 0; j < q.size; j++) {
		    if (cmp_graph_vertex((mgraph_vertex_t*) q.data + j, v) == 0)
			((mgraph_vertex_t*) q.data + j)->dist = ndist;
		}
	    }
	    h = h->next;
	}
    }
    min_heap_close(&q);
    return 0;
}

lgraph_node_t lgraph_find_succ(lgraph_t* g, lgraph_node_t s) {
    list_node_t* h = ((lgraph_vertex_t*) array_at_ref(&g->vertices, s))->adjacency_list.head;
    if (h)
	return *(long int*) list_node_data(h);
    return -1;
}

int lgraph_floyd(lgraph_t* g, lgraph_node_t s, int* out_T, int* out_C) {
    lgraph_node_t t, h;
    int T, C;
    if (!g || !(g->flags & graph_directed))
	return -1;
    t = lgraph_find_succ(g, s);
    h = lgraph_find_succ(g, lgraph_find_succ(g, s));
    while (t != h) {
	t = lgraph_find_succ(g, t);
	h = lgraph_find_succ(g, lgraph_find_succ(g, h));
    }
    if (out_T) {
	T = 0;
	t = s;
	while (t != h) {
	    t = lgraph_find_succ(g, t);
	    h = lgraph_find_succ(g, h);
	    T++;
	}
	*out_T = T;
	if (out_C) {
	    h = t;
	    C = 0;
	    while (h != t) {
		h = lgraph_find_succ(g, h);
		C++;
	    }
	    *out_C = C;
	}
    }
    return 0;
}

void lgraph_adjacency_lists_print(lgraph_t* g, void (*print)(void*)) {
    size_t i;
    lgraph_vertex_t* v;
    list_node_t* h;
    if (!g)
	return;
    for (i = 0; i < g->vertices.len; i++) {
	printf("-> ");
	v = array_at_ref(&g->vertices, i);
	print((char*) v + sizeof(lgraph_vertex_t));
	h = v->adjacency_list.head;
	while (h) {
	    printf(" %ld", *(long int*) list_node_data(h));
	    h = h->next;
	}
    }
}

