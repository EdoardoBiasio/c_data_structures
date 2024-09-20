#include "clib.h"
#include <stdlib.h>
#include <string.h>

void rb_tree_node_free(rb_tree_node_t* node, void* data) {
    if (node) {
	rb_tree_node_ptr(node)->left = NULL;
	rb_tree_node_ptr(node)->right = NULL;
	if (rb_tree_node_ptr(node)->p) {
	    if (rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left == node)
		rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left = NULL;
	    else
		rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right = NULL;
	}
	
	free(rb_tree_node_ptr(node));
    }
}

int rb_tree_close(rb_tree_t* tree) {
    if (!tree)
	return -1;
    rb_tree_visit_post_order(tree->root, rb_tree_node_free, NULL);
    tree->root = NULL;
    tree->type_size = tree->size = 0;
    tree->cmp = NULL;
    return 0;
}

rb_tree_node_t* rb_tree_node_alloc(size_t type_size, void* data) {
    rb_tree_node_t* new;
    if (!data)
	return NULL;
    new = (rb_tree_node_t*) malloc(sizeof(rb_tree_node_t) + type_size);
    rb_tree_node_set_red(new);
    memcpy(rb_tree_node_data(new), data, type_size);
    rb_tree_node_ptr(new)->left = NULL;
    rb_tree_node_ptr(new)->right = NULL;
    rb_tree_node_ptr(new)->p = NULL;
    return new;
}

void rb_tree_left_rotate(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* c;
    c = rb_tree_node_ptr(node)->right;
    rb_tree_node_ptr(node)->right = rb_tree_node_ptr(c)->left;
    if (rb_tree_node_ptr(c)->left)
	rb_tree_node_ptr(rb_tree_node_ptr(c)->left)->p = node;
    rb_tree_node_ptr(c)->p = rb_tree_node_ptr(node)->p;
    if (!rb_tree_node_ptr(node)->p)
	tree->root = c;
    else if (node == rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)
	rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left = c;
    else
	rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right = c;
    rb_tree_node_ptr(c)->left = node;
    rb_tree_node_ptr(node)->p = c;
}

void rb_tree_right_rotate(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* c;
    c = rb_tree_node_ptr(node)->left;
    rb_tree_node_ptr(node)->left = rb_tree_node_ptr(c)->right;
    if (rb_tree_node_ptr(c)->right)
	rb_tree_node_ptr(rb_tree_node_ptr(c)->right)->p = node;
    rb_tree_node_ptr(c)->p = rb_tree_node_ptr(node)->p;
    if (!rb_tree_node_ptr(node)->p)
	tree->root = c;
    else if (node == rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)
	rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left = c;
    else
	rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right = c;
    rb_tree_node_ptr(c)->right = node;
    rb_tree_node_ptr(node)->p = c;
}

void rb_tree_node_set_color(rb_tree_t* tree, rb_tree_node_t* node, int color) {
    if (color == rb_tree_red) {
	if (rb_tree_node_ptr(node)->p) {
	    if (node == rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)
		rb_tree_node_set_red(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left);
	    else
		rb_tree_node_set_red(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right);
	}
	if (rb_tree_node_ptr(node)->left)
	    rb_tree_node_set_red(rb_tree_node_ptr(rb_tree_node_ptr(node)->left)->p);
	if (rb_tree_node_ptr(node)->right)
	  rb_tree_node_set_red(rb_tree_node_ptr(rb_tree_node_ptr(node)->right)->p);  
    } else {
	if (rb_tree_node_ptr(node)->p) {
	    if (node == rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)
		rb_tree_node_set_black(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left);
	    else
		rb_tree_node_set_black(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right);
	}
	if (rb_tree_node_ptr(node)->left)
	    rb_tree_node_set_black(rb_tree_node_ptr(rb_tree_node_ptr(node)->left)->p);
	if (rb_tree_node_ptr(node)->right)
	  rb_tree_node_set_black(rb_tree_node_ptr(rb_tree_node_ptr(node)->right)->p);  
    }
}

void rb_tree_insert_fix(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* c;
    while (rb_tree_node_color(rb_tree_node_ptr(node)->p) == rb_tree_red) {
	if (rb_tree_node_ptr(node)->p == rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p)->left) {
	    c = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p)->right;
	    if (rb_tree_node_color(c) == rb_tree_red) {
		rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_black);
		rb_tree_node_set_color(tree, c, rb_tree_black);
		rb_tree_node_set_black(c);
		rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p, rb_tree_red);
		node = rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p;
	    } else {
		if (node == rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right) {
		    node = rb_tree_node_ptr(node)->p;
		    rb_tree_left_rotate(tree, node);
		} 
		rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_black);
		rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p, rb_tree_red);
		rb_tree_right_rotate(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p);
	    }
	} else {
	    c = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p)->left;
	    if (rb_tree_node_color(c) == rb_tree_red) {
		rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_black);
		rb_tree_node_set_color(tree, c, rb_tree_black);
		rb_tree_node_set_black(c);
		rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p, rb_tree_red);
		node = rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p;
	    } else {
		if (node == rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left) {
		    node = rb_tree_node_ptr(node)->p;
		    rb_tree_right_rotate(tree, node);
		}
		rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_black);
		rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p, rb_tree_red);
		rb_tree_left_rotate(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->p);
	    }
	}
    }
    rb_tree_node_set_color(tree, tree->root, rb_tree_black);
    rb_tree_node_set_black(tree->root);
}

int rb_tree_insert(rb_tree_t* tree, void* element) {
    rb_tree_node_t* new, *c, *p;
    if (!tree || !element)
	return -1;
    if (tree->root) {
	new = rb_tree_node_alloc(tree->type_size, element);
	c = tree->root;
	p = NULL;
	while (c) {
	    p = c;
	    if (tree->cmp(element, rb_tree_node_data(c)) >= 0)
		c = rb_tree_node_ptr(c)->right;
	    else
		c = rb_tree_node_ptr(c)->left;
	}
	if (tree->cmp(element, rb_tree_node_data(p)) >= 0)
	    rb_tree_node_ptr(p)->right = new;
	else
	    rb_tree_node_ptr(p)->left = new;
	rb_tree_node_ptr(new)->p = p;
	rb_tree_insert_fix(tree, new);
	tree->size++;
    } else {
	new = rb_tree_node_alloc(tree->type_size, element);
	tree->root = new;
	rb_tree_node_set_black(tree->root);
	tree->size++;
	return 0;
    }
    
    return 0;
}

rb_tree_node_t* rb_tree_min_node(rb_tree_t* tree, rb_tree_node_t* node) {
    while (rb_tree_node_ptr(node)->left) {
	node = rb_tree_node_ptr(node)->left;
    }
    return node;
}

rb_tree_node_t* rb_tree_max_node(rb_tree_t* tree, rb_tree_node_t* node) {
    while (rb_tree_node_ptr(node)->right) {
	node = rb_tree_node_ptr(node)->right;
    }
    return node;
}


rb_tree_node_t* rb_tree_search_node(rb_tree_t* tree, rb_tree_node_t* node, void* element) {
    int res;
    while (node) {
	res = tree->cmp(element, rb_tree_node_data(node));
	if (res == 0)
	    return node;
	else if (res > 0)
	    node = rb_tree_node_ptr(node)->right;
	else
	    node = rb_tree_node_ptr(node)->left;
    }
    return NULL;
}

rb_tree_node_t* rb_tree_successor_node(rb_tree_t* tree, rb_tree_node_t* node) {
    if (rb_tree_node_ptr(node)->right) {
	return rb_tree_min_node(tree, rb_tree_node_ptr(node)->right);
    }
    while (rb_tree_node_ptr(node)->p && rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right == node) {
	node = rb_tree_node_ptr(node)->p;
    }
    return rb_tree_node_ptr(node)->p;
}

rb_tree_node_t* rb_tree_predecessor_node(rb_tree_t* tree, rb_tree_node_t* node) {
    if (rb_tree_node_ptr(node)->left) {
	return rb_tree_max_node(tree, rb_tree_node_ptr(node)->left);
    }
    while (rb_tree_node_ptr(node)->p && rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left == node) {
	node = rb_tree_node_ptr(node)->p;
    }
    return rb_tree_node_ptr(node)->p;
}

void rb_tree_delete_fix(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* l, *r;
    int color;
    if (!node)
	return;
    if (rb_tree_node_color(node) == rb_tree_red) {
	rb_tree_node_set_color(tree, node, rb_tree_black);
	return;
    }
    if (!rb_tree_node_ptr(node)->p)
	return;
    if (rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right && node == rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right) {
	if (rb_tree_node_color(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left) == rb_tree_red) {
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_red);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left, rb_tree_black);
	    rb_tree_right_rotate(tree, rb_tree_node_ptr(node)->p);
	}
	l = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)->left;
	r = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)->right;
	if (rb_tree_node_color(r) == rb_tree_black && rb_tree_node_color(l) == rb_tree_black) {
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left, rb_tree_red);
	    rb_tree_delete_fix(tree, rb_tree_node_ptr(node)->p);
	    return;
	}
	
	if (rb_tree_node_color(l) == rb_tree_black) {
	    color = rb_tree_node_color(r);
	    rb_tree_node_set_color(tree, r, rb_tree_node_color(rb_tree_node_ptr(r)->p));
	    if (rb_tree_node_color(rb_tree_node_ptr(r)->p) == rb_tree_black)
		rb_tree_node_set_black(r);
	    else
		rb_tree_node_set_red(r);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(r)->p, color);
	    rb_tree_right_rotate(tree, rb_tree_node_ptr(r)->p);
	}
	l = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)->left;
	r = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->left)->right;
	if (rb_tree_node_color(l) == rb_tree_red) {
	    color = rb_tree_node_color(rb_tree_node_ptr(node)->p);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_black);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(l)->p, color);
	    rb_tree_node_set_color(tree, l, rb_tree_black);
	    rb_tree_node_set_black(l);
	    rb_tree_left_rotate(tree, rb_tree_node_ptr(node)->p);
	}
    } else {
	if (rb_tree_node_color(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right) == rb_tree_red) {
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_red);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right, rb_tree_black);
	    rb_tree_left_rotate(tree, rb_tree_node_ptr(node)->p);
	}
	l = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right)->left;
	r = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right)->right;
	if (rb_tree_node_color(l) == rb_tree_black && rb_tree_node_color(r) == rb_tree_black) {
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right, rb_tree_red);
	    rb_tree_delete_fix(tree, rb_tree_node_ptr(node)->p);
	    return;
	}
	
	if (rb_tree_node_color(r) == rb_tree_black) {
	    color = rb_tree_node_color(l);
	    rb_tree_node_set_color(tree, l, rb_tree_node_color(rb_tree_node_ptr(l)->p));
	    if (rb_tree_node_color(rb_tree_node_ptr(l)->p) == rb_tree_black)
		rb_tree_node_set_black(l);
	    else
		rb_tree_node_set_red(l);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(l)->p, color);
	    rb_tree_right_rotate(tree, rb_tree_node_ptr(l)->p);
	}
	l = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right)->left;
	r = rb_tree_node_ptr(rb_tree_node_ptr(rb_tree_node_ptr(node)->p)->right)->right;
	if (rb_tree_node_color(r) == rb_tree_red) {
	    color = rb_tree_node_color(rb_tree_node_ptr(node)->p);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(node)->p, rb_tree_black);
	    rb_tree_node_set_color(tree, rb_tree_node_ptr(r)->p, color);
	    rb_tree_node_set_color(tree, r, rb_tree_black);
	    rb_tree_node_set_black(r);
	    rb_tree_left_rotate(tree, rb_tree_node_ptr(node)->p);
	}
    }
}

int rb_tree_delete(rb_tree_t* tree, void* element) {
    rb_tree_node_t* node, *todel, *subtree;
    if (!tree || !element)
	return -1;
    node = rb_tree_search_node(tree, tree->root, element);
    if (!node)
	return -1;
    if (rb_tree_node_ptr(node)->left == NULL || rb_tree_node_ptr(node)->right == NULL)
	todel = node;
    else
	todel = rb_tree_successor_node(tree, node);
    if (rb_tree_node_ptr(todel)->left)
	subtree = rb_tree_node_ptr(todel)->left;
    else
	subtree = rb_tree_node_ptr(todel)->right;
    if (subtree)
	rb_tree_node_ptr(subtree)->p = rb_tree_node_ptr(todel)->p;
    if (rb_tree_node_ptr(todel)->p == NULL)
	tree->root = subtree;
    else if (todel == rb_tree_node_ptr(rb_tree_node_ptr(todel)->p)->left)
	rb_tree_node_ptr(rb_tree_node_ptr(todel)->p)->left = subtree;
    else
	rb_tree_node_ptr(rb_tree_node_ptr(todel)->p)->right = subtree;
    if (todel != node)
	memcpy(rb_tree_node_data(node), rb_tree_node_data(todel), tree->type_size);
    if (rb_tree_node_color(todel) == rb_tree_black) {
	rb_tree_delete_fix(tree, subtree);
    }
    free(rb_tree_node_ptr(todel));
    tree->size--;
    return 0;
}
void rb_tree_print_func(rb_tree_node_t* node, void* data) {
    void (*print)(void*) = data;
    if (rb_tree_node_color(node) == rb_tree_black)
	printf("b");
    else
	printf("r");
    print(rb_tree_node_data(node));
    printf(" ");
}

int rb_tree_visit_in_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data) {
    if (node) {
	rb_tree_visit_in_order(rb_tree_node_ptr(node)->left, func, data);
	func(node, data);
	rb_tree_visit_in_order(rb_tree_node_ptr(node)->right, func, data);
    }
    return 0;
}
int rb_tree_visit_pre_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data) {
    if (node) {
	func(node, data);
	rb_tree_visit_pre_order(rb_tree_node_ptr(node)->left, func, data);
	rb_tree_visit_pre_order(rb_tree_node_ptr(node)->right, func, data);
    }
    return 0;
}
int rb_tree_visit_post_order(rb_tree_node_t* node, void (*func)(rb_tree_node_t* node, void*), void* data) {
    if (node) {
	rb_tree_visit_post_order(rb_tree_node_ptr(node)->left, func, data);
	rb_tree_visit_post_order(rb_tree_node_ptr(node)->right, func, data);
	func(node, data);
    }
    return 0;
}

void* rb_tree_min_ref(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* t;
    if (!tree || !node)
	return NULL;
    t = rb_tree_min_node(tree, node);
    if (!t)
	return NULL;
    return rb_tree_node_data(t);
}

void* rb_tree_max_ref(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* t;
    if (!tree || !node)
	return NULL;
    t = rb_tree_max_node(tree, node);
    if (!t)
	return NULL;
    return rb_tree_node_data(t);
}

void* rb_tree_successor_ref(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* t;
    if (!tree || !node)
	return NULL;
    t = rb_tree_successor_node(tree, node);
    if (!t)
	return NULL;
    return rb_tree_node_data(t);
}

void* rb_tree_predecessor_ref(rb_tree_t* tree, rb_tree_node_t* node) {
    rb_tree_node_t* t;
    if (!tree || !node)
	return NULL;
    t = rb_tree_predecessor_node(tree, node);
    if (!t)
	return NULL;
    return rb_tree_node_data(t);
}

int rb_tree_min(rb_tree_t* tree, rb_tree_node_t* node, void* out) {
    rb_tree_node_t* t;
    if (!tree || !node || !out)
	return -1;
    t = rb_tree_min_node(tree, node);
    if (!t)
	return -1;
    memcpy(out, rb_tree_node_data(t), tree->type_size);
    return 0;
}

int rb_tree_max(rb_tree_t* tree, rb_tree_node_t* node, void* out) {
    rb_tree_node_t* t;
    if (!tree || !node || !out)
	return -1;
    t = rb_tree_max_node(tree, node);
    if (!t)
	return -1;
    memcpy(out, rb_tree_node_data(t), tree->type_size);
    return 0;
}

int rb_tree_successor(rb_tree_t* tree, rb_tree_node_t* node, void* out) {
    rb_tree_node_t* t;
    if (!tree || !node || !out)
	return -1;
    t = rb_tree_successor_node(tree, node);
    if (!t)
	return -1;
    memcpy(out, rb_tree_node_data(t), tree->type_size);
    return 0;
}

int rb_tree_predecessor(rb_tree_t* tree, rb_tree_node_t* node, void* out) {
    rb_tree_node_t* t;
    if (!tree || !node || !out)
	return -1;
    t = rb_tree_predecessor_node(tree, node);
    if (!t)
	return -1;
    memcpy(out, rb_tree_node_data(t), tree->type_size);
    return 0;
}

int rb_tree_search(rb_tree_t* tree, rb_tree_node_t* node, void* element, void* out) {
    rb_tree_node_t* t;
    if (!tree || !node || !element || !out)
	return -1;
    t = rb_tree_search_node(tree, node, element);
    if (!t)
	return -1;
    memcpy(out, rb_tree_node_data(t), tree->type_size);
    return 0;
}

void* rb_tree_search_ref(rb_tree_t* tree, rb_tree_node_t* node, void* element) {
    rb_tree_node_t* t;
    if (!tree || !node || !element)
	return NULL;
    t = rb_tree_search_node(tree, node, element);
    if (!t)
	return NULL;
    return rb_tree_node_data(t);

}
rb_tree_type_implementation(char, i8)
rb_tree_type_implementation(unsigned char, u8)
rb_tree_type_implementation(short, i16)
rb_tree_type_implementation(unsigned short, u16)
rb_tree_type_implementation(int, i32)
rb_tree_type_implementation(unsigned int, u32)
rb_tree_type_implementation(long, i64)
rb_tree_type_implementation(unsigned long, u64)
rb_tree_type_implementation(char, char)
rb_tree_type_implementation(float, f32)
rb_tree_type_implementation(double, f64)
