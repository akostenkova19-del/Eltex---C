#ifndef IPV4_H
#define IPV4_H

#include <stdint.h>

int parse_ipv4(const char*, uint32_t*);
int parse_mask(const char*, uint32_t*);

uint32_t generate_random_ip();

void ip_to_string(uint32_t, char result[16]);

int same_subnet(uint32_t destination, uint32_t network, uint32_t mask);

#endif