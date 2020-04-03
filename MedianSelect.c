/*
 * ===============================================
 * 	 Implementation of Median Selection Functions
 * ===============================================
 */

#include "MedianSelect.h"

// Modifies the array by ordering the values in a crescent order
// Despite insertion sort's average asymptotic complexity of O(n^2), which is worse than quick sort's O(n log(n)),
// insertion sorts still performs better on small arrays, and since it is being applied to arrays of 5 elements,
// it is the preferred option.

void insertionSort(int *arr, int arrLen) {

    int i;
    int j;
    int key;

    for (i = 1; i < arrLen; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

void median_swap(int *a, int *b) {

    int temp = *a;
    *a = *b;
    *b = temp;
}

// Modifies the array by moving all values greater than the pivot on its right
// and all elements smaller than the pivot on its left
// Returns the new index of the pivot

// The pivot is the median of medians
int median_partition(int *arr, int arrLen) {

    int pivot = arr[0];
    int i = arrLen - 1;
    int j;

    for (j = arrLen - 1; j > 0; j--) {
        if (arr[j] >= pivot){
            median_swap(&arr[i], &arr[j]);
            i--;
        }
    }

    median_swap(&arr[i], &arr[0]);

    return i;
}

// Sets the median value at arr[0]
void set_median(int *arr, int arrLen) {

    // Median values of each of the 5-sized subgroups ( included the last one which could be <5-sized )
    // are stored from arr[0] onwards by performing swaps, so that an in-place implementation can be achieved

    int i;
    for (i = 0; i < arrLen / 5; i++) { // Finds the median n/5 times

        // Orders the subgroup, it is implied the median value is at the median position
        insertionSort(arr + i * 5, 5);
        median_swap(&arr[i], &arr[i * 5 + 2]);
    }
    if (i * 5 < arrLen) { // Last possible group not included in the previous cycle
        insertionSort(arr + i * 5, arrLen % 5);
        median_swap(&arr[i], &arr[i * 5 + ((arrLen % 5) / 2)]);
        i++;
    }

    if (i != 1) { // There are more than 1 subgroups, meaning the median of medians value is not yet found
        set_median(arr, i);
    }
}

// Recursively applies partition and itself until the array is partitioned in such a way
// the index of the pivot is the same as the k value given
// Returns the kth smallest element in the array

int median_rec(int *arr, int arrLen, int k){

    set_median(arr, arrLen);
    int indexPiv = median_partition(arr, arrLen);

    // if the index of the pivot is the same as k
    if (indexPiv == k - 1) {
        // return the value at its position
        return arr[indexPiv];
        // if the index of the pivot is greater than k
    } else if (indexPiv > k - 1) {
        // recursion on the left partition of the array
        return median_rec(arr, indexPiv, k);
        // if the index of the pivot is greater than k
    } else {
        // recursion on the right partition of the array
        return median_rec(arr + indexPiv + 1, arrLen - indexPiv - 1, k - indexPiv - 1);
    }
}

// Returns the kth smallest value in the given vector
// Modifies the vector
int median_select(int *arr, int arrLen, int kth, int mode){

    int result = 0;
    int *arr_cpy = malloc(arrLen * sizeof(int));
    memcpy(arr_cpy, (int *)arr, arrLen * sizeof(int));

    if (mode == 0) {
        result = median_rec(arr_cpy, arrLen, kth);
    }

    free(arr_cpy);
    return result;
}