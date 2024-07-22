#include "clib.h"
#include <string.h>
#include <stdlib.h>
#include <limits.h>

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
    array_init(&g->vertices, vertex_size + sizeof(graph_vertex_t), 8, cmp); 
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

graph_vertex_t* graph_vertex_alloc(mgraph_t* g, void* data) {
    graph_vertex_t* new = malloc(g->vertex_data_size + sizeof(graph_vertex_t));
    new->tag = new->flags = 0;
    memcpy((char*) new + sizeof(graph_vertex_t), data, g->vertex_data_size);
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
    graph_vertex_t *new_vertex;
    size_t old_vc;
    
    if (!g || !vertex)
	return -1;
    new_vertex = graph_vertex_alloc(g, vertex);
    old_vc = g->vertices.len;
    array_push_back(&g->vertices, new_vertex);
    free(new_vertex);
    mgraph_realloc_adjacency_matrix(g, old_vc);
    return old_vc;
}

mgraph_node_t mgraph_add_vertices_carray(mgraph_t* g, void* vertices, int n) {
    array_t new;
    size_t i, old_vc;
    graph_vertex_t *temp = malloc(sizeof(graph_vertex_t) + g->vertex_data_size);
    array_init(&new, g->vertex_data_size + sizeof(graph_vertex_t), n, g->cmp);
    for (i = 0; i < n; i++) {
	temp->flags = temp->tag = 0;
	memcpy((char*) temp + sizeof(graph_vertex_t), (char*) vertices + i * g->vertex_data_size, g->vertex_data_size);
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
    if (!(g->flags & graph_oriented)) {
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
    if (!(g->flags & graph_oriented)) {
	idx = r * g->vertices.len + l;
	rem = idx % 8;
	*((char*) g->adjacency_matrix + round_down_to_byte(idx)) &= ~((char)0x1 << rem);
    }
    return 0;
}

int mgraph_breadth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_node_t, void*), void* data) {
    lqueue_t queue;
    mgraph_node_t i, curr;
    graph_vertex_t* t;
    if (!g)
	return -1;
    for (i = 0; i < g->vertices.len; i++) {
	if (i != node) {
	    t = (graph_vertex_t*)array_at_ref(&g->vertices, i);
	    t->flags &= ~graph_vertex_visited;
	    t->dist = INT_MAX;
	}
    }
    t = (graph_vertex_t*) array_at_ref(&g->vertices, node);
    t->flags |= graph_vertex_visited;
    lqueue_init(&queue, sizeof(mgraph_node_t), cmp_i64);
    lqueue_enqueue(&queue, &node);
    while (!lqueue_is_empty(&queue)) {
	lqueue_dequeue(&queue, &curr);
	func(curr, data);
	for (i = 0; i < g->vertices.len; i++) {
	    t = (graph_vertex_t*) array_at_ref(&g->vertices, i);

	    if (mgraph_connected(g, curr, i) && !(t->flags & graph_vertex_visited)) {
		t->flags |= graph_vertex_visited;
		t->dist = ((graph_vertex_t*) array_at_ref(&g->vertices, curr))->dist + 1;
		lqueue_enqueue(&queue, &i);
	    }
	}	
    }
    return 0;
}

int mgraph_depth_first_search(mgraph_t* g, mgraph_node_t node, void (*func)(mgraph_node_t, void*), void* data) {
    return 0;
}

void mgraph_print_adjacency_matrix(mgraph_t* g, void (*print)(void*)) {
    mgraph_node_t i, j;
    if (!g)
	return;
    for (i = 0; i < g->vertices.len; i++) {
	print((char*) array_at_ref(&g->vertices, i) + sizeof(graph_vertex_t));
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

