#include "logger.h"
#include "recursive/parser.h"
#include "recursive/rec_balancer.h"
#include <stdio.h>
#include <stdlib.h>

root_hint get_root_server(named_root_file *rnf, enum BALANCER_TYPE type, int *state) {
    //? we need state for round robin and first available
    switch (type) {
    case RANDOM_BALANCER: {
        return rnf->root_servers[rand() % ROOT_SERVERS_COUNT];
    }
    case FIRST_AVAILABLE_BALANCER: {
        return rnf->root_servers[*state % ROOT_SERVERS_COUNT];
    }
    case ROUND_ROBIN_BALANCER: {
        return rnf->root_servers[*state++ % ROOT_SERVERS_COUNT];
    }
    case ALWAYS_FIRST_BALANCER: {
        return rnf->root_servers[0];
    }
    default:
        log_error("Unknown balancer type\n");
        return rnf->root_servers[0];
    }
}
