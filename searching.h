#pragma once

#include "arr.h"
#include <stdint.h>

typedef struct{
    uint8_t found;
    uint64_t position;
} SearchResult;

SearchResult search_linear(Array *arr, int64_t element);
SearchResult search_binary(Array *arr, int64_t element);
