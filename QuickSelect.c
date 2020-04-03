/*
 * ===============================================
 * 	 Implementation of Quick Select Function
 * ===============================================
 */

#include "QuickSelect.h"

void quick_swap(int *a, int *b) {

    int temp = *a;
    *a = *b;
    *b = temp;
}

// Modifies the array by moving all values greater than the pivot on its right
// and all elements smaller than the pivot on its left
// Returns the new index of the pivot

// The chosen pivot is ALWAYS the rightmost value in the given array
int quick_partition(int arr[], int left, int right) {

    int pivot = arr[right];
    int i = left;
    int j;

    for (j = left; j < right; j++) {
        if (arr[j] <= pivot){
            quick_swap(&arr[i], &arr[j]);
            i++;
        }
    }

    quick_swap(&arr[i], &arr[right]);

    return i;
}

// Recursively applies partition and itself until the array is partitioned in such a way
// the index of the pivot is the same as the k value given
// Returns the kth smallest element in the array
int quick_rec(int arr[], int left, int right, int k) {

    int indexOfPivot = quick_partition(arr, left, right);

    // if the index of the pivot is the same as k
    if (indexOfPivot == k - 1) {
        // return the value at its position
        return arr[indexOfPivot];
        // if the index of the pivot is greater than k
    } else if (indexOfPivot > k - 1) {
        // recursion on the left partition of the array
        return quick_rec(arr, left, indexOfPivot - 1, k);
        // if the index of the pivot is greater than k
    } else {
        // recursion on the right partition of the array
        return quick_rec(arr, indexOfPivot + 1, right, k);
    }
}

// Returns the kth smallest value in the given vector
// Modifies the vector
int quick_select(int *arr, int arrLen, int kth, int mode){

    int result = 0;
    int *arr_cpy = malloc(arrLen * sizeof(int));
    memcpy(arr_cpy, (int *)arr, arrLen * sizeof(int));

    if (mode == 0) {
        result = quick_rec(arr_cpy, 0, arrLen -1, kth);
    }

    free(arr_cpy);
    return result;

}