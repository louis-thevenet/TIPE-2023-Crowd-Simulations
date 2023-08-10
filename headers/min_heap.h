struct Heap {
  int *arr;
  int *pos;
  int count;
  int capacity;
  int heap_type; // for min heap , 1 for max heap
};
typedef struct Heap Heap;

Heap *CreateHeap(int capacity, int heap_type);

void FreeHeap(Heap *h);

void insert(Heap *h, int key, int *dist);

int is_in_heap(Heap *h, int u);

void update_key(Heap *h, int i, int *d);

void print(Heap *h, int *d);

void heapify_bottom_top(Heap *h, int index, int *dist);

void heapify_top_bottom(Heap *h, int parent_node, int *dist);

int PopMin(Heap *h, int *dist);