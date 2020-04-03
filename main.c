/*
 * ===============================================
 * 	     Time Estimation Program applied to
 *    Quick Select, Median Select, Heap Select
 * ===============================================
 */

// Note :
// It's preferable to increase the program's priority in order to decrease the jitter caused by interrupts,
// I/0 and other processes. To achieve this, run the compiled file with the following code :
// sudo nice -n, --adjustment =-19 "NAME OF COMPILED FILE"

#define _GNU_SOURCE
#include <sched.h>
#include "Time.h"

// At least two tests MUST be performed at all times
#define SAME_ARRAY_TESTS 100
#define DIFFERENT_ARRAY_TESTS 100

void print_to_file(int n, int k, double t1, double d1, double t2, double d2, double t3, double d3){

    FILE *outputFile;
    outputFile = fopen("results.txt", "a");
    if(outputFile == NULL)
        exit(EXIT_FAILURE);
    fprintf(outputFile,
            "%d\t%d\t%0.9lf\t%0.9lf\t%0.9lf\t%0.9lf\t%0.9lf\t%0.9lf\n",
            n, k, t1, d1, t2, d2, t3, d3);
    fclose(outputFile);
}

void print_to_screen(int n, int k, double t1, double d1, double t2, double d2, double t3, double d3) {

    printf("N : %d\tK : %d\tT1 : %0.9lf\tD1 : %0.9lf\tT2 : %0.9lf\tD2 : %0.9lf\tT3 : %0.9lf\tD3 : %0.9lf\n",
           n, k, t1, d1, t2, d2, t3, d3);
}

void seed_rand() {

    // Seeding rand() with urandom file increases quality of random numbers generated
    FILE *fp = fopen("/dev/urandom", "rb");
    unsigned int seed;
    fread(&seed, sizeof(seed), 1, fp);
    fclose(fp);
    srand(seed);
}

int main() {

    // In order to decrease the amount of trashing caused by the program switching cores
    // and thus invalidating L1 and L2 cache, the process' affinity is set to core 0
    cpu_set_t my_set;
    CPU_ZERO(&my_set);
    CPU_SET(0, &my_set);
    sched_setaffinity(getpid(), sizeof(cpu_set_t), &my_set);

    double quick_timings[DIFFERENT_ARRAY_TESTS];
    double heap_timings[DIFFERENT_ARRAY_TESTS];
    double median_timings[DIFFERENT_ARRAY_TESTS];

    seed_rand();

    // ================================================================================
    // This section can be modified at will based on the test one is about to perform
    // ================================================================================

    int exponent = 0;
    int arrLen = 100;


    // Used to create multiple tests following a specific progression of values
    while (arrLen < 120) {

        int kth = arrLen/2;
        int index = 0;
        // Multiple tests are performed in order to create a monotonic function from the computed results
        // Assumes the size of the array is not modified between tests, but its values are
        for (int i = 0; i < DIFFERENT_ARRAY_TESTS; i++) {

            // Creates an array of random values
            int rndMax = arrLen/2;
            int rndMin = (arrLen/2) * (-1);
            int *arr = malloc(arrLen * sizeof(int));
            for (int m = 0; m < arrLen; m++) {
                arr[m] = (rand() % (rndMax - rndMin + 1)) + rndMin;
            }

    // ================================================================================

            // Multiple tests are performed in order to reduce jitter caused by the system as much as possible
            // Assumes the size of the array and its values are not modified between tests
            // A mean value of all the timings is calculated thereafter
            double_t quickTimings = 0; double_t heapTimings = 0; double_t medianTimings = 0;
            for (int j = 0; j < SAME_ARRAY_TESTS; j++) {

                // QUICK SELECT TIME ESTIMATION
                quickTimings  += compute_selection_timings(quick_select, arr, arrLen, kth);

                // HEAP SELECT TIME ESTIMATION
                heapTimings   += compute_selection_timings(heap_select, arr, arrLen, kth);

                // MEDIAN SELECT TIME ESTIMATION
                medianTimings += compute_selection_timings(median_select, arr, arrLen, kth);

            }


            quick_timings[index] = (quickTimings/SAME_ARRAY_TESTS);
            heap_timings[index] = (heapTimings/SAME_ARRAY_TESTS);
            median_timings[index] = (medianTimings/SAME_ARRAY_TESTS);

            index++;

            free(arr);
        }

        compute_standardDeviation(quick_timings, DIFFERENT_ARRAY_TESTS);
        compute_standardDeviation(heap_timings, DIFFERENT_ARRAY_TESTS);
        compute_standardDeviation(median_timings, DIFFERENT_ARRAY_TESTS);

        double t1 = quick_timings[0] ; double d1 = quick_timings[1];
        double t2 = heap_timings[0]  ; double d2 = heap_timings[1];
        double t3 = median_timings[0]; double d3 = median_timings[1];

        print_to_file (arrLen, kth, t1, d1, t2, d2, t3, d3);
        print_to_screen (arrLen, kth, t1, d1, t2, d2, t3, d3);


        // while cycle's guard
        exponent++;
        arrLen += 1;
        //arrLen = 100 * pow( 1.5, exponent );
    }

    return 0;
}