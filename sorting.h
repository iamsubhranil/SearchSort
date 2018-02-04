#pragma once

#include "arr.h"

void sort_bubble(Array *arr);
void sort_quick(Array *arr);
void sort_insertion(Array *arr);
void sort_selection(Array *arr);
void sort_merge(Array *arr);
void sort_heap(Array *arr);
void sort_radix(Array *arr);

void sort_merge_inplace(Array *arr);
void sort_insertion_inplace(Array *arr);

// Method to check if an array is sorted
uint8_t sort_test(Array *arr);
