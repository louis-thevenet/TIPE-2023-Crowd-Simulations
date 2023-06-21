#include "min_heap.h"
#include <stdio.h>
#include <stdlib.h>

Heap *CreateHeap(int capacity, int heap_type) {
    Heap *h = (Heap *) malloc(sizeof(Heap));

    if (h == NULL) {
        printf("Memory Error!");
        exit(0);
    }
    h->heap_type = heap_type;
    h->count = 0;
    h->capacity = capacity;
    h->arr = (int *) malloc(capacity * sizeof(int));
    h->pos = (int *) malloc(capacity * sizeof(int));

    if (h->arr == NULL || h->pos == NULL) {
        printf("Memory Error!");
        exit(0);
    }
    return h;
}

void FreeHeap(Heap *h) {
    free(h->arr);
    free(h->pos);
    free(h);
}

void insert(Heap *h, int key, int *dist) {
    if (h->count < h->capacity) {
        h->arr[h->count] = key;
        h->pos[key] = h->count;
        heapify_bottom_top(h, h->count, dist);
        h->count++;
    }
}

void heapify_bottom_top(Heap *h, int index, int *dist) {
    int temp_val;
    int parent_ind = (index - 1) / 2;

    if (dist[h->arr[parent_ind]] > dist[h->arr[index]]) {
        temp_val = h->arr[parent_ind];

        h->arr[parent_ind] = h->arr[index];
        h->arr[index] = temp_val;

        h->pos[h->arr[index]] = index;
        h->pos[h->arr[parent_ind]] = parent_ind;

        heapify_bottom_top(h, parent_ind, dist);
    }
}

void heapify_top_bottom(Heap *h, int parent_node, int *dist) {
    int left = parent_node * 2 + 1;
    int right = parent_node * 2 + 2;
    int min_pos;
    int min;
    int temp;

    if (left >= h->count || left < 0)
        left = -1;
    if (right >= h->count || right < 0)
        right = -1;

    if (left != -1 && dist[h->arr[left]] < dist[h->arr[parent_node]]) {
        min_pos = h->pos[h->arr[left]];
        min = left;
    } else
        min = parent_node;
    if (right != -1 && dist[h->arr[right]] < dist[h->arr[min]]) {
        min_pos = h->pos[h->arr[right]];
        min = right;
    }
    if (min != parent_node) {
        temp = h->arr[min];
        h->arr[min] = h->arr[parent_node];
        h->arr[parent_node] = temp;

        h->pos[h->arr[min]] = h->pos[h->arr[parent_node]];
        h->pos[h->arr[parent_node]] = min_pos;

        heapify_top_bottom(h, min, dist);
    }
}

int PopMin(Heap *h, int *dist) {
    int pop;
    if (h->count == 0) {
        printf("\n__Heap is Empty__ (Pop)\n");
        return -1;
    }

    pop = h->arr[0];
    h->arr[0] = h->arr[h->count - 1];
    h->pos[0] = h->pos[h->count - 1];
    h->count--;
    heapify_top_bottom(h, 0, dist);
    return pop;
}

int is_in_heap(Heap *h, int u) {
    if (h->count == 0) {
        return 0;
    }
    for (int i = 0; i < h->capacity; i++) {
        if (h->arr[i] == u)
            return 1;
    }
    return 0;
}

void update_key(Heap *h, int i, int *d) {
    int temp;
    while (i != 0 && d[h->arr[i]] > d[h->arr[(i - 1) / 2]]) {
        temp = h->arr[(i - 1) / 2];
        h->arr[(i - 1) / 2] = h->arr[i];
        h->arr[i] = temp;

        i = (i - 1) / 2;
    }
}