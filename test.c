#include "clib.h"
#include <stdio.h>


int main(int argc, char** argv) {
    int n;
    rb_tree_t t;
    rb_tree_init(&t, sizeof(int), cmp_i32);
    n = 10;
    rb_tree_insert(&t, &n);
    n = 20;
    rb_tree_insert(&t, &n);
    n = -5;
    rb_tree_insert(&t, &n);
    n = 18;
    rb_tree_insert(&t, &n);
    n = -20;
    rb_tree_insert(&t, &n);
    n = 25;
    rb_tree_insert(&t, &n);
    n = 6;
    rb_tree_insert(&t, &n);
    n = 11;
    rb_tree_insert(&t, &n);
    n = -21;
    rb_tree_insert(&t, &n);
    n = 2;
    rb_tree_insert(&t, &n);
    n = 4;
    rb_tree_insert(&t, &n);
    n = 24;
    rb_tree_insert(&t, &n);
    rb_tree_print_in_order(t.root, print_i32);
    n= 4;
    rb_tree_delete(&t, &n);
    n= 25;
    rb_tree_delete(&t, &n);
        n= -20;
    rb_tree_delete(&t, &n);
        n= 11;
    rb_tree_delete(&t, &n);
        n= 18;
    rb_tree_delete(&t, &n);
        n=-5;
    rb_tree_delete(&t, &n);
    printf("\n");
    rb_tree_print_in_order(t.root, print_i32);
    return 0;
}
