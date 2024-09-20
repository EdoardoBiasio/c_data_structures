#include "clib.h"
#include <stdlib.h>
#include <string.h>

lbinary_tree_node_t* lbinary_tree_node_alloc(size_t type_size, void* data) {
    lbinary_tree_node_t* new;
    if (!data)
	return NULL;
    new = (lbinary_tree_node_t*) malloc(sizeof(lbinary_tree_node_t) + type_size);
    memcpy(lbinary_tree_node_data(new), data, type_size);
    new->left = NULL;
    new->right = NULL;
    new->p = NULL;
    return new;
}

int binary_search_tree_insert(binary_search_tree_t* bst, void* element) {
    lbinary_tree_node_t* new, *c, *p;
    if (!bst || !element)
	return -1;
    if (bst->root) {
	new = lbinary_tree_node_alloc(bst->type_size, element);
	c = bst->root;
	p = NULL;
	while (c) {
	    p = c;
	    if (bst->cmp(element, lbinary_tree_node_data(c)) >= 0)
		c = c->right;
	    else
		c = c->left;
	}
	if (bst->cmp(element, lbinary_tree_node_data(p)) >= 0)
	    p->right = new;
	else
	    p->left = new;
	new->p = p;
    } else {
	new = lbinary_tree_node_alloc(bst->type_size, element);
	bst->root = new;
    }
    return 0;
}

int binary_search_tree_delete(binary_search_tree_t* bst, void* element) {
    binary_search_tree_node_t* node, *todel, *subtree;
    if (!bst || !element)
	return -1;
    node = binary_search_tree_search_node(bst, bst->root, element);
    if (!node)
	return -1;
    if (node->left == NULL || node->right == NULL)
	todel = node;
    else
	todel = binary_search_tree_successor_node(bst, node);
    if (todel->left)
	subtree = todel->left;
    else
	subtree = todel->right;
    if (subtree)
	subtree->p = todel->p;
    if (todel->p == NULL)
	bst->root = subtree;
    else if (todel == todel->p->left)
	todel->p->left = subtree;
    else
	todel->p->right = subtree;
    if (todel != node)
	memcpy(lbinary_tree_node_data(node), lbinary_tree_node_data(todel), bst->type_size);
    free(todel);
    return 0;
}

int binary_search_tree_search(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* key, void* out) {
    binary_search_tree_node_t* t;
    if (!bst || !node || !key || !out)
	return -1;
    t = binary_search_tree_search_node(bst, node, key);
    if (!t)
	return -1;
    memcpy(out, lbinary_tree_node_data(t), bst->type_size);
    return 0;
}

void* binary_search_tree_search_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* key) {
    binary_search_tree_node_t* t;
    if (!bst || !node || !key)
	return NULL;
    t = binary_search_tree_search_node(bst, node, key);
    if (!t)
	return NULL;
    return lbinary_tree_node_data(t);
}

binary_search_tree_node_t* binary_search_tree_search_node(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* key) {
    int res;
    if (!node || !key)
	return NULL;
    res = bst->cmp(lbinary_tree_node_data(node), key);
    if (res == 0)
	return node;
    if (res < 0)
	return binary_search_tree_search_node(bst, node->right, key);
    return binary_search_tree_search_node(bst, node->left, key);
}

int binary_search_tree_min(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out) {
    binary_search_tree_node_t* t;
    if (!bst || !node || !out)
	return -1;
    t = binary_search_tree_min_node(bst, node);
    if (!t)
	return -1;
    memcpy(out, lbinary_tree_node_data(t), bst->type_size);
    return 0;
}

int binary_search_tree_max(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out) {
    binary_search_tree_node_t* t;
    if (!bst || !node || !out)
	return -1;
    t = binary_search_tree_max_node(bst, node);
    if (!t)
	return -1;
    memcpy(out, lbinary_tree_node_data(t), bst->type_size);
    return 0;
}

int binary_search_tree_successor(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out) {
    binary_search_tree_node_t* t;
    if (!bst || !node || !out)
	return -1;
    t = binary_search_tree_successor_node(bst, node);
    if (!t)
	return -1;
    memcpy(out, lbinary_tree_node_data(t), bst->type_size);
    return 0;
}

int binary_search_tree_predecessor(binary_search_tree_t* bst, binary_search_tree_node_t* node, void* out) {
    binary_search_tree_node_t* t;
    if (!bst || !node || !out)
	return -1;
    t = binary_search_tree_predecessor_node(bst, node);
    if (!t)
	return -1;
    memcpy(out, lbinary_tree_node_data(t), bst->type_size);
    return 0;
}

void* binary_search_tree_min_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node) {
    binary_search_tree_node_t* t;
    if (!bst || !node)
	return NULL;
    t = binary_search_tree_min_node(bst, node);
    if (!t)
	return NULL;
    return lbinary_tree_node_data(t);
}

void* binary_search_tree_max_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node) {
    binary_search_tree_node_t* t;
    if (!bst || !node)
	return NULL;
    t = binary_search_tree_max_node(bst, node);
    if (!t)
	return NULL;
    return lbinary_tree_node_data(t);
}

void* binary_search_tree_successor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node) {
    binary_search_tree_node_t* t;
    if (!bst || !node)
	return NULL;
    t = binary_search_tree_successor_node(bst, node);
    if (!t)
	return NULL;
    return lbinary_tree_node_data(t);
}

void* binary_search_tree_predecessor_ref(binary_search_tree_t* bst, binary_search_tree_node_t* node)  {
    binary_search_tree_node_t* t;
    if (!bst || !node)
	return NULL;
    t = binary_search_tree_predecessor_node(bst, node);
    if (!t)
	return NULL;
    return lbinary_tree_node_data(t);
}

binary_search_tree_node_t* binary_search_tree_min_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) {
    if (!bst || !node)
	return NULL;
    while (node->left)
	node = node->left;
    return node;
}

binary_search_tree_node_t* binary_search_tree_max_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) {
    if (!bst || !node)
	return NULL;
    while (node->right)
	node = node->right;
    return node;
}

binary_search_tree_node_t* binary_search_tree_successor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) {
    binary_search_tree_node_t* p;
    if (!bst || !node)
	return NULL;
    if (node->right)
	return binary_search_tree_min_node(bst, node->right);
    p = node->p;
    while (p && p->right == node) {
	node = p;
	p = p->p;
    }
    return p;
}

binary_search_tree_node_t* binary_search_tree_predecessor_node(binary_search_tree_t* bst, binary_search_tree_node_t* node) {
    binary_search_tree_node_t* p;
    if (!bst || !node)
	return NULL;
    if (node->left)
	return binary_search_tree_max_node(bst, node->left);
    p = node->p;
    while (p && p->left == node) {
	node = p;
	p = p->p;
    }
    return p;
}
binary_search_tree_type_implementation(char, i8)
binary_search_tree_type_implementation(unsigned char, u8)
binary_search_tree_type_implementation(short, i16)
binary_search_tree_type_implementation(unsigned short, u16)
binary_search_tree_type_implementation(int, i32)
binary_search_tree_type_implementation(unsigned int, u32)
binary_search_tree_type_implementation(long, i64)
binary_search_tree_type_implementation(unsigned long, u64)
binary_search_tree_type_implementation(char, char)
binary_search_tree_type_implementation(float, f32)
binary_search_tree_type_implementation(double, f64)
