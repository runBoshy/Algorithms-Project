#ifndef QUICK_SELECT_H
#define QUICK_SELECT_H

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void quick_swap(int *, int *);
int  quick_partition(int *, int, int);
int  quick_rec(int *, int, int, int);
int  quick_select(int *, int, int, int);

#endif // QUICK_SELECT_H
