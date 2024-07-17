#include "clib.h"
util_type_implementation(char, i8)
util_type_implementation(unsigned char, u8)
util_type_implementation(short, i16)
util_type_implementation(unsigned short, u16)
util_type_implementation(int, i32)
util_type_implementation(unsigned int, u32)
util_type_implementation(long, i64)
util_type_implementation(unsigned long, u64)
util_type_implementation(char, char)
util_type_implementation(float, f32)
util_type_implementation(double, f64)


void print_i8(void* el) {
    printf("%d", *(char*) el);
}

void print_i16(void* el) {
    printf("%d", *(short int*) el);
}

void print_i32(void* el) {
    printf("%d", *(int*) el);
}

void print_i64(void* el) {
    printf("%ld", *(long int*) el);
}

void print_u8(void* el) {
    printf("%u", *(unsigned char*) el);
}

void print_u16(void* el) {
    printf("%u", *(unsigned short*) el);
}

void print_u32(void* el) {
    printf("%u", *(unsigned int*) el);
}

void print_u64(void* el) {
    printf("%lu", *(unsigned long int*) el);
}

void print_f32(void* el) {
    printf("%f", *(float*) el);
}

void print_f64(void* el) {
    printf("%f", *(double*) el);
}

void print_char(void* el) {
    printf("%c", *(char*) el);
}


