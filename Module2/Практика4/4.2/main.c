#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <locale.h>

#include "priority_queue.h"

int main(void)
{
    setlocale(LC_ALL,".UTF-8");
    PriorityQueue queue;
    pq_init(&queue);

    srand((unsigned int)time(NULL));

    printf("\nГенерация сообщений:\n");

    for (int i = 1; i <= 15; i++)
    {
        int priority = rand() % 256;

        if (pq_enqueue(&queue, i, priority))
        {
            printf("Добавлено сообщение %d, приоритет %d\n", i, priority);
        }
        else
        {
            printf("Ошибка добавления сообщения\n");
        }
    }

    print_queue(&queue);

    printf("\nДобавление тестовых сообщений:\n");

    pq_enqueue(&queue, 100, 20);
    pq_enqueue(&queue, 101, 19);
    pq_enqueue(&queue, 102, 19);
    pq_enqueue(&queue, 103, 5);
    pq_enqueue(&queue, 104, 30);

    print_queue(&queue);

    int value;
    uint8_t priority;

    printf("\n1. Извлечение первого элемента:\n");

    if (pq_dequeue_first(&queue, &value, &priority))
    {
        printf("Извлечено сообщение %d, приоритет %u\n",
               value, (unsigned)priority);
    }
    else
    {
        printf("Очередь пуста.\n");
    }

    printf("\n2. Извлечение с приоритетом 19:\n");

    if (pq_dequeue_exact(&queue, 19, &value))
    {
        printf("Извлечено сообщение %d\n", value);
    }
    else
    {
        printf("Элемент не найден.\n");
    }

    printf("\n3. Извлечение с приоритетом не ниже 20:\n");

    if (pq_dequeue_at_least(&queue, 20, &value, &priority))
    {
        printf("Извлечено сообщение %d, фактический приоритет %u\n",
               value, (unsigned)priority);
    }
    else
    {
        printf("Подходящий элемент не найден.\n");
    }

    printf("\n3. Извлечение с приоритетом не ниже 20:\n");

    if (pq_dequeue_at_least(&queue, 20, &value, &priority))
    {
        printf("Извлечено сообщение %d, фактический приоритет %u\n",
               value, (unsigned)priority);
    }
    else
    {
        printf("Подходящий элемент не найден.\n");
    }

    print_queue(&queue);

    printf("\nКоличество оставшихся сообщений: %zu\n",
           pq_size(&queue));

    pq_clear(&queue);

    printf("\nОчередь очищена.\n");

    return 0;
}
