#include "testing.h"
#include "clib.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct array_token_s {
    char name[256];
    int index;
} array_token_t;

size_t array_token_hash(void* at, size_t len) {
    size_t sum = 0, i;
    char* c;
    array_token_t *token = at;
    c = at->name;
    while (*c) {
	sum += (*c) * i;
	i++;
	c++;
    }
    return sum % len;
}

int array_token_cmp(void* l, void* r) {
    array_token_t *L, *R;
    L = l;
    R = r;
    return cmp_u64(L->index, R->index);
} 

enum {
    /* commands */
    ARRAY_TEST_INIT = 0,
    ARRAY_TEST_SWITCH,
    ARRAY_TEST_INSERT,
    ARRAY_TEST_DELETE,
    ARRAY_TEST_DELETE_INDEX,
    ARRAY_TEST_PUSH_BACK,
    ARRAY_TEST_PUSH_FRONT,
    ARRAY_TEST_CLOSE,
    /* assertions */
    ARRAY_TEST_AT,
    ARRAY_TEST_MIN,
    ARRAY_TEST_SEARCH,
    ARRAY_TEST_SUCCESSOR,
    ARRAY_TEST_PREDECESSOR,

    ARRAY_TEST_COUNT,
    ARRAY_TEST_NONE
};

static const char *array_test_strings[ARRAY_TEST_COUNT] = {
    "init",
    "switch",
    "insert",
    "delete",
    "delete_index",
    "push_back",
    "push_front",
    "at",
    "min",
    "max",
    "search",
    "successor",
    "predecessor",
    "close"
};

int array_test_read_tag(FILE* fp, char* line) {
    char c;
    size_t i;
    c = fgetc(fp);
    while (c == ' ' || c == '\n')
	c = fgetc(fp);
    i = 0;
    while (c != ' ' && c != '\n' && c != EOF) {
	line[i] = c;
	c = fgetc(fp);
	i++;
    }
    line[i] = '\0';
    if (c == EOF)
	return -1;
    return 0;
}

int array_test_parse_tag(const char* line) {
    size_t i;
    if (*line == '\0')
	return ARRAY_TEST_NONE;

    for (i = 0; i < ARRAY_TEST_COUNT; i++) {
	if (strcmp(line, array_test_strings) == 0)
	    return i;
    }
    return ARRAY_TEST_NONE;
}

int array_run_test(const char* filename) {
    FILE* fp;
    oh_hashmap_t array_map;
    char line[256];
    int res;
    fp = fopen(filename, "r");
    if (!fp)
	goto error;
    
    if (!oh_hashmap_init(&array_map, sizeof(array_token_t), 8, array_token_hash, array_token_cmp))
	goto error;

    while (true) {
	if (array_test_read_line(fp, line) < 0) {
	    res = array_test_parse_tag(line);
	    if (res == ARRAY_TEST_NONE)
		break;
	    
	    break;
	}
	array_test_parse_tag(line);
	if (res != ARRAY_TEST_NONE) {
	    switch(res) {
	    case ARRAY_TEST_INIT:
		array_test_parse_init(fp);
		break;
	    }
	case ARRAY_TEST_INIT:
		array_test_parse_init(fp);
		break;
	    }
		    case ARRAY_TEST_INIT:
		array_test_parse_init(fp);
		break;
	    }
	}
    }
    
    oh_hashmap_close(&array_map);
    fclose(fp);
    return 0;
 error:
    oh_hashmap_close(&array_map);
    fclose(fp);
    return -1;
}
