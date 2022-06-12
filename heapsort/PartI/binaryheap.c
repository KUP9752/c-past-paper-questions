#include "binaryheap.h"
#include <assert.h>
#include <string.h>
#define NULLCHECK(ptr, msg) \
    if (!ptr) { \
        perror(msg); \
    };

/*allocate memory in the heap for a node_heap type element and returns a pointer to the new node_heap*/

node_heap *allocate_node_heap(void)
{
    node_heap *newNode = malloc(sizeof(node_heap));
    NULLCHECK(newNode, "Allocate Node: memory allocation error");
    return newNode; //returns null if cannot allocate the pointer
}

/*initialise the heap array elements*/

void initial_heap(node_heap **heap, char* sequence)
{
    for (int i = 0; sequence[i]; i++) {
        heap[i + 1] = allocate_node_heap(); // i + 1 as we start indexing at 1
        heap[i + 1] -> key = &sequence[i];
        heap[i + 1] -> position = i + 1; 
    }
}


/*print every element of the heap array*/
void print_elem_heap(node_heap **heap, int length)
{
    for(int i = 1; i < length; i++) {
        printf("%c ", heap[i] -> key[0]);
    }
    printf("\n");

    for(int i = 1; i < length; i++) {
        printf("%d ", heap[i] -> position);
    }
    printf("\n");
}

/* returns the index in the heap array where the parent is allocated for the index passed as argument*/

int parent(int index)
{
   return index / 2;
}

/* returns the index in the heap array where the left child is allocated for the index passed as argument*/

int left_child(int index)
{
   return index * 2;
}

/* returns the index in the heap array where the right child is allocated for the index passed as argument*/

int right_child(int index)
{
    return index * 2 + 1;
}

/* exchange node_heap node1 to node_heap node2*/ 

void swap(node_heap *node1, node_heap *node2)
{
    node_heap tempNode;
    tempNode.key = node1 -> key;
    tempNode.position = node1 -> position;

    node1 -> key = node2 -> key;
    node1 -> position = node2 -> position;
    
    node2 -> key = tempNode.key;
    node2 -> position = tempNode.position;
}

/*Moves down the value of the heap[current] so the subtree rooted at index "current" satisfies with the max-heap property*/ 

void max_heapify(node_heap **heap, int current, int heap_size)
{
    int max_pos = current;

    if (left_child(current) < heap_size) {
        max_pos = (strcmp(heap[max_pos]->key, heap[left_child(current)]->key) < 0) 
            ? left_child(current) : max_pos;
    }

    if (right_child(current) < heap_size) {
        max_pos =  (strcmp(heap[max_pos]->key, heap[right_child(current)]->key) < 0)
            ? right_child(current) : max_pos;
    }

    if (max_pos != current) {
        swap(heap[current], heap[max_pos]);
        max_heapify(heap, max_pos, heap_size);
    }

}

/*it orders the heap so the ordered heap complies the max-heap property*/

void build_max_heap(node_heap **heap, int heap_size)
{
    //starts from final non-leaf node:
    for (int i = parent(heap_size + 1); i > 0; i--) {
        max_heapify(heap, i, heap_size);
    }
}

/*starting from a max-heap ordered array, it moves the largest item from the heap and it into the array position left as the heap shrinks*/

void heapsort(node_heap **heap, int length)
{
   build_max_heap(heap, length);
   for(int i = length ; i > 1; ) {
        i--;
        swap(heap[1], heap[i]);
        max_heapify(heap, 1, i);
   }
    
}

/*free the memory allocated by a node_heap type element in the heap*/

void free_node(node_heap *node)
{
    if (node) {
        free(node -> key);
        free(node);
    }
}

/*free the memory allocated in the heap array*/

void free_heap(node_heap **heap, int length)
{
    for(int i = 1; i < length; i++){
        free_node(heap[i]);
    }
}


