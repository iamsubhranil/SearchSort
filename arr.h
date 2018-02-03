#pragma once

#include <stdint.h>

// Core array structure
typedef struct{
    int64_t *values; // Actual array
    uint64_t count; // Size of the array
} Array;

// Create an array solely by users choice of size and elements
Array* arr_create();
// Create an array with the specified size, with no inputs from the user
Array* arr_new(uint64_t count);
// Free a created array
void arr_free(Array *arr);
// Print an array
void arr_print(Array *arr);
// Swap two arrays
void arr_swap(Array *arr1, Array *arr2);
// Copy one array to the other
Array* arr_copy(Array *orig);

// Access an element of the array 'a' at 'i'th index
#define arr_at(a, i) a->values[i]
// Get the size of the array
#define arr_size(a) a->count
// Get the collection itself
#define arr_elements(a) a->values
