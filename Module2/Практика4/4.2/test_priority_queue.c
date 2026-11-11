
#include <stdio.h>
#include <assert.h>
#include "priority_queue.h"

void test_enqueue(void)
{
    PriorityQueue queue;
    pq_init(&queue);

    assert(pq_is_empty(&queue));

    assert(pq_enqueue(&queue, 1, 10));
    assert(pq_enqueue(&queue, 2, 5));
    assert(pq_enqueue(&queue, 3, 20));

    assert(pq_size(&queue) == 3);
    assert(!pq_is_empty(&queue));

    assert(queue.head->priority == 5);
    assert(queue.head->next->priority == 10);
    assert(queue.head->next->next->priority == 20);

    assert(pq_enqueue(&queue, 4, 0));
    assert(pq_enqueue(&queue, 5, 255));

    assert(!pq_enqueue(&queue, 6, -1));
    assert(!pq_enqueue(&queue, 7, 256));

    pq_clear(&queue);

    printf("test_enqueue: OK\n");
}

void test_dequeue_first(void)
{
    PriorityQueue queue;
    pq_init(&queue);

    pq_enqueue(&queue, 10, 20);
    pq_enqueue(&queue, 20, 5);
    pq_enqueue(&queue, 30, 10);

    int value;
    uint8_t priority;

    assert(pq_dequeue_first(&queue, &value, &priority));

    assert(value == 20);
    assert(priority == 5);

    assert(pq_size(&queue) == 2);

    assert(pq_dequeue_first(&queue, &value, &priority));

    assert(value == 30);
    assert(priority == 10);

    pq_clear(&queue);

    assert(!pq_dequeue_first(&queue, &value, &priority));

    printf("test_dequeue_first: OK\n");
}

void test_dequeue_exact(void)
{
    PriorityQueue queue;
    pq_init(&queue);

    pq_enqueue(&queue, 1, 10);
    pq_enqueue(&queue, 2, 20);
    pq_enqueue(&queue, 3, 10);
    pq_enqueue(&queue, 4, 30);

    int value;

    assert(pq_dequeue_exact(&queue, 10, &value));

    assert(value == 1);

    assert(pq_dequeue_exact(&queue, 10, &value));
    assert(value == 3);
    assert(!pq_dequeue_exact(&queue, 50, &value));

    assert(pq_size(&queue) == 2);

    pq_clear(&queue);

    printf("test_dequeue_exact: OK\n");
}
void test_dequeue_at_least(void)
{
    PriorityQueue queue;
    pq_init(&queue);

    pq_enqueue(&queue, 1, 5);
    pq_enqueue(&queue, 2, 19);
    pq_enqueue(&queue, 3, 20);
    pq_enqueue(&queue, 4, 25);
    pq_enqueue(&queue, 5, 19);

    int value;
    uint8_t priority;

    assert(pq_dequeue_at_least(
        &queue, 20, &value, &priority
    ));

    assert(value == 3);
    assert(priority == 20);

    assert(pq_dequeue_at_least(
        &queue, 20, &value, &priority
    ));

    assert(value == 2);
    assert(priority == 19);

    assert(pq_dequeue_at_least(
        &queue, 20, &value, &priority
    ));

    assert(value == 5);
    assert(priority == 19);

    pq_clear(&queue);

    printf("test_dequeue_at_least: OK\n");
}

int main(void)
{
    test_enqueue();
    test_dequeue_first();
    test_dequeue_exact();
    test_dequeue_at_least();
    return 0;
}
