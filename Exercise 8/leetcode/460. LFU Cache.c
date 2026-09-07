#include <stdlib.h>
typedef struct Node {
    int key, value, freq;
    struct Node *prev, *next;
} Node;
typedef struct {
    Node *head, *tail;
} DList;
typedef struct {
    int capacity, size, min_freq;
    Node** key_table;
    DList** freq_table;
} LFUCache;
static void removeNode(DList* l, Node* n) {
    if (n->prev) n->prev->next = n->next; else l->head = n->next;
    if (n->next) n->next->prev = n->prev; else l->tail = n->prev;
}
static void addHead(DList* l, Node* n) {
    n->next = l->head; n->prev = NULL;
    if (l->head) l->head->prev = n;
    l->head = n;
    if (!l->tail) l->tail = n;
}
static void updateFreq(LFUCache* c, Node* n) {
    DList *old_l = c->freq_table[n->freq];
    removeNode(old_l, n);
    if (!old_l->head && c->min_freq == n->freq) c->min_freq++;
    n->freq++;
    if (!c->freq_table[n->freq]) c->freq_table[n->freq] = (DList*)calloc(1, sizeof(DList));
    addHead(c->freq_table[n->freq], n);
}
LFUCache* lFUCacheCreate(int capacity) {
    LFUCache* c = (LFUCache*)malloc(sizeof(LFUCache));
    c->capacity = capacity; c->size = 0; c->min_freq = 0;
    c->key_table = (Node**)calloc(100001, sizeof(Node*));
    c->freq_table = (DList**)calloc(200005, sizeof(DList*));
    return c;
}
int lFUCacheGet(LFUCache* obj, int key) {
    if (obj->capacity <= 0 || !obj->key_table[key]) return -1;
    updateFreq(obj, obj->key_table[key]);
    return obj->key_table[key]->value;
}
void lFUCachePut(LFUCache* obj, int key, int value) {
    if (obj->capacity <= 0) return;
    Node* n = obj->key_table[key];
    if (n) {
        n->value = value;
        updateFreq(obj, n);
        return;
    }
    if (obj->size >= obj->capacity) {
        Node* evict = obj->freq_table[obj->min_freq]->tail;
        removeNode(obj->freq_table[obj->min_freq], evict);
        obj->key_table[evict->key] = NULL;
        free(evict);
        obj->size--;
    }
    Node* new_n = (Node*)malloc(sizeof(Node));
    *new_n = (Node){key, value, 1, NULL, NULL};
    obj->key_table[key] = new_n;
    obj->min_freq = 1;
    if (!obj->freq_table[1]) obj->freq_table[1] = (DList*)calloc(1, sizeof(DList));
    addHead(obj->freq_table[1], new_n);
    obj->size++;
}
void lFUCacheFree(LFUCache* obj) {
    for (int i = 0; i <= 100000; i++) if (obj->key_table[i]) free(obj->key_table[i]);
    for (int i = 0; i <= 200004; i++) if (obj->freq_table[i]) free(obj->freq_table[i]);
    free(obj->key_table); free(obj->freq_table); free(obj);
}
