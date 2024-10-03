
#include <stdio.h>
#include <stdlib.h>

#include "some_heap.h"

#define KEY_NOT_PRESENT -1


heap_t *heap_create(int capacity) {
    heap_t *heap = malloc(sizeof(heap_t));
    if (!heap) {
        return NULL;
    }
    heap->data = malloc(capacity * sizeof(heap_node_t));
    if (!heap->data) {
        free(heap);
        return NULL;
    }
    heap->size = 0;
    heap->capacity = capacity;
    return heap;
}

void heap_free(heap_t *heap) {
    free(heap->data);
    free(heap);
}

unsigned int heap_size(heap_t *heap) {
    return heap->size;
}

unsigned int heap_parent(unsigned int index) {
    return (index - 1) / 2;
}

unsigned int heap_left_child(unsigned int index) {
    return 2 * index + 1;
}

unsigned int heap_right_child(unsigned int index) {
    return 2 * index + 2;
}

unsigned int heap_level(unsigned int index) {
    unsigned int heap_level = 0;
    while (index != 0) {
        index = heap_parent(index);
        heap_level++;
    }
    return heap_level;
}

void heap_print(heap_t *heap) {
    for (int ix = 0; ix < heap_size(heap); ix++) {
        printf("%3d - %3d : " HEAP_KEY_FORMAT "\n", heap_level(ix), ix,
               heap->data[ix].key);
    }
    printf("\n");
}

void heap_swap(heap_t *heap, int index1, int index2) {
    heap_node_t temp_node = heap->data[index1];
    heap->data[index1] = heap->data[index2];
    heap->data[index2] = temp_node;
}

void heap_bubble_up(heap_t *heap, int index) {
    while (index > 0) {
        unsigned int parent_node = heap_parent(index);
        if (heap->data[index].key < heap->data[parent_node].key) {
            heap_swap(heap, index, parent_node);
            index = parent_node;
        } else {
            break;
        }
    }
}

void heap_bubble_down(heap_t *heap, int index) {
    unsigned int left_node, right_node, smallest_node;
    while (1) {
        left_node = heap_left_child(index);
        right_node = heap_right_child(index);
        smallest_node = index;
        if (left_node < heap_size(heap) && heap->data[left_node].key < heap->data[smallest_node].key) {
            smallest_node = left_node;
        } else if (right_node < heap_size(heap) && heap->data[right_node].key < heap->data[smallest_node].key) {
            smallest_node = right_node;
        } else if (smallest_node == index) {
            break;
        }
        heap_swap(heap, index, smallest_node);
        index = smallest_node;
    }
}

void heap_insert(heap_t *heap, heap_key_t key, heap_value_t data) {
    if (heap_size(heap) == heap->capacity) {
        return;
    }

    heap->data[heap_size(heap)].key = key;
    heap->data[heap_size(heap)].value = data;
    heap->size++;

    heap_bubble_up(heap, heap_size(heap) - 1);
}


heap_value_t heap_remove_min(heap_t *heap) {
    if (heap_size(heap) == 0) {
        return NULL;
    }

    heap_value_t min = heap->data[0].value;

    heap->size--;

    // move last element to the root
    heap->data[0] = heap->data[heap_size(heap)];

    // then bubble it down to its correct position
    heap_bubble_down(heap, 0);

    return min;
}
