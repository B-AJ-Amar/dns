#include "parser.h"
#include "logger.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// for this version i will ignore ttl i will just use the named.root that i download manually
named_root_file *parse_named_root_file(const char *filename) {
    FILE *file = fopen(filename, "r");
    if (file == NULL) {
        log_error("Failed to open named.root file: %s\n", filename);
        return NULL;
    }
    named_root_file *nrf = calloc(1, sizeof(named_root_file));
    if (nrf == NULL) {
        log_error("Failed to allocate memory for named.root file structure\n");
        fclose(file);
        return NULL;
    }

    char line[512];
    int  server_index = -1;
    char ns[256];
    char ipv4_str[INET_ADDRSTRLEN];
    char ipv6_str[INET6_ADDRSTRLEN];
    while (fgets(line, sizeof(line), file) != NULL && server_index < ROOT_SERVERS_COUNT) {
        if (line[0] == ';')
            continue;

        if (strstr(line, "NS")) {
            server_index++;
            if (sscanf(line, "%*s %*s %*s NS %255s", ns) == 1) {
                strncpy(nrf->root_servers[server_index].ns, ns, sizeof(nrf->root_servers[server_index].ns) - 1);
            }
        } else if (strstr(line, "AAAA")) {
            if (sscanf(line, "%*s %*s %*s AAAA %s", ipv6_str) == 1) {
                inet_pton(AF_INET6, ipv6_str, &nrf->root_servers[server_index].ipv6);
            }
        } else {
            if (sscanf(line, "%*s %*s %*s A %s", ipv4_str) == 1) {
                inet_pton(AF_INET, ipv4_str, &nrf->root_servers[server_index].ipv4);
            }
        }

        fclose(file);
        return nrf;
    }
}

void free_named_root_file(named_root_file *nrf) {
    if (nrf != NULL) {
        free(nrf);
    }
}

