#include "searching.h"
#include "sorting.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>

int main(){
    Array *arr = arr_create();
    int64_t el;
    printf("\nEnter the element to search : ");
    scanf("%" SCNd64, &el);
    uint64_t c;
choice:
    printf("\n1. Linear Search");
    printf("\n2. Binary Search : ");
    scanf("%" SCNu64, &c);
    if(c < 1 || c > 2){
        printf("\n[Error] Wrong choice!");
        goto choice;
    }
    SearchResult s;
    if(c == 1){
        printf("\nPerforming linear search..\n");
        s = search_linear(arr, el);
    }
    else{
        printf("\nChecking if the given array is sorted..\n");
        if(sort_test(arr)){
            printf("\nGiven array is already sorted!\n");
        }
        else{
            printf("\nGiven array is not sorted!\nSorting given array using quick sort method..\n");
            sort_quick(arr);
        }
        printf("\nPerforming binary search..\n");
        s = search_binary(arr, el);
    }
    if(s.found){
        printf("\nElement %" PRId64 " found at position %" PRId64"!", el, s.position+1);
    }
    else
        printf("\nElement %" PRId64 " not found!", el);
    printf("\n");
    arr_free(arr);
    return 0;
}
