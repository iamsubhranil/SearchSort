#include "sorting.h"
#include <stdio.h>
#include <stdint.h>
#include <inttypes.h>
#include <time.h>

#define perform_sort(x) printf("\nPerforming " #x " sort..\n"); clockStart = clock(); sort_##x(a); clockEnd = clock(); break;

int main(){
    Array *source = arr_create();
    while(1){
        int64_t choice = 0;
        Array *a = arr_copy(source);
        printf("\n1. Bubble Sort");
        printf("\n2. Selection Sort");
        printf("\n3. Insertion Sort");
        printf("\n4. Quick Sort");
        printf("\n5. Merge Sort");
        printf("\n6. Heap Sort");
        printf("\n7. Radix Sort");
        printf("\n8. Exit : ");
        scanf("%" SCNd64, &choice);
        if(choice < 1 || choice > 8){
            printf("\n[Error] Wrong choice!\n");
            continue;
        }
        clock_t clockStart = 0, clockEnd = 0;
        switch(choice){
            case 1:
                perform_sort(bubble);
            case 2:
                perform_sort(selection);
            case 3:
                perform_sort(insertion);
            case 4:
                perform_sort(quick);
            case 5:
                perform_sort(merge);
            case 6:
                perform_sort(heap);
            case 7:
                perform_sort(radix);
            case 8:
                arr_free(a);
                goto end;
        }

        double sec = (double)(clockEnd - clockStart) / CLOCKS_PER_SEC;
        printf("\nTesting sort..\n");
        if(sort_test(a))
            printf("\nSorting succeeded [took %g seconds]!", sec);
        else{
            printf("\nSorting failed [took %g seconds]!", sec);
        }
        printf("\n\nDo you want to see the result ? \n1. Yes \n2. No : ");
        scanf("%" SCNd64, &choice);
        if(choice == 1){
            printf("\nInput array : ");
            arr_print(source);
            printf("\nOutput array : ");
            arr_print(a);
        }
        arr_free(a);
        printf("\n");
    }
end:
    printf("\n");
    arr_free(source);
    return 0;
}
