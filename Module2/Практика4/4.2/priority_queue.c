#include "priority_queue.h"
#include <stdlib.h>
#include <stdio.h>

void pq_init(PriorityQueue *queue)
{
    queue->head = NULL;
    queue->size = 0;
}

bool pq_is_empty(const PriorityQueue *queue)
{
    return queue == NULL || queue->head == NULL;
}

size_t pq_size(const PriorityQueue *queue)
{
    return queue == NULL ? 0 : queue->size;
}

bool pq_enqueue(PriorityQueue *queue, int value, int priority)
{
    if (queue == NULL || priority < 0 || priority > 255)
        return false;

    PQNode *new_node = malloc(sizeof(PQNode));

    if (new_node == NULL) return false;

    new_node->value = value;
    new_node->priority = (uint8_t)priority;
    new_node->next = NULL;

    if (queue->head == NULL || queue->head->priority > new_node->priority)
    {
        new_node->next = queue->head;
        queue->head = new_node;
    }
    else
    {
        PQNode *current = queue->head;

        while (current->next != NULL &&
               current->next->priority <= new_node->priority)
        {
            current = current->next;
        }
        new_node->next = current->next;
        current->next = new_node;
    }

    queue->size++;
    return true;
}

static bool remove_node(PriorityQueue *queue, PQNode *previous, PQNode *target, int *value, uint8_t *priority)
{
    if (queue == NULL || target == NULL || value == NULL)
        return false;

    if (previous == NULL)
        queue->head = target->next;
    else
        previous->next = target->next;

    *value = target->value;

    if (priority != NULL)
        *priority = target->priority;

    free(target);
    queue->size--;

    return true;
}

bool pq_dequeue_first( PriorityQueue *queue, int *value, uint8_t *priority)
{
    if (queue == NULL || queue->head == NULL || value == NULL)
        return false;

    return remove_node(queue, NULL, queue->head, value, priority);
}

bool pq_dequeue_exact(PriorityQueue *queue, uint8_t priority, int *value)
{
    if (queue == NULL || value == NULL) return false;

    PQNode *current = queue->head;
    PQNode *previous = NULL;

    while (current != NULL)
    {
        if (current->priority == priority)
        {
            return remove_node(queue, previous, current, value, NULL);
        }

        if (current->priority > priority)
            break;
        previous = current;
        current = current->next;
    }
    return false;
}

bool pq_dequeue_at_least(PriorityQueue *queue, uint8_t min_priority, int *value, uint8_t *actual_priority)
{
    if (queue == NULL || queue->head == NULL || value == NULL)
        return false;

    PQNode *current = queue->head;
    PQNode *previous = NULL;

    PQNode *candidate = NULL;
    PQNode *candidate_previous = NULL;

    while (current != NULL &&
           current->priority <= min_priority)
    {
        
        if (candidate == NULL ||
            candidate->priority != current->priority)
        {
            candidate = current;
            candidate_previous = previous;
        }

        previous = current;
        current = current->next;
    }

    if (candidate == NULL)
        return false;

    return remove_node(queue, candidate_previous, candidate, value, actual_priority);
}

void pq_clear(PriorityQueue *queue)
{
    if (queue == NULL)
        return;

    PQNode *current = queue->head;

    while (current != NULL)
    {
        PQNode *next = current->next;
        free(current);
        current = next;
    }
    queue->head = NULL;
    queue->size = 0;
}

void print_queue(const PriorityQueue *queue)
{
    printf("\nТекущее состояние очереди:\n");

    if (pq_is_empty(queue))
    {
        printf("Очередь пуста.\n");
        return;
    }

    PQNode *current = queue->head;

    while (current != NULL)
    {
        printf("[Сообщение: %d | Приоритет: %u] \n",
               current->value,
               (unsigned)current->priority);
        current = current->next;
    }
    printf("\n");
}