#include "clib.h"
#include <stdlib.h>

int lbinary_tree_init(lbinary_tree_t* tree, int type_size, int (*cmp)(void*, void*)) {
    if (!tree || !cmp)
	return -1;
    tree->root = NULL;
    tree->type_size = type_size;
    tree->cmp = cmp;
    tree->size = 0;
    return 0;
}

void lbinary_tree_node_free(lbinary_tree_node_t* node, void* data) {
    (void) data;
    node->left = NULL;
    node->right = NULL;
    if (node->p) {
	if (node->p->left == node)
	    node->p->left = NULL;
	else
	    node->p->right = NULL;
    }
    free(node);
}

int lbinary_tree_close(lbinary_tree_t* tree) {
    if (!tree)
	return -1;
    lbinary_tree_visit_post_order(tree->root, lbinary_tree_node_free, NULL);
    tree->root = NULL;
    tree->type_size = tree->size = 0;
    tree->cmp = NULL;
    return 0;
}


void lbinary_tree_visit_in_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data) {
    if (node) {
	lbinary_tree_visit_in_order(node->left, func, data);
	func(node, data);
	lbinary_tree_visit_in_order(node->right, func, data);
    }
}

void lbinary_tree_visit_pre_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data) {
    if (node) {
	func(node, data);
	lbinary_tree_visit_pre_order(node->left, func, data);
	lbinary_tree_visit_pre_order(node->right, func, data);
    }
}

void lbinary_tree_visit_post_order(lbinary_tree_node_t* node, void (*func)(lbinary_tree_node_t*, void*), void* data) {
    if (node) {
	lbinary_tree_visit_post_order(node->left, func, data);
	lbinary_tree_visit_post_order(node->right, func, data);
	func(node, data);
    }
}

void lbinary_tree_print_func(lbinary_tree_node_t* node, void* data) {
    void (*print)(void*) = data;
    print(node);
}

int abinary_tree_init(abinary_tree_t* tree, int type_size, int capacity, int (*cmp)(void*, void*)) {
    if (!tree || !cmp)
	return -1;
    tree->size = 0;
    tree->capacity = capacity;
    tree->type_size = type_size;
    tree->data = calloc(type_size, capacity);
    tree->cmp = cmp;
    return 0;
}

int abinary_tree_close(abinary_tree_t* tree) {
    if (!tree)
	return -1;
    if (tree->data)
	free(tree->data);
    tree->data = NULL;
    tree->size = 0;
    tree->cmp = NULL;
    tree->type_size = 0;
    return 0;
}

void abinary_tree_visit_in_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data) {
    if (!tree || i >= tree->size)
	return;
    abinary_tree_visit_in_order(tree, abinary_tree_node_left(i), func, data);
    func(tree, i, data);
    abinary_tree_visit_in_order(tree, abinary_tree_node_right(i), func, data);
}

void abinary_tree_visit_pre_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data) {
    if (!tree || i >= tree->size)
	return;
    func(tree, i, data);
    abinary_tree_visit_in_order(tree, abinary_tree_node_right(i), func, data);
    abinary_tree_visit_in_order(tree, abinary_tree_node_left(i), func, data);
}
void abinary_tree_visit_post_order(abinary_tree_t* tree, int i, void (*func)(abinary_tree_t*, int, void*), void* data) {
    if (!tree || i >= tree->size)
	return;
    abinary_tree_visit_in_order(tree, abinary_tree_node_right(i), func, data);
    abinary_tree_visit_in_order(tree, abinary_tree_node_left(i), func, data);
    func(tree, i, data);
}

void abinary_tree_print_func(abinary_tree_t* tree, int i, void* data) {
    void (*print)(void*) = data;
    if (!tree || i >= tree->size)
	return;
    print((char*) tree->data + i * tree->type_size);
}

lbinary_tree_type_implementation(char, i8)
lbinary_tree_type_implementation(unsigned char, u8)
lbinary_tree_type_implementation(short, i16)
lbinary_tree_type_implementation(unsigned short, u16)
lbinary_tree_type_implementation(int, i32)
lbinary_tree_type_implementation(unsigned int, u32)
lbinary_tree_type_implementation(long, i64)
lbinary_tree_type_implementation(unsigned long, u64)
lbinary_tree_type_implementation(char, char)
lbinary_tree_type_implementation(float, f32)
lbinary_tree_type_implementation(double, f64)

abinary_tree_type_implementation(char, i8)
abinary_tree_type_implementation(unsigned char, u8)
abinary_tree_type_implementation(short, i16)
abinary_tree_type_implementation(unsigned short, u16)
abinary_tree_type_implementation(int, i32)
abinary_tree_type_implementation(unsigned int, u32)
abinary_tree_type_implementation(long, i64)
abinary_tree_type_implementation(unsigned long, u64)
abinary_tree_type_implementation(char, char)
abinary_tree_type_implementation(float, f32)
abinary_tree_type_implementation(double, f64)

