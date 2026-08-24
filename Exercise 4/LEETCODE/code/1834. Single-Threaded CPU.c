#include <stdlib.h>
typedef struct {
    int enqueue_time;
    int processing_time;
    int index;
} CPUTask;
typedef struct {
    int processing_time;
    int index;
} MinHeapNode;
int cmpTasks(const void* a, const void* b) {
    CPUTask* t1 = (CPUTask*)a;
    CPUTask* t2 = (CPUTask*)b;
    if (t1->enqueue_time != t2->enqueue_time) {
        return (t1->enqueue_time > t2->enqueue_time) - (t1->enqueue_time < t2->enqueue_time);
    }
    return t1->index - t2->index;
}
int* getOrder(int** tasks, int tasksSize, int* tasksColSize, int* returnSize) {
    *returnSize = tasksSize;
    int* result = (int*)malloc(tasksSize * sizeof(int));
    CPUTask* sortedTasks = (CPUTask*)malloc(tasksSize * sizeof(CPUTask));
    for (int i = 0; i < tasksSize; ++i) {
        sortedTasks[i].enqueue_time = tasks[i][0];
        sortedTasks[i].processing_time = tasks[i][1];
        sortedTasks[i].index = i;
    }
    qsort(sortedTasks, tasksSize, sizeof(CPUTask), cmpTasks);
    MinHeapNode* heap = (MinHeapNode*)malloc(tasksSize * sizeof(MinHeapNode));
    int heapSize = 0;
    long long currentTime = 0;
    int taskPointer = 0;
    int resPointer = 0;
    while (resPointer < tasksSize) {
        if (heapSize == 0 && currentTime < sortedTasks[taskPointer].enqueue_time) {
            currentTime = sortedTasks[taskPointer].enqueue_time;
        }
        while (taskPointer < tasksSize && sortedTasks[taskPointer].enqueue_time <= currentTime) {
            int i = heapSize++;
            heap[i].processing_time = sortedTasks[taskPointer].processing_time;
            heap[i].index = sortedTasks[taskPointer].index;
            while (i > 0) {
                int parent = (i - 1) >> 1;
                if (heap[i].processing_time < heap[parent].processing_time || 
                   (heap[i].processing_time == heap[parent].processing_time && heap[i].index < heap[parent].index)) {
                    MinHeapNode temp = heap[i];
                    heap[i] = heap[parent];
                    heap[parent] = temp;
                    i = parent;
                } else {
                    break;
                }
            }
            taskPointer++;
        }
        result[resPointer++] = heap[0].index;
        currentTime += heap[0].processing_time;
        --heapSize;
        heap[0] = heap[heapSize];
        int idx = 0;
        while ((idx << 1) + 1 < heapSize) {
            int left = (idx << 1) + 1;
            int right = left + 1;
            int smallest = left;
            if (right < heapSize) {
                if (heap[right].processing_time < heap[left].processing_time ||
                   (heap[right].processing_time == heap[left].processing_time && heap[right].index < heap[left].index)) {
                    smallest = right;
                }
            }
            if (heap[smallest].processing_time < heap[idx].processing_time ||
               (heap[smallest].processing_time == heap[idx].processing_time && heap[smallest].index < heap[idx].index)) {
                MinHeapNode temp = heap[idx];
                heap[idx] = heap[smallest];
                heap[smallest] = temp;
                idx = smallest;
            } else {
                break;
            }
        }
    }
    free(sortedTasks);
    free(heap);
    return result;
}
