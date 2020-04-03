/*
 * ===============================================
 *           Time estimation functions
 * ===============================================
 */

#include "Time.h"

#define BILLION 1000000000
#define RESOLUTION_LOOPS 10
#define PERCENTAGE_ERROR 0.005

double_t systemResolution = -1;
double_t value = 0;

double_t compute_sysResolution() {

    double_t res[RESOLUTION_LOOPS];
    double_t execTime;
    struct timespec tick, tock;
    for (int i = 0; i < RESOLUTION_LOOPS; i++) {
        clock_gettime(CLOCK_MONOTONIC, &tick);
        do {
            clock_gettime(CLOCK_MONOTONIC, &tock);
        } while (tick.tv_nsec == tock.tv_nsec);

        execTime = (tock.tv_nsec - tick.tv_nsec);
        execTime /= BILLION;
        res[i] = execTime;
    }

    time_insertionSort(res, RESOLUTION_LOOPS);
    double_t resMedian = res[RESOLUTION_LOOPS / 2];
    return resMedian;
}

// Used to easily find the median value that will be used as the system's resolution
// Obviously not time efficient
void time_insertionSort(double_t *arr, int arr_len) {

    int i;
    int j;
    double_t key;

    for (i = 1; i < arr_len; i++) {
        key = arr[i];
        j = i - 1;
        while (j >= 0 && arr[j] > key) {
            arr[j + 1] = arr[j];
            j = j - 1;
        }
        arr[j + 1] = key;
    }
}

// The result should be expressed in %0.9lf format to avoid loss of information
double_t compute_execTime(struct timespec tick,struct timespec tock) {

    double_t execTime;
    execTime = ( BILLION *(tock.tv_sec - tick.tv_sec) + ( tock.tv_nsec - tick.tv_nsec ));
    return execTime / BILLION;
}

// Calculate mean time and standard deviations given an array of doubles
// Modifies the given array and sets the mean time at arr[0] and the standard deviation at arr[1];
void compute_standardDeviation(double *arr, int arr_len){

    double mean = 0.0;
    double sampleVariance = 0.0;
    double standardDeviation;
    int i;

    for (i = 0; i < arr_len; i++){
        mean += arr[i];
    }
    mean /= arr_len;
    for (i = 0; i < arr_len; i++){
        arr[i] -= mean;
    }
    for (i = 0; i < arr_len; i++){
        arr[i] *= arr[i];
    }
    for (i = 0; i < arr_len; i++){
        sampleVariance += arr[i];
    }
    sampleVariance /= (arr_len);
    standardDeviation = sqrt(sampleVariance);

    arr[0] = mean;
    arr[1] = standardDeviation;
}

// This function takes a function as a parameter (quick_select, median_select or heap_select)
// so that the time estimation's code doesn't have to be repeated multiple times
double_t compute_selection_timings(int (*f)(int *, int, int, int), int *arr, int arrLen, int kth) {

    if (systemResolution == -1) {
        fprintf(stderr, "Computing system's resolution...\n");
        systemResolution = compute_sysResolution();
        value = systemResolution * ((1 / PERCENTAGE_ERROR) + 1);
    }

    double_t fullTime = 0; double_t initTime = 0;
    struct timespec tick, tock;

    // For cycle is used to calculate the duration of the entirety of the algorithm first
    // and only the initialization of data structures second
    // "mode" int is used to switch between the two scenarios
    // All selection algorithms have been modified to allow such behavior
    int mode = 0;
    for (int i = 0; i<2; i++) {

        clock_gettime(CLOCK_MONOTONIC, &tick);
        int count = 0;
        do {
            (*f)(arr, arrLen, kth, mode);
            clock_gettime(CLOCK_MONOTONIC, &tock);
            count++;
        } while (compute_execTime(tick, tock) <= value);

        if (mode == 0) {
            fullTime = (compute_execTime(tick, tock)) / count;
        } else if (mode == 1) {
            initTime = (compute_execTime(tick, tock)) / count;
        }

        mode++;
    }

    double_t execTime = fullTime - initTime;
    return execTime;
}
