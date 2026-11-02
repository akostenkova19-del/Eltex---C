#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ipv4.h"

int parse_ipv4(const char *text, uint32_t *ip)
{
    unsigned int a, b, c, d;

    if (sscanf(text, "%u.%u.%u.%u", &a, &b, &c, &d) != 4)
        return 0;

    if (a > 255 || b > 255 || c > 255 || d > 255)
        return 0;

    *ip = (a << 24) | (b << 16) | (c << 8) | d;

    return 1;
}

int parse_mask(const char *text, uint32_t *mask)
{
    if (text[0] == '/')
    {
        char *end;
        long prefix = strtol(text + 1, &end, 10);

        if (text[1] == '\0' || *end != '\0' ||
            prefix < 0 || prefix > 32)
            return 0;

        if (prefix == 0)
            *mask = 0;
        else
            *mask = UINT32_MAX << (32 - prefix);

        return 1;
    }

    if (!parse_ipv4(text, mask))
        return 0;

    uint32_t inverse = ~(*mask);

    if ((inverse & (inverse + 1)) != 0)
        return 0;

    return 1;
}

uint32_t generate_random_ip(void)
{
    uint32_t ip = 0;

    for (int i = 0; i < 4; i++)
    {
        ip = (ip << 8) | (rand() & 0xFF);
    }

    return ip;
}

void ip_to_string(uint32_t ip, char result[16])
{
    sprintf(result, "%u.%u.%u.%u",
            (ip >> 24) & 0xFF,
            (ip >> 16) & 0xFF,
            (ip >> 8) & 0xFF,
            ip & 0xFF);
}

int same_subnet(uint32_t destination,
                uint32_t gateway,
                uint32_t mask)
{
    return (destination & mask) == (gateway & mask);
}