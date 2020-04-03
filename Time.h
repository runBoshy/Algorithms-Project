#ifndef TIME_TIME_H
#define TIME_TIME_H

#include <math.h>
#include <time.h>
#include <unistd.h>
#include "QuickSelect.h"
#include "MedianSelect.h"
#include "HeapSelect.h"

double_t compute_sysResolution();
void     time_insertionSort(double_t *, int);
double_t compute_execTime(struct timespec, struct timespec);
void     compute_standardDeviation(double *, int);
double_t compute_selection_timings(int (*f)(int *, int, int, int), int *, int, int);


#endif //TIME_TIME_H
