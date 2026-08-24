#include <stdlib.h>
typedef struct {
    int weight;
    int index;
} FreeServer;
typedef struct {
    long long free_time;
    int weight;
    int index;
} BusyServer;
void pushFree(FreeServer* heap, int* size, int w, int idx) {
    int i = (*size)++;
    heap[i].weight = w;
    heap[i].index = idx;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[i].weight < heap[p].weight || (heap[i].weight == heap[p].weight && heap[i].index < heap[p].index)) {
            FreeServer tmp = heap[i]; heap[i] = heap[p]; heap[p] = tmp;
            i = p;
        } else break;
    }
}
FreeServer popFree(FreeServer* heap, int* size) {
    FreeServer res = heap[0];
    heap[0] = heap[--(*size)];
    int i = 0;
    while (i * 2 + 1 < *size) {
        int l = i * 2 + 1, r = l + 1, s = (r < *size && (heap[r].weight < heap[l].weight || (heap[r].weight == heap[l].weight && heap[r].index < heap[l].index))) ? r : l;
        if (heap[s].weight < heap[i].weight || (heap[s].weight == heap[i].weight && heap[s].index < heap[i].index)) {
            FreeServer tmp = heap[i]; heap[i] = heap[s]; heap[s] = tmp;
            i = s;
        } else break;
    }
    return res;
}
void pushBusy(BusyServer* heap, int* size, long long ft, int w, int idx) {
    int i = (*size)++;
    heap[i].free_time = ft; heap[i].weight = w; heap[i].index = idx;
    while (i > 0) {
        int p = (i - 1) / 2;
        if (heap[i].free_time < heap[p].free_time || 
           (heap[i].free_time == heap[p].free_time && heap[i].weight < heap[p].weight) ||
           (heap[i].free_time == heap[p].free_time && heap[i].weight == heap[p].weight && heap[i].index < heap[p].index)) {
            BusyServer tmp = heap[i]; heap[i] = heap[p]; heap[p] = tmp;
            i = p;
        } else break;
    }
}
BusyServer popBusy(BusyServer* heap, int* size) {
    BusyServer res = heap[0];
    heap[0] = heap[--(*size)];
    int i = 0;
    while (i * 2 + 1 < *size) {
        int l = i * 2 + 1, r = l + 1, s = l;
        if (r < *size) {
            if (heap[r].free_time < heap[l].free_time || 
               (heap[r].free_time == heap[l].free_time && heap[r].weight < heap[l].weight) ||
               (heap[r].free_time == heap[l].free_time && heap[r].weight == heap[l].weight && heap[r].index < heap[l].index)) {
                s = r;
            }
        }
        if (heap[s].free_time < heap[i].free_time || 
           (heap[s].free_time == heap[i].free_time && heap[s].weight < heap[i].weight) ||
           (heap[s].free_time == heap[i].free_time && heap[s].weight == heap[i].weight && heap[s].index < heap[i].index)) {
            BusyServer tmp = heap[i]; heap[i] = heap[s]; heap[s] = tmp;
            i = s;
        } else break;
    }
    return res;
}
int* assignTasks(int* servers, int serversSize, int* tasks, int tasksSize, int* returnSize) {
    *returnSize = tasksSize;
    int* ans = (int*)malloc(tasksSize * sizeof(int));
    FreeServer* freeHeap = (FreeServer*)malloc(serversSize * sizeof(FreeServer));
    BusyServer* busyHeap = (BusyServer*)malloc(serversSize * sizeof(BusyServer));
    int freeSize = 0, busySize = 0;
    for (int i = 0; i < serversSize; i++) {
        pushFree(freeHeap, &freeSize, servers[i], i);
    }
    long long current_time = 0;
    for (int j = 0; j < tasksSize; j++) {
        if (current_time < j) {
            current_time = j;
        }
        if (freeSize == 0 && busySize > 0 && busyHeap[0].free_time > current_time) {
            current_time = busyHeap[0].free_time;
        }
        while (busySize > 0 && busyHeap[0].free_time <= current_time) {
            BusyServer finished = popBusy(busyHeap, &busySize);
            pushFree(freeHeap, &freeSize, finished.weight, finished.index);
        }
        FreeServer best = popFree(freeHeap, &freeSize);
        ans[j] = best.index;
        pushBusy(busyHeap, &busySize, current_time + tasks[j], best.weight, best.index);
    }
    free(freeHeap);
    free(busyHeap);
    return ans;
}
