#ifndef HEAP_SELECT_H
#define HEAP_SELECT_H

#include <stdio.h>
#include <stdlib.h>

struct _node{
    int value;
    int index;
}; typedef struct _node Node;

struct _heap {
    Node *data;
    int size;
    int capacity;
    int type;
}; typedef struct _heap Heap;

enum heapType {minHeap = 0, maxHeap = 1, unknown = 2};

Heap *heap_init(Heap *, enum heapType );
int   heap_capacity(Heap *);
int   heap_size(Heap *);
void  heap_resize(Heap *, int);
Node  heap_get_root(Heap *);
int   heap_parent_index(int);
int   heap_left_index(int);
int   heap_right_index(int);
void  heap_insert(Heap *, Node);
void  heap_extract(Heap *);
void  heap_swap(Node *, int, int);
void  heap_Heapify_up(Heap *, int);
void  heap_Heapify_down(Heap *, int);

void  heap_buildFromStdin(Heap *);
void  heap_buildFromArr(int *, int, Heap *);

Node  heap_rec(Heap *, Heap *, int);
int   heap_select(int *, int, int, int);

#endif // HEAP_SELECT_H