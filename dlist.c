#include "clib.h"
#include <string.h>
#include <stdlib.h>


int dlist_init(dlist_t* list, size_t type_size, int (*cmp)(void*, void*)) {
    if (!list)
        return -1;
    list->type_size = type_size;
    list->len = 0;
    list->head = NULL;
    list->tail = NULL;
    list->cmp = cmp;
    return 0;

}

int dlist_close(dlist_t* list) {
    dlist_node_t* h, *t;
    if (!list)
	return -1;
    h = list->head;
    t = NULL;
    while (h) {
	t = h;
	h = h->next;
	free(t);
    }
    list->head = list->tail = NULL;
    list->len = 0;
    list->type_size = 0;
    list->cmp = NULL;
    return 0;
}

int dlist_is_empty(dlist_t* list) {
    if (!list)
	return -1;
    return list->len == 0;
}

int dlist_push_front(dlist_t* list, void* data) {
    dlist_node_t* n;
    if (!list || !data)
        return -1;
    n = (dlist_node_t*) malloc(sizeof(dlist_node_t) + list->type_size);
    memcpy(dlist_node_data(n), data, list->type_size);
    n->next = list->head;
    n->prev = NULL;
    if (!list->head)
	list->tail = n;
    list->head = n;
    list->len++;
    return 0;
}

int dlist_push_back(dlist_t* list, void* data) {
    dlist_node_t* n;
    if (!list || !data)
        return -1;
    n = (dlist_node_t*) malloc(sizeof(dlist_node_t) + list->type_size);
    memcpy(dlist_node_data(n), data, list->type_size);
    n->prev = list->tail;
    if (!list->tail) {
	list->head = n;
    } else {
	list->tail->next = n;
    }
    list->tail = n;
    n->next = NULL;
    list->len++;
    return 0;
}

int dlist_pop_front(dlist_t* list, void* out) {
    dlist_node_t* t;
    if (dlist_is_empty(list))
	return -1;
    if (out)
	memcpy(out, dlist_node_data(list->head), list->type_size);
    t = list->head->next;
    free(list->head);
    list->head = t;
    list->len--;
    if (!list->head)
	list->tail = NULL;
    else
	t->prev = NULL;
    return 0;
}

int dlist_pop_back(dlist_t* list, void* out) {
    dlist_node_t* h;
    if (dlist_is_empty(list))
	return -1;
    if (out)
	memcpy(out, dlist_node_data(list->tail), list->type_size);
    if (!list->tail->prev)
	list->head = NULL;
    else
	list->tail->prev->next = NULL;
    h = list->tail->prev;
    free(list->tail);
    list->tail = h;
    list->len--;
    return 0;
}

int dlist_insert_index(dlist_t* list, int index, void* data) {
    dlist_node_t *n, *h;
    size_t i;
    if (!list || !data  || index < 0)
        return -1;
    if (index == 0)
        return dlist_push_front(list, data);
    else if (index >= list->len)
        return dlist_push_back(list, data);
    else {
        h = list->head;
        for (i = 0; i < index; i++)
            h = h->next;
        if (!h)
            return -1;
        n = (dlist_node_t*) malloc(sizeof(dlist_node_t) + list->type_size);
	
        memcpy(dlist_node_data(n), data, list->type_size);
	n->prev = h;
	h->next->prev = n;
	n->next = h->next;
        h->next = n;
	list->len++;
    }
    return 0;
}

int dlist_delete(dlist_t* list, void* element) {
    dlist_node_t* n;
    if (!list || !element)
	return -1;
    n = dlist_search_node(list, element);
    if (!n)
	return -1;
    if (n->prev)
	n->prev->next = n->next;
    else
	list->head = n->next;
    if (n->next)
	n->next->prev = n->prev;
    else
	list->tail = n->prev;
    list->len--;
    free(n);
    return 0;
}

int dlist_delete_index(dlist_t* list, int index) {
    size_t i;
    dlist_node_t* h, *t;
    if (!list || index >= list->len || index < 0)
        return -1;
    if (index == 0)
	return dlist_pop_front(list, NULL);
    if (index == list->len - 1)
	return dlist_pop_back(list, NULL);
    h = list->head;
    for (i = 0; i < index - 1; i++)
        h = h->next;
    if (!h)
        return -1;
    list->len--;
    t = h->next;
    h->next = t->next;
    if (t->next)
	t->next->prev = h;
    free(t);
    return 0;
}

int dlist_print(dlist_t* list, void (*print)(void*)) {
    dlist_node_t* h;
    if (!list || !print)
        return -1;
    h = list->head;
    print(dlist_node_data(h));
    h = h->next;    
    while (h) {
        printf(", ");
        print(dlist_node_data(h));
        h = h->next;
    }
    printf("\n");
    return 0;
}

int dlist_search(dlist_t* list, void* key, void* out) {
    dlist_node_t* h;
    size_t i;
    if (!list || !key)
        return -1;
    h = list->head;
    i = 0;
    while (h) {
        if (list->cmp(key, dlist_node_data(h)) == 0) {
	    if (out)
		memcpy(out, dlist_node_data(h), list->type_size);
            return i;
        }
        h = h->next;
	i++;
    }
    return -1;
}

dlist_node_t* dlist_search_node(dlist_t* list, void* key) {
    dlist_node_t* h;
    if (!list || !key)
        return NULL;
    h = list->head;
    while (h) {
        if (list->cmp(key, dlist_node_data(h)) == 0) {
            return h;
        }
        h = h->next;
    }
    return NULL;
}

int dlist_min(dlist_t* list, void* out) {
    size_t imin, i;
    dlist_node_t* min;
    dlist_node_t* h;
    if (!list)
	return -1;
    min = list->head;
    h = list->head;
    imin = 0;
    i = 0;
    while (h) {
	if (list->cmp(dlist_node_data(h), dlist_node_data(min)) < 0) {
	    min = h;
	    imin = i;
	}
	i++;
    }
    if (out)
	memcpy(out, dlist_node_data(min), list->type_size);
    return imin;
}

dlist_node_t* dlist_min_node(dlist_t* list) {

    dlist_node_t* min;
    dlist_node_t* h;
    if (!list)
	return NULL;
    
    h = list->head;
    while (h) {
	if (list->cmp(dlist_node_data(h), dlist_node_data(min)) < 0)
	    min = h;
    }
    return min;
}

int dlist_max(dlist_t* list, void* out) {
    size_t imax, i;
    dlist_node_t* max;
    dlist_node_t* h;
    if (!list)
	return -1;
    max = list->head;
    h = list->head;
    imax = 0;
    i = 0;
    while (h) {
	if (list->cmp(dlist_node_data(h), dlist_node_data(max)) > 0) {
	    max = h;
	    imax = i;
	}
	i++;
    }
    if (out)
	memcpy(out, dlist_node_data(max), list->type_size);
    return (int)imax;
}

dlist_node_t* dlist_max_node(dlist_t* list) {
    dlist_node_t* max;
    dlist_node_t* h;
    if (!list)
	return NULL;
    max = list->head;
    h = list->head;
    while (h) {
	if (list->cmp(dlist_node_data(h), dlist_node_data(max)) > 0)
	    max = h;
    }
    return max;
}

int dlist_successor(dlist_t* list, void* element, void* out) {

    size_t isucc, i;
    dlist_node_t* succ, *h;
    if (!list)
	return -1;
    h = list->head;
    i = 0;
    while (h && list->cmp(dlist_node_data(h), element) <= 0) {
	i++;
	h = h->next;
    }
    if (h) {
	succ = h;
	isucc = i;
    } else
	return -1;

    while (h) {
	if (list->cmp(dlist_node_data(h), element) > 0 && list->cmp(dlist_node_data(h), dlist_node_data(succ)) < 0) {
	    succ = h;
	    isucc = i;
	}
	h = h->next;
	i++;
    }
    if (out)
	memcpy(out, dlist_node_data(succ), list->type_size);
    return (int) isucc;
}

dlist_node_t* dlist_successor_node(dlist_t* list, void* element) {
    dlist_node_t* succ, *h;
    if (!list)
	return NULL;
    h = list->head;
    while (h && list->cmp(dlist_node_data(h), element) <= 0) {
	h = h->next;
    }
    if (h) {
	succ = h;
    } else
	return NULL;

    while (h) {
	if (list->cmp(dlist_node_data(h), element) > 0 && list->cmp(dlist_node_data(h), dlist_node_data(succ)) < 0) {
	    succ = h;
	}
	h = h->next;
    }
    return succ;
}

int dlist_predecessor(dlist_t* list, void* element, void* out) {

    size_t ipred, i;
    dlist_node_t* pred, *h;
    if (!list)
	return -1;
    h = list->head;
    i = 0;
    while (h && list->cmp(dlist_node_data(h), element) >= 0) {
	i++;
	h = h->next;
    }
    if (h) {
	pred = h;
	ipred = i;
    } else
	return -1;

    while (h) {
	if (list->cmp(dlist_node_data(h), element) < 0 && list->cmp(dlist_node_data(h), dlist_node_data(pred)) > 0) {
	    pred = h;
	    ipred = i;
	}
	h = h->next;
	i++;
    }
    if (out)
	memcpy(out, dlist_node_data(pred), list->type_size);
    return (int)ipred;
}

dlist_node_t* dlist_predecessor_node(dlist_t* list, void* element) {
    dlist_node_t* pred, *h;
    if (!list)
	return NULL;
    h = list->head;
    while (h && list->cmp(dlist_node_data(h), element) >= 0) {
	h = h->next;
    }
    if (h) {
	pred = h;
    } else
	return NULL;

    while (h) {
	if (list->cmp(dlist_node_data(h), element) < 0 && list->cmp(dlist_node_data(h), dlist_node_data(pred)) > 0) {
	    pred = h;
	}
	h = h->next;
    }
    return pred;
}

dlist_type_implementation(char, i8)
dlist_type_implementation(unsigned char, u8)
dlist_type_implementation(short, i16)
dlist_type_implementation(unsigned short, u16)
dlist_type_implementation(int, i32)
dlist_type_implementation(unsigned int, u32)
dlist_type_implementation(long, i64)
dlist_type_implementation(unsigned long, u64)
dlist_type_implementation(char, char)
dlist_type_implementation(float, f32)
dlist_type_implementation(double, f64)



