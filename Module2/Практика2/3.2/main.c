#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <locale.h>

#include "ipv4.h"

int main(void)
{
    setlocale(LC_ALL,".UTF-8");
    uint32_t gateway, mask;

    int packet_count;
    int local_count = 0;
    int other_count = 0;

    char ip_text[16];
    char mask_text[16];

    printf("Введите IP-адрес шлюза: ");
    if (scanf("%15s", ip_text) != 1)
        return 1;

    if (!parse_ipv4(ip_text, &gateway))
        return 1;

    printf("Введите маску подсети: ");
    if (scanf("%15s", mask_text) != 1)
        return 1;

    if (!parse_mask(mask_text, &mask))
        return 1;

    printf("Введите количество пакетов: ");

    if (scanf("%d", &packet_count) != 1 || packet_count <= 0)
        return 1;

    srand((unsigned int)time(NULL));

    for (int i = 0; i < packet_count; i++)
    {
        uint32_t destination = generate_random_ip();

        char destination_text[16];
        ip_to_string(destination, destination_text);

        if (same_subnet(destination, gateway, mask))
        {
            local_count++;
            printf("%s - своя подсеть\n", destination_text);
        }
        else
        {
            other_count++;
            printf("%s - другая сеть\n", destination_text);
        }
    }

    double local_percent =
        (double)local_count / packet_count * 100;

    double other_percent =
        (double)other_count / packet_count * 100;

    printf("\nСвоя подсеть: %d (%.2f%%)\n",
           local_count, local_percent);

    printf("Другие сети: %d (%.2f%%)\n",
           other_count, other_percent);

    return 0;
}
