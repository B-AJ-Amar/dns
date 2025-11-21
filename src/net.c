#include <netinet/in.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

#include "common.h"
#include "dns.h"
#include "logger.h"
#include <unistd.h>

int udp_server_socket(__uint16_t port, __uint16_t buffer_size, char *hostname) {
    int                sockfd;
    struct sockaddr_in server_addr;

    if ((sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0) {
        log_fatal("Failed to create socket\n");
    }

    server_addr.sin_family = AF_INET;
    server_addr.sin_port   = htons(port);
    if (hostname == NULL)
        server_addr.sin_addr.s_addr = INADDR_ANY;
    else
        server_addr.sin_addr.s_addr = inet_addr(hostname);

    if (bind(sockfd, (struct sockaddr *)&server_addr, sizeof(server_addr)) < 0) {
        log_fatal("failed to bind socket to port %d\n", port);
    }

    if (setsockopt(sockfd, SOL_SOCKET, SO_RCVBUF, &buffer_size, sizeof(buffer_size)) < 0) {
        log_fatal("failed to set socket buffer size\n");
    }

    log_info("UDP server listening on %s:%d with buffer size %d bytes\n", hostname, port,
             buffer_size);
    return sockfd;
}

int read_udp_packet(int sockfd, char *buffer, size_t buffer_size, struct sockaddr_in *client_addr,
                    socklen_t *addr_len) {
    ssize_t n = recvfrom(sockfd, buffer, buffer_size, 0, (struct sockaddr *)client_addr, addr_len);
    if (n < 0) {
        log_error("failed to receive UDP packet\n");
        return RET_FAILURE;
    }
    return n;
}

int send_udp_packet(int sockfd, const char *buffer, size_t buffer_size,
                    struct sockaddr_in *client_addr, socklen_t addr_len) {
    ssize_t n = sendto(sockfd, buffer, buffer_size, 0, (struct sockaddr *)client_addr, addr_len);
    if (n < 0) {
        log_error("failed to send UDP packet\n");
        return RET_FAILURE;
    }
    return n;
}

int send_dns_message(int sockfd, dns_message *message, struct sockaddr_in *client_addr,
                     socklen_t addr_len) {

    __uint8_t *msg = deserialize_dns_message(message);

    if (msg == NULL) {
        log_error("failed to deserialize DNS message\n");
        return RET_FAILURE;
    }

    int n = sendto(sockfd, msg, sizeof(msg), 0, (struct sockaddr *)client_addr, addr_len);
    if (n < 0) {
        log_error("failed to send DNS message\n");
        return RET_FAILURE;
    }

    return n;
}
