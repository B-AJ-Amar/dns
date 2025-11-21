#include "common.h"
#include "dns.h"
#include "logger.h"
#include "net.h"
#include "recursive/parser.h"
#include "recursive/rec_balancer.h"
#include <stdio.h>
#include <stdlib.h>

int main() {

    // ? ====================================================
    // ? INIT
    // ? ====================================================
    // TODO : LOAD CONFIG FROM FILE
    init_logger(&lg, LOG_DEBUG, 1, DEFAULT_TIME_FORMAT, stdout);
    if (!lg.output)
        log_fatal("Failed to initialize logger\n");

    char buffer[4096];

    int sock = udp_server_socket(8080, 1024, "127.0.0.1");

    if (sock < 0)
        log_fatal("Failed to create UDP server socket\n");

    named_root_file *nrf = parse_named_root_file("named.root");
    if (!nrf)
        log_fatal("Failed to parse named.root file\n");

    int balancer_type = ALWAYS_FIRST_BALANCER, balancer_state = 0;


    root_hint root_server;

    uint16_t default_root_server_port = 5300; //? to avoid problems

    // ? ====================================================
    // ? MAIN LOOP
    // ? ====================================================
    while (TRUE) {
        struct sockaddr_in client_addr;
        socklen_t          addr_len = sizeof(client_addr);

        int n = read_udp_packet(sock, buffer, sizeof(buffer), &client_addr, &addr_len);
        if (n > 0) {
            log_info("Received %d bytes from client\n", n);

            dns_message *message = parse_dns_message(buffer, n);
            if (message) {
                log_info("Parsed DNS message with ID: %u\n", message->header.id);

                // i need to make a pending messages logic to know which request is for which
                // response

                free(message);
            } else {
                log_error("Failed to parse DNS message\n");
                // todo : send error to the client
            }
        }
    }

    printf("Hello, World!\n");
    return 0;
}