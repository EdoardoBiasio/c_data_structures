#!/bin/bash

gcc -o test test.c function_zeros.c lin_systems.c -std=c89 -lm -L../data_structures -lclib_ds
