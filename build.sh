#!/bin/zsh

gcc -o test hashmap.c bst.c binary_tree.c stack.c queue.c list.c array.c util.c deque.c dlist.c sorted_array.c sorting.c rbtree.c test.c -fsanitize=address -Wall -std=c89 -g

