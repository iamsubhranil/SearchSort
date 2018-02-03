#include "arr.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>
#include <time.h>
#include <string.h>

// Allocates a new array with the specified size
Array* arr_new(uint64_t a){
    Array *arr = (Array *)malloc(sizeof(Array)); // malloc
    arr->count = a; // intialize the count
    arr->values = (int64_t *)malloc(sizeof(int64_t) * a); // initialize the value array
    return arr;
}

// Frees an array
void arr_free(Array *a){
    free(a->values); // free the value array
    free(a); // free the pointer itself
}

// Create an array by user's choice
Array* arr_create(){
    uint64_t size;
reenter:
    printf("\nNumber of elements : ");
    scanf("%" SCNu64, &size);
    if(size == 0){
        printf("\n[Error] Size must be > 0!");
        goto reenter;
    }
    Array* arr = arr_new(size);
recheck:
    printf("\n1. Enter elements manually");
    printf("\n2. Generate random elements : ");
    scanf("%" SCNu64, &size);
    if(size < 1 || size > 2){
        printf("\n[Error] Wrong choice!");
        goto recheck;
    }
    uint64_t i = 0;
    int64_t inpt, range;
    if(size == 2){
        printf("\nRange : ");
        scanf("%" SCNd64, &range);
        printf("\nGenerating %" PRIu64 " random elements..\n", arr->count);
        srand48(time(NULL)); // Reset the random seed
    }
    while(i < arr_size(arr)){
        if(size == 1){
            printf("\nElement %" PRIu64 " : ", i+1);
            scanf("%" SCNd64, &inpt);
        }
        else{
            inpt = ((lrand48() << 16) | lrand48()) % range; // generate a 64bit random value
        }
        arr_at(arr, i) = inpt;
        i++;
    }
    return arr;
}

// Prints an array
void arr_print(Array *a){
    if(arr_size(a) == 0)
        printf("{empty}");
    else{
        printf("{ %" PRId64, arr_at(a, 0));
        uint64_t i = 1;
        while(i < arr_size(a)){
            printf(", %" PRId64, arr_at(a, i));
            i++;
        }
        printf("}");
    }
}

// Swaps to arrays
void arr_swap(Array *arr1, Array *arr2){
    uint64_t temp = arr_size(arr1);
    arr_size(arr1) = arr_size(arr2);
    arr_size(arr2) = temp;

    int64_t *val = arr_elements(arr1);
    arr_elements(arr1) = arr_elements(arr2);
    arr_elements(arr2) = val;
}

// Make a copy of an array
Array* arr_copy(Array *orig){
    Array *n = arr_new(arr_size(orig));
    memcpy(arr_elements(n), arr_elements(orig), sizeof(int64_t) * arr_size(orig));
    return n;
}
