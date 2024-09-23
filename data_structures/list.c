#include "ds.h"
#include <stdlib.h>
#include <string.h>
#include <stdio.h>


int list_init(list_t* list, size_t type_size, int (*cmp)(void*, void*)) {
    if (!list)
        return -1;
    list->type_size = type_size;
    list->len = 0;
    list->head = NULL;
    list->tail = NULL;
    list->cmp = cmp;
    return 0;
}

int list_close(list_t* list) {
    struct list_node_s* h, *t;
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
    return 0;
}

int list_is_empty(list_t* list) {
    if (!list)
	return 0;
    return list->len == 0;
}

int list_push_front(list_t* list, void* data) {
    struct list_node_s* n;
    if (!list || !data)
        return -1;
    n = (struct list_node_s*) malloc(sizeof(struct list_node_s) + list->type_size);
    memcpy(list_node_data(n), data, list->type_size);
    n->next = list->head;
    if (!list->head)
	list->tail = n;
    list->head = n;
    list->len++;
    return 0;
}


int list_push_back(list_t* list, void* data) {
    struct list_node_s* n, *t;
    if (!list || !data)
        return -1;
    n = (struct list_node_s*) malloc(sizeof(struct list_node_s) + list->type_size);
    memcpy(list_node_data(n), data, list->type_size);
    if (list->head) {
        t = list->tail;
	t->next = n;
	list->tail = n;
    } else {
        list->head = n;
    }
    list->tail = n;
    n->next = NULL;
    list->len++;
    return 0;
}

int list_pop_front(list_t* list, void* out) {
    struct list_node_s* t;
    if (list_is_empty(list))
	return -1;
    if (out)
	memcpy(out, list_node_data(list->head), list->type_size);
    t = list->head->next;
    free(list->head);
    list->head = t;
    list->len--;
    if (!list->head)
	list->tail = NULL;
    return 0;
}

int list_pop_back(list_t* list, void* out) {
    struct list_node_s* h;
    if (list_is_empty(list))
	return -1;
    if (out)
	memcpy(out, list_node_data(list->tail), list->type_size);
    h = list->head;
    while (h && h->next && h->next != list->tail)
	h = h->next;
    if (h->next != list->tail)
	return -1;
    list->tail = h;
    list->len--;
    free(h->next);
    h->next = NULL;
    return 0;
}

int list_insert_index(list_t* list, int index, void* data) {
    struct list_node_s *n, *h;
    size_t i;
    if (!list || !data  || index < 0)
        return -1;
    if (index == 0)
        return list_push_front(list, data);
    else if (index >= list->len)
        return list_push_back(list, data);
    else {
        h = list->head;
        for (i = 0; i < index; i++)
            h = h->next;
        if (!h)
            return -1;
        n = (struct list_node_s*) malloc(sizeof(struct list_node_s) + list->type_size);
        memcpy(list_node_data(n), data, list->type_size);
        n->next = h->next;
        h->next = n;
    }
    list->len++;
    return 0;
}

int list_delete(list_t* list, void* element) {
    struct list_node_s* h, *t;
    if (!list || !element)
        return -1;
    h = list->head;
    if (memcmp(list_node_data(h), element, list->type_size) == 0) {
	list->head = h->next;
	if (!list->head)
	    list->tail = NULL;
	free(h);
	list->len--;
	return 0;
    }
    while (h && h->next && memcmp(list_node_data(h->next), element, list->type_size) != 0)
        h = h->next;
    if (!h || !h->next)
        return -1;
    list->len--;
    t = h->next;
    if (!t->next)
	list->tail = h;
    
    h->next = t->next;
    free(t);
    return 0;
}

int list_delete_index(list_t* list, int index) {
    size_t i;
    struct list_node_s* h, *t;
    if (!list || index >= list->len || index < 0)
        return -1;
    if (index == 0)
	return list_pop_front(list, NULL);
    if (index == list->len - 1)
	return list_pop_back(list, NULL);
    h = list->head;
    for (i = 0; i < index - 1; i++)
        h = h->next;
    if (!h)
        return -1;
    list->len--;
    t = h->next;
    h->next = t->next;
    free(t);
    return 0;
}

int list_print(list_t* list, void (*print)(void*)) {
    struct list_node_s* h;
    if (!list || !print)
        return -1;
    h = list->head;
    print(list_node_data(h));
    h = h->next;    
    while (h) {
        printf(", ");
        print(list_node_data(h));
        h = h->next;
    }
    printf("\n");
    return 0;
}

int list_search(list_t* list, void* key, void* out) {
    struct list_node_s* h;
    int i;
    if (!list || !key)
        return -1;
    h = list->head;
    i = 0;
    while (h) {
        if (list->cmp(key, list_node_data(h)) == 0) {
	    if (out)
		memcpy(out, list_node_data(h), list->type_size);
            return i;
        }
        h = h->next;
	i++;
    }
    return -1;
}

list_node_t* list_search_node(list_t* list, void* key) {
    struct list_node_s* h;
    if (!list || !key)
        return NULL;
    h = list->head;
    while (h) {
        if (list->cmp(key, list_node_data(h)) == 0) {
            return h;
        }
        h = h->next;
    }
    return NULL;
}

int list_min(list_t* list, void* out) {
    int imin, i;
    struct list_node_s* min;
    struct list_node_s* h;
    if (!list)
	return -1;
    min = list->head;
    h = list->head;
    imin = 0;
    i = 0;
    while (h) {
	if (list->cmp(list_node_data(h), list_node_data(min)) < 0) {
	    min = h;
	    imin = i;
	}
	h = h->next;
	i++;
    }
    if (out)
	memcpy(out, list_node_data(min), list->type_size);
    return imin;
}

list_node_t* list_min_node(list_t* list) {

    struct list_node_s* min;
    struct list_node_s* h;
    if (!list)
	return NULL;
    
    h = list->head;
    while (h) {
	if (list->cmp(list_node_data(h), list_node_data(min)) < 0)
	    min = h;
	h = h->next;
    }
    return min;
}

int list_max(list_t* list, void* out) {
    int imax, i;
    struct list_node_s* max;
    struct list_node_s* h;
    if (!list)
	return -1;
    max = list->head;
    h = list->head;
    imax = 0;
    i = 0;
    while (h) {
	if (list->cmp(list_node_data(h), list_node_data(max)) > 0) {
	    max = h;
	    imax = i;
	}
	i++;
	h = h->next;
    }
    if (out)
	memcpy(out, list_node_data(max), list->type_size);
    return imax;
}

list_node_t* list_max_node(list_t* list) {
    struct list_node_s* max;
    struct list_node_s* h;
    if (!list)
	return NULL;
    max = list->head;
    h = list->head;
    while (h) {
	if (list->cmp(list_node_data(h), list_node_data(max)) > 0)
	    max = h;
	h = h->next;
    }
    return max;
}

int list_successor(list_t* list, void* element, void* out) {

    int isucc, i;
    struct list_node_s* succ, *h;
    if (!list)
	return -1;
    h = list->head;
    i = 0;
    while (h && list->cmp(list_node_data(h), element) <= 0) {
	i++;
	h = h->next;
    }
    if (h) {
	succ = h;
	isucc = i;
    } else
	return -1;

    while (h) {
	if (list->cmp(list_node_data(h), element) > 0 && list->cmp(list_node_data(h), list_node_data(succ)) < 0) {
	    succ = h;
	    isucc = i;
	}
	h = h->next;
	i++;
    }
    if (out)
	memcpy(out, list_node_data(succ), list->type_size);
    return isucc;
}

list_node_t* list_successor_node(list_t* list, void* element) {
   struct list_node_s* succ, *h;
    if (!list)
	return NULL;
    h = list->head;
    while (h && list->cmp(list_node_data(h), element) <= 0) {
	h = h->next;
    }
    if (h) {
	succ = h;
    } else
	return NULL;

    while (h) {
	if (list->cmp(list_node_data(h), element) > 0 && list->cmp(list_node_data(h), list_node_data(succ)) < 0) {
	    succ = h;
	}
	h = h->next;
    }
    return succ;
}

int list_predecessor(list_t* list, void* element, void* out) {
    
    int ipred, i;
    struct list_node_s* pred, *h;
    if (!list)
	return -1;
    h = list->head;
    i = 0;
    while (h && list->cmp(list_node_data(h), element) >= 0) {
	i++;
	h = h->next;
    }
    if (h) {
	pred = h;
	ipred = i;
    } else
	return -1;

    while (h) {
	if (list->cmp(list_node_data(h), element) < 0 && list->cmp(list_node_data(h), list_node_data(pred)) > 0) {
	    pred = h;
	    ipred = i;
	}
	h = h->next;
	i++;
    }
    if (out)
	memcpy(out, list_node_data(pred), list->type_size);
    return ipred;
}

list_node_t* list_predecessor_node(list_t* list, void* element) {
    struct list_node_s* pred, *h;
    if (!list)
	return NULL;
    h = list->head;
    while (h && list->cmp(list_node_data(h), element) >= 0) {
	h = h->next;
    }
    if (h) {
	pred = h;
    } else
	return NULL;

    while (h) {
	if (list->cmp(list_node_data(h), element) < 0 && list->cmp(list_node_data(h), list_node_data(pred)) > 0) {
	    pred = h;
	}
	h = h->next;
    }
    return pred;
}

list_type_implementation(char, i8)
list_type_implementation(unsigned char, u8)
list_type_implementation(short, i16)
list_type_implementation(unsigned short, u16)
list_type_implementation(int, i32)
list_type_implementation(unsigned int, u32)
list_type_implementation(long, i64)
list_type_implementation(unsigned long, u64)
list_type_implementation(char, char)
list_type_implementation(float, f32)
list_type_implementation(double, f64)
