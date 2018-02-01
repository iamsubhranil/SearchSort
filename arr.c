#include "arr.h"
#include <stdlib.h>
#include <stdio.h>
#include <inttypes.h>

Array* arr_new(uint64_t a){
    Array *arr = (Array *)malloc(sizeof(Array));
    arr->count = a;
    arr->values = (int64_t *)malloc(sizeof(int64_t) * a);
    return arr;
}

void arr_free(Array *a){
    free(a->values);
    free(a);
}

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
    }
    while(i < arr_size(arr)){
        if(size == 1){
            printf("\nElement %" PRIu64 " : ", i+1);
            scanf("%" SCNd64, &inpt);
        }
        else{
            inpt = random() % range;
        }
        arr_at(arr, i) = inpt;
        i++;
    }
    return arr;
}

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
