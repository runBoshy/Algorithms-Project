/*
 * ===============================================
 *        Implementation of Heap functions
 * ===============================================
 */

#include "HeapSelect.h"
#define INIT_CAPACITY 8
#define MAX_SIZE 10000

// Initializes the given heap struct
// Requires a heapType specification : minHeap ,maxHeap or unknown
Heap *heap_init(Heap *hp, enum heapType type){

    int capacity = INIT_CAPACITY;

    hp->size = 0;
    hp->capacity = capacity;
    hp->data = malloc(sizeof(Node *) * hp->capacity);
    hp->type = type;

    return hp;
}

int heap_capacity(Heap *hp){

    return hp->capacity;
}

int heap_size(Heap *hp){

    return hp->size;
}

void heap_resize(Heap *hp, int new_capacity) {

    hp->data = realloc(hp->data, sizeof(Node *) * new_capacity);
    hp->capacity = new_capacity;
}

Node heap_get_root(Heap *hp){

    return hp->data[0];
}

int heap_parent_index(int index){

    return (index - 1) / 2;
}

int heap_left_index(int index){

    return (index * 2) + 1;
}

int heap_right_index(int index){

    return (index * 2) + 2;
}

// Applies an heapify bottom to top function to reorder the heap after each insertion
void heap_insert(Heap *hp, Node n){

    hp->data[heap_size(hp)] = n;
    heap_Heapify_up(hp, heap_size(hp));
    hp->size++;

    // resize if needed
    if (heap_capacity(hp)== heap_size(hp))
        heap_resize(hp, heap_capacity(hp) * 2);
}

// Applies an heapify top to bottom function to reorder the heap after each removal
void heap_extract(Heap *hp){

    heap_swap(hp->data, 0, heap_size(hp) - 1);
    hp->size--;
    heap_Heapify_down(hp, 0);

    // resize if needed
    if (heap_size(hp) > 0 && heap_size(hp) == heap_capacity(hp) / 4)
       heap_resize(hp, heap_capacity(hp) / 2);
}

void heap_swap(Node *n, int index1, int index2){

    Node temp = n[index1];
    n[index1] = n[index2];
    n[index2] = temp;
}

// Heapify function ( going "upwards" )
// Modifies the heap by performing a bottom to top heapify function from a given index
// Behaves accordingly to the heapType in which the Heap structure is set
// Performed automatically after insert function
void heap_Heapify_up(Heap *hp, int index) {

    int parent = heap_parent_index(index);
    int parentValue = hp->data[parent].value;
    int indexedValue = hp->data[index].value;

    // if the heap it's a min-heap
    if (hp->type == minHeap) {
        if (parentValue > indexedValue) {
            heap_swap(hp->data, parent, index);
            heap_Heapify_up(hp, parent);
        }
        // if the heap it's a max-heap
    } else if (hp->type == maxHeap) {
        if (parentValue < indexedValue) {
            heap_swap(hp->data, parent, index);
            heap_Heapify_up(hp, parent);
        }
    }
}

// Heapify function ( going "downwards" )
// Modifies the heap by performing a top to bottom heapify function from a given index
// Behaves accordingly to the heapType in which the Heap structure is set
// Performed automatically after extract function
void heap_Heapify_down(Heap *hp, int index) {

    int size = heap_size(hp);
    int left = heap_left_index(index);
    int right = heap_right_index(index);

    // if the heap it's a min-heap
    if (hp->type == minHeap) {
        int min;

        if (left < size && hp->data[left].value < hp->data[index].value) {
            min = left;
        } else {
            min = index;
        }
        if (right < size && hp->data[right].value < hp->data[min].value) {
            min = right;
        }
        if (min != index) {
            heap_swap(hp->data, index, min);
            heap_Heapify_down(hp, min);
        }
        // if the heap it's a max-heap
    } else if (hp->type == maxHeap){
        int max;

        if (left < size && hp->data[left].value > hp->data[index].value) {
            max = left;
        } else {
            max = index;
        }
        if (right < size && hp->data[right].value > hp->data[max].value) {
            max = right;
        }
        if (max != index) {
            heap_swap(hp->data, index, max);
            heap_Heapify_down(hp, max);
        }
    }
}


// Modifies a given heap structure by inserting newly built nodes created from the standard input
// Assumes the given heap has been initialized and is empty at the start
// DOES NOT perform heapify
void heap_buildFromStdin (Heap *hp) {

    int m;
    int t = 0;
    int num;
    char buffer[MAX_SIZE];
    fgets(buffer, MAX_SIZE, stdin);

    while (sscanf(buffer + t, "%d%n", &num, &m) != EOF) {

        int hpSize = heap_size(hp);
        int hpCapacity = heap_capacity(hp);

        // Builds node from value directly from stdin
        Node n;
        n.value = num;
        hp->data[hpSize] = n;
        hp->size++;

        // resize if needed
        if (hpCapacity == hpSize)
            heap_resize(hp, hpCapacity * 2);

        t = t + m;
    }
}

// Modifies a given heap structure by inserting newly built nodes created from the values of an auxiliary array
// Assumes the given heap has been initialized and is empty at the start
// DOES NOT perform heapify
void heap_buildFromArr(int *arr, int arr_len, Heap *hp){

    for (int i = 0; i < arr_len; i++) {
        int hpSize = heap_size(hp);
        int hpCapacity = heap_capacity(hp);

        // Builds node from value found at arr[i]
        Node n;
        n.value = arr[i];
        hp->data[hpSize] = n;
        hp->size++;

        // resize if needed
        if (hpCapacity == hpSize)
            heap_resize(hp, hpCapacity * 2);
    }
}

/*
 * ===============================================
 *           Heap Selection Algorithms
 * ===============================================
 */

// Recursively applies extract to find the kth element
// Does not modify the initial heap
// Assumes that the heap structure "hp" has been already heapify-ed
Node heap_rec(Heap *hp, Heap *hpAux, int kth){

    int index;
    for (int i = 0; i <= kth -2; i++){

        index = heap_get_root(hpAux).index;
        // Calculate the position of said node's left and right sons in hp
        int left = heap_left_index(index);
        int right = heap_right_index(index);
        // Extracts node from hpAux
        heap_extract(hpAux);
        // Inserts correct node in hpAux
        if (left < heap_size(hp)) {
            heap_insert(hpAux, hp->data[left]);
        }
        if (right < heap_size(hp)) {
            heap_insert(hpAux, hp->data[right]);
        }
    }
    return heap_get_root(hpAux);
}

// Selection algorithm based on extract and an auxiliary heap structure
// Assumes hp not to be heapify-ed yet
int heap_select(int *arr, int arrLen, int kth, int mode){

    int result = 0;
    Heap hp;
    Heap hpAux;
    heap_init(&hp, unknown);
    heap_init(&hpAux, unknown);
    heap_buildFromArr((int *)arr, arrLen, &hp); // Heapify still not performed at this point

    if (mode == 0) {
        // Modifies the heap by changing its heapType depending on the value of k
        // Modifies the kth value if it is a max heap
        int size = heap_size(&hp);
        if (kth <= size / 2) {
            hp.type = minHeap;
            hpAux.type = minHeap;
        } else {
            kth = size - kth + 1;
            hp.type = maxHeap;
            hpAux.type = maxHeap;
        }

        // Performs heapify to order the heap
        for (int i = (size / 2) - 1; i >= 0; --i) {
            heap_Heapify_down(&hp, i);
        }

        // Sets the index of each of hp's nodes to that of their position in the heap
        for (int i = 0; i < size; i++) {
            hp.data[i].index = i;
        }

        heap_insert(&hpAux, heap_get_root(&hp));
        Node n = heap_rec(&hp, &hpAux, kth);
        result = n.value;
    }

    free(hp.data);
    free(hpAux.data);
    return result;
}
