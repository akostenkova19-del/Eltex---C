
#ifndef PRIORITY_QUEUE_H
#define PRIORITY_QUEUE_H

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>

typedef struct PQNode
{
    int value;
    uint8_t priority;
    struct PQNode *next;
} PQNode;

typedef struct PriorityQueue
{
    PQNode *head;
    size_t size;
} PriorityQueue;

void pq_init(PriorityQueue *queue);

bool pq_is_empty(const PriorityQueue *queue);

size_t pq_size(const PriorityQueue *queue);

bool pq_enqueue(PriorityQueue *queue, int value, int priority);

bool pq_dequeue_first(PriorityQueue *queue, int *value, uint8_t *priority);

bool pq_dequeue_exact(PriorityQueue *queue, uint8_t priority, int *value);

bool pq_dequeue_at_least(PriorityQueue *queue, uint8_t min_priority, int *value, uint8_t *actual_priority);

void pq_clear(PriorityQueue *queue);

void print_queue(const PriorityQueue *queue);

#endif
