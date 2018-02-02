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
    arr_swap(arr, out);
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
        }
    }
}

// Heap sort
// =============================
//

static void heap_create2(Array *arr, uint64_t size){
    uint64_t numChild = 1, lastLevel = 0;
    while(lastLevel + 3*numChild < size){
        lastLevel += numChild;
        numChild *= 2;
    }
outerwhile:;
    uint64_t start = lastLevel;
    while(1){
        uint64_t children = 0;
        while(children < numChild){
            uint64_t parent = start + children;
            uint64_t leftChild = parent * 2 + 1;
            uint64_t rightChild = parent * 2 + 2;
            int64_t *min = &arr_at(arr, parent);
            if(leftChild < size && arr_at(arr, leftChild) < *min){
                min = &arr_at(arr, leftChild);
            }
            if(rightChild < size && arr_at(arr, rightChild) < *min){
                min = &arr_at(arr, rightChild);
            }
            if(*min != arr_at(arr, parent)){
                swap(min, &arr_at(arr, parent));
                start = lastLevel;
                goto outerwhile;
            }
            children++;
        }
        if(start == 0)
            break;
        else
            start--;
    }
}

void sort_heap(Array *arr){
    heap_create2(arr, arr_size(arr));
    Array *sorted = arr_new(arr_size(arr));
    for(uint64_t i = arr_size(arr), j = 0;i > 0;i--, j++){
        arr_at(sorted , j) = arr_at(arr , 0);
        arr_at(arr , 0) = arr_at(arr, i - 1);
        heap_create2(arr, i - 1);
    }
    arr_at(sorted, arr_size(sorted) - 1) = arr_at(arr, 0);
    
    arr_swap(sorted, arr);
    arr_free(sorted);
}

int main(){
    Array *a = arr_create();
    printf("\nBefore heap sort : ");
    arr_print(a);
    printf("\nPerforming heap sort..\n");
    sort_heap(a);
    printf("\nAfter heap sort : ");
    arr_print(a);
    
    printf("\nTesting..\n");
    if(sort_test(a))
        printf("\nSorting succeeded!");
    else
        printf("\nSorting failed!");
    
    printf("\n");
    arr_free(a);
    return 0;
}
