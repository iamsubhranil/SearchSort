#include "searching.h"
#include <stdio.h>
#include <inttypes.h>

SearchResult search_binary(Array *arr, int64_t search){
    SearchResult res = {0, 0};
    uint64_t l = 0, u = arr_size(arr);
    while(l <= u){
        uint64_t mid = (l + u)/2;
        if(arr_at(arr, mid) == search){
            res.found = 1;
            res.position = mid;
            return res;
        }
        if(search < arr_at(arr, mid))
            u = mid - 1;
        else
            l = mid + 1;
    }
    return res;
}

SearchResult search_linear(Array *arr, int64_t search){
    SearchResult res = {0, 0};
    uint64_t i = 0;
    while(i < arr_size(arr)){
        if(arr_at(arr, i) == search){
            res.found = 1;
            res.position = i;
            return res;
        }
        i++;
    }
    return res;
}
