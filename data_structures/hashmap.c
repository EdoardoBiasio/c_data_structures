#include "ds.h"
#include <string.h>
#include <stdlib.h>

int oh_hashmap_init(oh_hashmap_t* map, int type_size, int capacity, size_t (*hash)(void*, size_t), int (*cmp)(void*, void*)) {
    int i;
    if (!map || !hash || !cmp)
	return -1;
    map->type_size = type_size;
    map->bucket_count = capacity;
    map->full_buckets = map->size = 0;
    map->hash = hash;
    map->cmp = cmp;
    map->data = calloc(sizeof(list_t), map->bucket_count);
    for (i = 0; i < map->bucket_count; i++)
	list_init(map->data + i, map->type_size, map->cmp); 
    return 0;
}

int oh_hashmap_close(oh_hashmap_t* map) {
    int i;
    if (!map)
	return -1;
    map->type_size = map->full_buckets = map->size = 0;
    map->hash = NULL;
    map->cmp = NULL;
    for (i = 0; i < map->bucket_count; i++)
	list_close(map->data + i);
    map->bucket_count = 0;
    free(map->data);
    map->data = NULL;
    return 0;
}

double oh_hashmap_load_factor(oh_hashmap_t* map) {
    return (double) map->size / (double) map->bucket_count;
}

void oh_hashmap_realloc(oh_hashmap_t* map) {
    list_t* new, *temp;
    list_node_t* h, *t;
    size_t new_bucket_count = map->bucket_count * 2, i, idx;
    if (!map)
	return;
    new = calloc(sizeof(list_t), new_bucket_count);
    for (i = 0; i < new_bucket_count; i++)
	list_init(new + i, map->type_size, map->cmp);
    for (i = 0; i < map->bucket_count; i++) {
	temp = map->data + i;
	h = temp->head;
	while (h) {
	    t = h->next;
	    idx = map->hash(list_node_data(h), new_bucket_count);
	    h->next = (new + idx)->head;
	    new[idx].head = h;
	    h = t;
	}
    }

    for (i = 0; i < map->bucket_count; i++)
	list_close(map->data + i);
    free(map->data);
    map->data = new;
    map->bucket_count = new_bucket_count;
}

int oh_hashmap_insert(oh_hashmap_t* map, void* in) {
    size_t hash;
    if (!map || !in)
	return -1;
    hash = map->hash(in, map->bucket_count);
    if (hash > map->bucket_count)
	return -1;
    map->size++;
    if (list_is_empty(map->data + hash))
	map->full_buckets ++;
    if (list_push_front(map->data + hash, in) < 0)
	return -1;
    if (oh_hashmap_load_factor(map) > 1.0)
	oh_hashmap_realloc(map);
    return 0;
}

int oh_hashmap_delete(oh_hashmap_t* map, void* element) {
    size_t hash;
    
    if (!map || !element)
	return -1;
    hash = map->hash(element, map->bucket_count);
    if (hash > map->bucket_count)
	return -1;
    
    if(list_delete(map->data + hash, element) < 0)
	return -1;
    if (list_is_empty(map->data + hash))
	map->full_buckets--;
    return 0;
}

int oh_hashmap_search(oh_hashmap_t* map, void* key, void* out) {
    size_t hash;
    if (!map || !key || !out)
	return -1;
    hash = map->hash(key, map->bucket_count);
    if (hash > map->bucket_count)
	return -1;

    return list_search(map->data + hash, key, out);
}

void* oh_hashmap_search_ref(oh_hashmap_t* map, void* key) {
    size_t hash;
    list_node_t* res;
    if (!map || !key)
	return NULL;
    hash = map->hash(key, map->bucket_count);
    if (hash > map->bucket_count)
	return NULL;
    res = list_search_node(map->data + hash, key);
    if (!res)
	return NULL;
    return list_node_data(res);
}


oh_hashmap_type_implementation(char, i8)
oh_hashmap_type_implementation(unsigned char, u8)
oh_hashmap_type_implementation(short, i16)
oh_hashmap_type_implementation(unsigned short, u16)
oh_hashmap_type_implementation(int, i32)
oh_hashmap_type_implementation(unsigned int, u32)
oh_hashmap_type_implementation(long, i64)
oh_hashmap_type_implementation(unsigned long, u64)
oh_hashmap_type_implementation(char, char)
oh_hashmap_type_implementation(float, f32)
oh_hashmap_type_implementation(double, f64)

