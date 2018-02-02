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

static inline void swap(int64_t *p1, int64_t *p2){
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

static void heap_create(Array *arr){
    uint64_t i = 0;
    Array *b = arr_new(arr_size(arr));
    while(i < arr_size(arr)){
        arr_at(b, i) = arr_at(arr, i);
        uint64_t j = i;
        while(j > 0){
            uint64_t parent = (j + 1) / 2 - 1;
            if(arr_at(b, j) < arr_at(b, parent)){
                swap(&arr_at(b, j), &arr_at(b, parent));
                j = parent;
            }
            else
                break;
        }
        i++;
    }
    
    arr_swap(arr, b);
    arr_free(b);
}

void heap_rebuild(Array *arr, uint64_t size){
    if(size == 1)
        return;
    uint64_t j = 0;
    while(1){
        uint64_t leftChild = 2 * j + 1, rightChild = 2 * j + 2;
        uint64_t parent = j, nextCheck = j;
        int64_t *min = &arr_at(arr, parent);
        if(leftChild < size && arr_at(arr, leftChild) < *min){
            min = &arr_at(arr, leftChild);
            nextCheck = leftChild;
        }
        if(rightChild < size && arr_at(arr, rightChild) < *min){
            min = &arr_at(arr, rightChild);
            nextCheck = rightChild;
        }
        if(*min != arr_at(arr, parent)){
            swap(min, &arr_at(arr, parent));
            j = nextCheck;
        }
        else
            break;
    }
}

void sort_heap(Array *arr){
    printf("\nCreating heap..\n");
    heap_create(arr);
    printf("\nHeap created..\n");
    Array *sorted = arr_new(arr_size(arr));
    for(uint64_t i = arr_size(arr), j = 0;i > 0;i--, j++){
        arr_at(sorted , j) = arr_at(arr , 0);
        arr_at(arr , 0) = arr_at(arr, i - 1);
        heap_rebuild(arr, i - 1);
    }
    arr_at(sorted, arr_size(sorted) - 1) = arr_at(arr, 0);

    arr_swap(sorted, arr);
    arr_free(sorted);
}

// Bubble sort
// =========================
//

void sort_bubble(Array *arr){
    for(uint64_t i = 0;i < arr_size(arr) - 1;i++){
        for(uint64_t j = 0;j < arr_size(arr) - i - 1;j++){
            if(arr_at(arr, j) > arr_at(arr, j + 1)){
                swap(&arr_at(arr, j), &arr_at(arr, j + 1));
            }
        }
    }
}

// Quick sort
// ======================
//

uint64_t quick_sort_partition(Array *arr, uint64_t left, uint64_t right){
    uint64_t loc = left;
    while(left < right){
        while(arr_at(arr, loc) <= arr_at(arr, right) && loc < right)
            right--;
        if(arr_at(arr, loc) > arr_at(arr, right)){
            swap(&arr_at(arr, loc), &arr_at(arr, right));
            loc = right;
            left++;
        }
        while(arr_at(arr, loc) >= arr_at(arr, left) && loc > left)
            left++;
        if(arr_at(arr, loc) < arr_at(arr, left)){
            swap(&arr_at(arr, loc), &arr_at(arr, left));
            loc = left;
            right--;
        }
    }
    return loc;
}

void quick_sort(Array *a, uint64_t low, uint64_t high){
    if(low < high){
        uint64_t loc = quick_sort_partition(a, low, high);
        if(loc != 0)
            quick_sort(a, low, loc - 1);
        quick_sort(a, loc + 1, high);
    }
}

void sort_quick(Array *arr){
    quick_sort(arr, 0, arr_size(arr) - 1);
}

int main(){
    Array *a = arr_create();
    const char *sortString = "quick";
    //printf("\nBefore %s sort : ", sortString);
    //arr_print(a);
    printf("\nPerforming %s sort..\n", sortString);
    sort_quick(a);
    //printf("\nAfter %s sort : ", sortString);
    //arr_print(a);

    printf("\nTesting..\n");
    if(sort_test(a))
        printf("\nSorting succeeded!");
    else
        printf("\nSorting failed!");

    printf("\n");
    arr_free(a);
    return 0;
}
