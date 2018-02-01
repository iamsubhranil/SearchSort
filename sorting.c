#include "sorting.h"
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

uint8_t sort_test(Array *arr){
    uint64_t i = 0;
    while(i < (arr_size(arr) - 1)){
        if(arr_at(arr, i) <= arr_at(arr, i + 1))
            i++;
        else{
            printf("\nElement %" PRId64 " expected after %" PRId64 "!", arr_at(arr, i), arr_at(arr, i+1));
            return 0;
        }
    }
    return 1;
}

static void swap(int64_t *p1, int64_t *p2){
    int64_t p3 = *p1;
    *p1 = *p2;
    *p2 = p3;
}

// Insertion sort
// =========================
//

void sort_insertion(Array *arr){
    Array *out = arr_new(arr_size(arr));
    arr_at(out, 0) = arr_at(arr, 0);
    for(uint64_t j = 1;j < arr_size(arr);j++){
        int64_t k = arr_at(arr, j);
        uint64_t i = j - 1, shiftpos = 0;
        uint8_t allShift = 0;
        while(1){
            if(k < arr_at(out, i)){
                if(i == 0){
                    allShift = 1;
                    break;
                }
                i = i - 1;
            }
            else
                break;
        }
        uint64_t p = j + 1;
        if(allShift)
            shiftpos = 0;
        else
            shiftpos = i + 1;
        while(p > shiftpos){
            arr_at(out, p) = arr_at(out, p - 1);
            p--;
        }
        arr_at(out, shiftpos) = k;
    }

    // Swap the array to make it look like inplace ;)
    int64_t *temp = arr_elements(arr);
    arr_elements(arr) = arr_elements(out);
    arr_elements(out) = temp;
    arr_free(out);
}

// Selection sort
// ===================
//

void sort_selection(Array *arr){
    for(uint64_t i = 0;i < arr_size(arr) - 1;i++){
        // Select minimum
        uint64_t j = i, minpos = i;
        int64_t min = arr_at(arr, i);
        while(j < arr_size(arr)){
            if(arr_at(arr, j) < min){
                min = arr_at(arr, j);
                minpos = j;
            }
            j++;
        }

        // Swap
        if(i != minpos){
            swap(&arr_at(arr, i), &arr_at(arr, minpos));
            //int64_t temp = arr_at(arr, i);
            //arr_at(arr, i) = arr_at(arr, minpos);
            //arr_at(arr, minpos) = temp;
        }
    }
}

// Heap sort
// =============================
//

static Array* heap_create(Array *arr){
    Array *ret = arr_new(arr_size(arr));
    uint64_t i = 0;
    while(i < arr_size(arr)){
        int64_t x = arr_at(arr, i);
        arr_at(ret, i) = x;
        uint64_t j = i;
        while(j > 0){
            // Max heap
            if(arr_at(ret, j) > arr_at(ret, j/2)){
                swap(&arr_at(ret, j), &arr_at(ret, j/2));
                //int64_t temp = arr_at(ret, j);
                //arr_at(ret, j) = arr_at(ret, j/2);
                //arr_at(ret, j/2) = temp;
            }
            else
                j = 0;
        }
        i++;
    }
    return ret;
}

static void heap_removemax(Array *arr, uint64_t pos){
    int64_t temp = arr_at(arr, pos);
    arr_at(arr, pos) = arr_at(arr, 0);
    arr_at(arr, 0) = temp;
}

static void heap_rebuild(Array *heap, uint64_t i){
    if(i == 0)
        return;
    uint64_t j = 0;
    while(1){
        uint64_t leftChild = 2*j, rightChild = 2*j + 1;
        if(rightChild <= i){
            if(arr_at(heap, j) <= arr_at(heap, leftChild) && arr_at(heap, leftChild) >= arr_at(heap, rightChild)){
                swap(&arr_at(heap, j), &arr_at(heap, leftChild));
                j = leftChild;
            }
            else{
                if(arr_at(heap, j) <= arr_at(heap, rightChild) && arr_at(heap, rightChild) >= arr_at(heap, leftChild)){
                    swap(&arr_at(heap, j), &arr_at(heap, rightChild));
                }
                else
                    break;
            }
        }
        else {
            if(leftChild <= i){
                if(arr_at(heap, j) <= arr_at(heap, leftChild)){
                    swap(&arr_at(heap, j), &arr_at(heap, leftChild));
                    j = leftChild;
                }
                else
                    break;
            }
        }
    }
}

void sort_heap(Array *arr){
    Array *heap = heap_create(arr);
    for(uint64_t i = arr_size(arr) - 1;i > 1;i--){
        heap_removemax(heap, i);
        heap_rebuild(heap, i - 1);
    }
}

int main(){
    Array *a = arr_create();
    //printf("\nBefore sorting : ");
    //arr_print(a);
    printf("\nPerforming selection sort..\n");
    sort_selection(a);
    //printf("\nAfter heapifying : ");
    printf("\nTesting..\n");
    if(sort_test(a))
        printf("\nSorting succeeded!");
    else
        printf("\nSorting failed!");
    
    printf("\n");
    arr_free(a);
    return 0;
}
