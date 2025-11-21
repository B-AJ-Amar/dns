#ifndef NET_H
#define NET_H

#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int udp_server_socket(__uint16_t port, __uint16_t buffer_size, char *hostname);
int read_udp_packet(int sockfd, char *buffer, size_t buffer_size, struct sockaddr_in *client_addr,
                    socklen_t *addr_len);
int send_udp_packet(int sockfd, const char *buffer, size_t buffer_size,
                    struct sockaddr_in *client_addr, socklen_t addr_len);

int send_dns_message(int sockfd, dns_message *message, struct sockaddr_in *client_addr,
                     socklen_t addr_len);

#endif // NET_H