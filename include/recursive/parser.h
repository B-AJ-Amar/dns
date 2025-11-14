#ifndef PARSER_H
#define PARSER_H

#include "common.h"
#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <time.h>

#define ROOT_DNS_RECORDS       "https://www.internic.net/domain/named.root"
#define DEFAULT_NAMED_ROOT_TTL 3600000
#define ROOT_SERVERS_COUNT     13

//? usaly named.root has 13 root servers, and all of them have one NS & one A & one AAAA record

typedef struct root_hint {
    char            ns[256];
    struct in_addr  ipv4;
    struct in6_addr ipv6;
} root_hint;

typedef struct named_root_file {
    root_hint root_servers[ROOT_SERVERS_COUNT];
    time_t    last_fetch_time;
} named_root_file;

int              fetch_named_root_file(const char *output_filename);
named_root_file *parse_named_root_file(const char *filename);
void             free_named_root_file(named_root_file *nrf);

#endif // PARSER_H