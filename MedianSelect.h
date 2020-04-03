#ifndef MEDIAN_SELECT_H
#define MEDIAN_SELECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void median_swap(int *, int *);
int  median_partition(int *, int);
void insertionSort(int *, int);
void set_median(int *, int);
int  median_rec(int *, int, int);
int  median_select(int *, int, int, int);

#endif // MEDIAN_SELECT_H