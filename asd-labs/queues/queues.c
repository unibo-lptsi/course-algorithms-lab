#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

/**
 * Esercizio
 * - implementare la funzione queue_remove(Queue *q)
 */

// NB: compile with gcc "-DDEBUG"
#ifdef DEBUG
#define LOG(fmt, ...) fprintf(stdout, fmt, ##__VA_ARGS__)
#else
#define LOG(fmt, ...)
#endif

typedef int TInfo;

/* Queue implementation */

struct SQueue {
    TInfo *item;
    int from;
    int size;
    int capacity;
};
typedef struct SQueue Queue;

Queue queue_create(int capacity);
void queue_destroy(Queue *q);
bool queue_add(Queue *q, TInfo value);
TInfo queue_remove(Queue *q);

Queue queue_create(int capacity) {
    Queue q;
    q.item = (TInfo*) malloc(sizeof(TInfo) * capacity);
    q.capacity = capacity;
    q.size = 0;
    q.from = 0;
    return q;
}

void queue_destroy(Queue *q) {
    if(q->item != NULL) {
        free(q->item);
        q->item = NULL;
    }
    q->size = 0;
    q->capacity = 0;
    q->from = 0;
}

bool queue_add(Queue *q, TInfo value) {
    LOG("[ACTION] Requested adding %d to queue.\n", value);
    if(q->size < q->capacity) {
        q->item[(q->from + q->size) % q->capacity] = value;
        q->size++;
        return true;
    }
    LOG("[ACTION] Cannot add %d to queue: full.\n", value);
    return false;
}

TInfo queue_remove(Queue *q) {
    LOG("[ACTION] Requested removal from queue.\n");
    // TODO: implement removal from queue
    LOG("TODO: REMOVAL TO BE IMPLEMENTED");
}

int _queue_back_index(Queue *q) {
    return (q->from + q->size - 1) % q->capacity;
}

void queue_print(Queue *q) {
    printf("QUEUE = [");
    if(q->size>0){
        int i = q->from;
        int to = _queue_back_index(q);
        for(; i != to; i = (i+1) % q->capacity) {
            printf("%d, ", q->item[i]);
        }
        printf("%d", q->item[i]);
    }
    printf("] INTERNAL REPR = (capacity=%d,from=%d;to=%d)[", q->capacity, q->from, _queue_back_index(q));
    for(int i=0; i<q->capacity; i++) {
        printf("%d,", q->item[i]);
    }
    printf("]");
}

int main() {
    Queue q = queue_create(4);
    queue_add(&q, 5);
    queue_add(&q, 7);
    queue_print(&q); 
    puts("");
    printf("Removed from queue: %d\n", queue_remove(&q));
    queue_add(&q, 3);
    queue_add(&q, 1);
    queue_print(&q);
    puts("");
    queue_remove(&q);
    queue_add(&q, 9);
    queue_print(&q);
    puts("");
    queue_destroy(&q);
    return 0;
}