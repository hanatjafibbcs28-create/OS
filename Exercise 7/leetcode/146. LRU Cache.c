#include <stdlib.h>
typedef struct Node {
    int key;
    int value;
    struct Node* prev;
    struct Node* next;
} Node;
typedef struct {
    int capacity;
    int count;
    Node head_sentinel;
    Node tail_sentinel;
    Node* map[10001];       
    Node* node_pool;     
    int pool_index;
} LRUCache;
#define LINK_NODES(p, n) { (p)->next = (n); (n)->prev = (p); }
void unlinkNode(Node* node) {
    LINK_NODES(node->prev, node->next);
}
void insertAtHead(LRUCache* obj, Node* node) {
    Node* first = obj->head_sentinel.next;
    LINK_NODES(&(obj->head_sentinel), node);
    LINK_NODES(node, first);
}
LRUCache* lRUCacheCreate(int capacity) {
    LRUCache* cache = (LRUCache*)malloc(sizeof(LRUCache));
    cache->capacity = capacity;
    cache->count = 0;
    cache->pool_index = 0;
    LINK_NODES(&(cache->head_sentinel), &(cache->tail_sentinel));
    cache->node_pool = (Node*)malloc(capacity * sizeof(Node));
    for (int i = 0; i <= 10000; ++i) {
        cache->map[i] = NULL;
    } 
    return cache;
}

int lRUCacheGet(LRUCache* obj, int key) {
    Node* node = obj->map[key];
    if (!node) return -1;
    unlinkNode(node);
    insertAtHead(obj, node);
    return node->value;
}
void lRUCachePut(LRUCache* obj, int key, int value) {
    Node* node = obj->map[key]; 
    if (node) {
        node->value = value;
        unlinkNode(node);
        insertAtHead(obj, node);
    } else {
        if (obj->count == obj->capacity) {
            Node* lruNode = obj->tail_sentinel.prev;
            obj->map[lruNode->key] = NULL;
            unlinkNode(lruNode);         
            node = lruNode; 
        } else {
            node = &(obj->node_pool[obj->pool_index++]);
            obj->count++;
        }       
        node->key = key;
        node->value = value;
        obj->map[key] = node;
        insertAtHead(obj, node);
    }
}
void lRUCacheFree(LRUCache* obj) {
    free(obj->node_pool); 
    free(obj);
}
