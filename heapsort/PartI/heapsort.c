#include "binaryheap.h"
#include<string.h>


int main(int argc, char **argv)
{
    if (argc != 2) {
        perror("Invalid number of arguments");
        exit(EXIT_FAILURE);
    }

    char *string = argv[1];
    //node_heap **heap = calloc(strlen(string) + 1, sizeof(node_heap));
    int heap_size = strlen(string) + 1;
    node_heap *heap[heap_size];
    initial_heap(heap, string);
    print_elem_heap(heap, heap_size);

    build_max_heap(heap, heap_size);
    print_elem_heap(heap, heap_size);

    heapsort(heap, heap_size);
    print_elem_heap(heap, heap_size);


}