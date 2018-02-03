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
        uint64_t p = j == arr_size(arr) - 1 ? j : j + 1;
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

static void heap_rebuild(Array *arr, uint64_t size){
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
    heap_create(arr);
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

static uint64_t quick_sort_partition(Array *arr, uint64_t left, uint64_t right){
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

static void quick_sort(Array *a, uint64_t low, uint64_t high){
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

// Merge sort
// =========================
//

static void sorted_merge(Array *arr, uint64_t l, uint64_t mid, uint64_t r, Array *c){
    uint64_t i = l, j = mid + 1, k = 0;
    while(i <= mid && j <= r){
        if(arr_at(arr, i) <= arr_at(arr, j)){
            arr_at(c, k) = arr_at(arr, i);
            i++;
        }
        else{
            arr_at(c, k) = arr_at(arr, j);
            j++;
        }
        k++;
    }
    uint64_t restStart = 1, restEnd = 0;

    if(i > mid && j <= r){
        restStart = j;
        restEnd = r;
    }
    else if(i <= mid && j > r){
        restStart = i;
        restEnd = mid;
    }
    for(uint64_t m = restStart;m <= restEnd;m++){
        arr_at(c, k) = arr_at(arr, m);
        k++;
    }
    for(uint64_t m = 0;m < k;m++){
        arr_at(arr, m + l) = arr_at(c, m);
    }
}

static void merge_sort(Array *arr, uint64_t l, uint64_t r, Array *aux){
    if(r <= l)
        return;
    uint64_t mid = (l + r)/2;
    merge_sort(arr, l, mid, aux);
    merge_sort(arr, mid+1, r, aux);
    sorted_merge(arr, l, mid, r, aux);
}

void sort_merge(Array *ar){
    Array *aux = arr_new(arr_size(ar));
    merge_sort(ar, 0, arr_size(ar) - 1, aux);
    arr_free(aux);
}

// Radix sort (decimal)
//

void sort_radix(Array *arr){
    // Find the maximum number
    int64_t max = arr_at(arr, 0);
    for(uint64_t i = 1; i < arr_size(arr) ;i++){
        if(max < arr_at(arr, i))
            max = arr_at(arr, i);
    }
    // Count the number of digits in it, and hence number of passes
    uint64_t passes = 0;
    while(max > 0){
        max /= 10; passes++;
    }

    // Create 10 buckets, for digits 0-9
    Array *buckets[10];
    // Initialize all of them to number_of_elements, just to be on the safe side
    // This can be optimized
    for(uint64_t i = 0; i < 10 ; i++)
        buckets[i] = arr_new(arr_size(arr));

    uint64_t partExtractor = 10, digitExtractor = 1;
    // Start pass
    while(passes > 0){
        // Pointer to buckets
        uint64_t bucketPointer[10] = {0};
        // Extraction loop
        for(uint64_t i = 0; i < arr_size(arr) ; i++){
            int64_t element = arr_at(arr, i); // Get the element at ith position
            uint64_t digit = (element % partExtractor) / digitExtractor; // Extract the digit
            arr_at(buckets[digit], bucketPointer[digit]) = element; // Put the element into required bucket
            bucketPointer[digit]++; // Increase the bucketPointer for that digit
        }

        // Put them again in the original array
        uint64_t digit = 0, arrPointer = 0, tempPointer = 0;
        while(digit < 10){
            if(tempPointer == bucketPointer[digit]){ // tempPointer should always be less than
                tempPointer = 0;                    // bucketPointer if the bucket at digit
                digit++;                            // has atleast one element
                continue;
            }

            // Put the element from bucket to the original array
            arr_at(arr, arrPointer) = arr_at(buckets[digit], tempPointer);
            arrPointer++;
            tempPointer++;
        }

        // Increment
        partExtractor *= 10;
        digitExtractor *= 10;
        passes--;
    }
    
    // Free the buckets
    for(uint64_t i = 0; i < 10 ; i++)
        arr_free(buckets[i]);
}
