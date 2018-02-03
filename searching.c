#include "searching.h"
#include <stdio.h>
#include <inttypes.h>

/* Binary search
 * =============
 * This type of search works on a sorted array by divide
 * and conquer approach. In this method, the source
 * array is recursively halved until the size of the new
 * half reaches 1, and then it is checked whether that
 * half contains the desired value.
 * The halving is done by comparing the item_to_be_searched
 * with the item at the mid position of the current half.
 * If the item_to_be_searched is greater than the value
 * at mid point, since the array is sorted, it can be
 * concluded that the item will reside to the upper half
 * of the array, and hence the new lower index becomes
 * mid + 1. Else, it can be concluded that the item
 * resides to the lower half of the current subarray
 * and hence the upper index of the current subarray
 * is set to mid - 1.
 * In this way, iteratively the search continues.
 */
SearchResult search_binary(Array *arr, int64_t search){
    SearchResult res = {0, 0}; // Structure to hold the search result
    // Initialization of initial lower and upper limits,
    // which are the first index, i.e. 0 and the size_of_the_array
    // respectively
    uint64_t l = 0, u = arr_size(arr);
    
    // The terminal condition.
    // This condition implies that there is atleast one
    // element between lower and upper index (i.e. when l == u),
    // and hence there is a possibility that the item to be searched
    // resides here.
    while(l <= u){
        // The midpoint to pivot on
        uint64_t mid = (l + u)/2;
        // Item is at the midpoint, search successful!
        if(arr_at(arr, mid) == search){
            res.found = 1;
            res.position = mid;
            return res;
        }
        // Item is less than the value at midpoint,
        // hence it can only reside into the lower
        // half of present subarray
        if(search < arr_at(arr, mid))
            u = mid - 1; // The upper limit of present
                        // subarray is decreased to indicate
                        // the boundary of search
        else // Item can only reside into the upper
            // half of the present subarray
            l = mid + 1; // The lower limit of the present
                        // subarray is increased to reflect 
                        // the change
    }
    return res; // Return the result
}

/* Linear search
 * =============
 * This is the simplest search method of all.
 * In this process, the given array is searched
 * sequentially from the lower index to the upper
 * index until the item_to_be_searched is found.
 */
SearchResult search_linear(Array *arr, int64_t search){
    SearchResult res = {0, 0}; // Initialize result
    uint64_t i = 0; // Initiate the index with lower bound
    while(i < arr_size(arr)){ // Continue till all elements are searched
        if(arr_at(arr, i) == search){ // Explicitly compare item_to_be_searched
                                        // with present element each time
            res.found = 1;
            res.position = i;
            return res;
        }
        i++; // Increase the index
    }
    return res;
}
