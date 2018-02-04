#include "sorting.h"
#include <stdint.h>
#include <stdio.h>
#include <inttypes.h>

/* Tests whether a given array is sorted in ascending
 * order of magnitude
 * ==================================================
 * This test is performed by comparing two sequential
 * elements of the given array. If the array is sorted
 * in ascending order, the item with at lower index
 * should be lesser than or equal to the item at
 * the upper index.
 */
uint8_t sort_test(Array *arr){
    uint64_t i = 0; // Intiate at the lowest index
    
    // Since we are comparing present element
    // to the next (i.e. (present + 1)th) element,
    // the loop should only continue till the
    // penultimte element.
    while(i < (arr_size(arr) - 1)){
        if(arr_at(arr, i) <= arr_at(arr, i + 1)) // The items are in ascending order
            i++; // Continue
        else{ // The items are not in ascending order
            printf("\nElement %" PRId64 " expected after %" PRId64 "!", arr_at(arr, i), arr_at(arr, i+1));
            return 0;
        }
    }
    return 1;
}

// Swaps to elements at the given pointers
static inline void swap(int64_t *p1, int64_t *p2){
    int64_t p3 = *p1;
    *p1 = *p2;
    *p2 = p3;
}

/* Insertion sort
 * =========================
 * This type of sorting is performed using an auxiliary
 * array and by sorted insertion in that array.
 * Consider having two arrays, A and B, A being the array
 * to be sorted, B is an auxiliary array of equal size. At 
 * first, the first item from A is picked an placed as
 * the first item of B. Now, the second item from A
 * is picked and compared with the item presently at
 * B to determine whether it will follow or precede the
 * item. Since the sorting is in ascending order, if the
 * new item is less than the old item, then the old item
 * is shifted to one place right and the new item is
 * placed in front of it in B. This method is iteratively
 * continued for all items present at A, and finally
 * B becomes the sorted array.
 */

void sort_insertion(Array *arr){
    Array *out = arr_new(arr_size(arr)); // Initialize the auxiliary array with same size
    arr_at(out, 0) = arr_at(arr, 0); // Place the first element
    // This loop will start at index 1, since index 0 is already copied,
    // and continue till all the items of input array is traversed
    for(uint64_t j = 1;j < arr_size(arr);j++){
        int64_t k = arr_at(arr, j); // Get the element at present index
        
        // 'i' denotes the last position till which the auxiliary array 'out'
        // is full. If the present index of traversal for the input array is 'j',
        // then previous 'j' items have already been put into place at 'out', i.e.
        // the last index is 'j - 1'.
        uint64_t i = j - 1;
        
        // 'shiftPos' denotes the position of insertion of 'k' at 'out',
        // i.e. the position upto which the items of 'out' have to be rightshifted
        // from the end.
        uint64_t shiftpos = 0;
        
        // Now there might be a case where 'k' is going to be the lowest item at 'out'.
        // We need to know if 'k' will be placed at 0th index in 'out', and all the 
        // subsequent elements will be shifted towards right. Since we're working with
        // unsigned integers, we can't really flag it by setting 'i' to '-1', hence
        // this 8 bit value will act as boolean variable to infrom us for that particular
        // case.
        uint8_t allShift = 0;
        
        // Continue eternally ;)
        while(1){
            // The item at hand is less than the item at the 'i'th index
            // of 'out', hence we need to decrement 'i'
            if(k < arr_at(out, i)){
                if(i == 0){ // This is the special case.
                            // 'i' is already 0. Decrementing it anymore
                            // will make it UINT64_MAX. Hence we set the
                            // 'allShift' flag and break out of the loop.
                    allShift = 1;
                    break;
                }
                i = i - 1;
            }
            // The item at hand is greater than the item at the 'i'th
            // index of 'out', hence we need to insert it at the 'i+1'th
            // index
            else
                break;
        }

        // This variable denotes the upper limit of the right-shifting
        // madness at 'out'. By previous logic, we've already placed
        // 'j' items in 'out', and going to insert a new one,
        // making the size of out 'j+1', and its end index will be 'j'.
        // Hence the items which will be rightshifted, should be
        // rightshifted from 'shiftPos' to 'j', or in C,
        // 'while(shiftPos < j)'. Since in our case, 'shiftPos'
        // and 'j' are constants for this context, we copy 'j' to
        // another variable 'p', and move it downwards towards 'shiftPos'.
        uint64_t p = j;

        if(allShift) // All items are to be shifted,
                    // hence the new item is going to be inserted
                    // at index 0
            shiftpos = 0;
        else // 'k' is greater than the item at 'i' in 'out',
            // hence it is going to be inserted at index 'i+1'
            shiftpos = i + 1;

        // Rightshift loop
        while(p > shiftpos){
            // Just copy the element at (present - 1)th index
            // to the present index
            arr_at(out, p) = arr_at(out, p - 1);
            // Decrement the pointer
            p--;
        }
        // Finally, place the item at hand, i.e. 'k' to the
        // auxiliary array 'out' at 'shiftPos'.
        arr_at(out, shiftpos) = k;
    }

    // Swap the array to make it look like inplace ;)
    arr_swap(arr, out);
    // Free the auxiliary array
    arr_free(out);
}

/* Insertion Sort Inplace
 * ======================
 * This type of insertion sort does not use an auxiliary 
 * array to store the sorted elements. Rather, for each
 * sorted element, it is inserted into the input array 
 * directly. This does not increase the time complexity of
 * the algorithm, but rather decreases the space complexity 
 * to O(1).
 *
 */

void sort_insertion_inplace(Array *arr){
    // Start from index 1, i.e. leave the 0th
    // element as it is, since it will be
    // automatically compared and shifted as required.
    for(uint64_t i = 1; i < arr_size(arr) ;i++){
        int64_t item = arr_at(arr, i); // Get the element to sort
        // This is the index upto which the input
        // array is already sorted
        uint64_t lastIndex = i - 1;
        // A flag to denote if the element to be
        // inserted at 0th index, i.e. all other
        // already sorted elements have to be
        // rightshifted
        uint8_t allShift = 0;
        // Find the index in which the element
        // is going to be inserted. This loop performs
        // actual sorting.
        //
        // If the item at hand is less than the
        // last sorted item, then it will be inserted
        // before the later, i.e. its index will be
        // decremented. This will be continued
        // until such an element is found whose
        // magnitude is lesser than the one at hand.
        while(item < arr_at(arr, lastIndex)){
            if(lastIndex == 0){ // But present index
                                // is already 0, hence
                                // all previously sorted
                                // items have to be shifted
                                // by 1 position.
                allShift = 1;
                break; // Exit from the loop
            }
            lastIndex--; // Element at hand will be inserted
                        // before this index
        }
        // This is the position
        // i) at which the element at hand is to be inserted
        // ii) from which all the elements has to be rightshifted
        uint64_t shiftPos;
        if(allShift) // This flag is set to 1, hence item is to be
                    // inserted at 0th index
            shiftPos = 0;
        else
            shiftPos = lastIndex + 1; // Item is to be inserted
                                    // at 'lastIndex+1', since
                                    // 'lastIndex' is already
                                    // decremented in the
                                    // while loop
        uint64_t curPos = i; // Create a copy of present position

        // Rightshift loop
        while(curPos > shiftPos){
            arr_at(arr, curPos) = arr_at(arr, curPos - 1);
            curPos--;
        }

        // Inserted the element
        arr_at(arr, shiftPos) = item;
    }
}

/* Selection sort
 * ===================
 * This type of sorting works by selecting
 * the minimum element of the rest of the array
 * at each iteration and placing it in the
 * present index.
 *
 */

void sort_selection(Array *arr){
    // Continue till the penultimate index,
    // since even the item at the last index
    // will automatically be swapped with it
    // if required in the inner loop
    for(uint64_t i = 0;i < arr_size(arr) - 1;i++){
        
        // Find the position of the minimum element
        // from the rest of the array, i.e.
        // from 'i' to 'size_of_the_array', and
        // store it in 'minpos'
        uint64_t j = i, minpos = i;
        int64_t min = arr_at(arr, i);
        while(j < arr_size(arr)){
            if(arr_at(arr, j) < min){
                min = arr_at(arr, j);
                minpos = j;
            }
            j++;
        }

        // If the item at present index is not
        // the minimum in the rest of the array,
        // swap it with the one that is.
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

// Merge sort inplace
// ===================
//

static void sorted_merge_inplace(Array *source, uint64_t low, uint64_t mid, uint64_t high){
    uint64_t j = mid + 1;
    while(low <= mid && j <= high){
        if(arr_at(source, low) <= arr_at(source, j)){
            low++;
        }
        else{
            int64_t element = arr_at(source, j);
            for(uint64_t t = j; t > low ;t--)
                arr_at(source, t) = arr_at(source, t - 1);
            arr_at(source, low) = element;
            low++;
            j++;
            mid++;
        }
    }
}

static void merge_sort_inplace(Array *source, uint64_t low, uint64_t high){
    if(low >= high)
        return;
    uint64_t mid = (low + high) / 2;
    merge_sort_inplace(source, low, mid);
    merge_sort_inplace(source, mid + 1, high);
    sorted_merge_inplace(source, low, mid, high);
}

void sort_merge_inplace(Array *source){
    merge_sort_inplace(source, 0, arr_size(source) - 1);
}

// Radix sort (decimal)
//

#define el_abs(x) (x < 0 ? -x : x)

#define SPACE_CONSTRAINED_RADIX

void sort_radix(Array *arr){
    // Find the maximum number
    int64_t max = el_abs(arr_at(arr, 0));
    for(uint64_t i = 1; i < arr_size(arr) ;i++){
        if(max < el_abs(arr_at(arr, i)))
            max = el_abs(arr_at(arr, i));
    }
    // Count the number of digits in it, and hence number of passes
    uint64_t passes = 0;
    while(max > 0){
        max /= 10; passes++;
    }

    // Create 20 buckets, for digits -ve 0-9 and +ve 0-9 respectively
    Array *buckets[20];
    // Initialize all of them to number_of_elements, just to be on the safe side
    // This can be optimized
    for(uint64_t i = 0; i < 20 ; i++)
#ifdef SPACE_CONSTRAINED_RADIX
        buckets[i] = NULL;
#else
        buckets[i] = arr_new(arr_size(arr));
#endif

    uint64_t partExtractor = 10, digitExtractor = 1;
    // Start pass
    while(passes > 0){
        // Pointer to buckets
        uint64_t bucketPointer[20] = {0};
        // Extraction loop
        for(uint64_t i = 0; i < arr_size(arr) ; i++){
            int64_t element = arr_at(arr, i); // Get the element at ith position
            int64_t digit = ((element < 0 ? -element : element )% partExtractor) / digitExtractor; // Extract the digit
            if(element >= 0)
                digit += 10;
            else
                digit = 9 - digit;
#ifdef SPACE_CONSTRAINED_RADIX
            if(buckets[digit] == NULL)
                buckets[digit] = arr_new(arr_size(arr));
#endif
            arr_at(buckets[digit], bucketPointer[digit]) = element; // Put the element into required bucket
            bucketPointer[digit]++; // Increase the bucketPointer for that digit
        }

        // Put them again in the original array
        uint64_t digit = 0, arrPointer = 0, tempPointer = 0;
        while(digit < 20){
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
    for(uint64_t i = 0; i < 20 ; i++)
#ifdef SPACE_CONSTRAINED_RADIX
        if(buckets[i] != NULL)
#endif
            arr_free(buckets[i]);
}
