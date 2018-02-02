#pragma once

#include <stdint.h>

typedef struct{
    int64_t *values;
    uint64_t count;
} Array;

Array* arr_create();
Array* arr_new(uint64_t count);
void arr_free(Array *arr);
void arr_print(Array *arr);
void arr_swap(Array *arr1, Array *arr2);

#define arr_at(a, i) a->values[i]
#define arr_size(a) a->count
#define arr_elements(a) a->values
