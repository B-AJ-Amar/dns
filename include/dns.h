#ifndef DNS_H
#define DNS_H

#include <stdio.h>
#include <stdlib.h>

// ? i masks and shift , to make it esey later: (dns_header & DNS_{H}_MASK) >> DNS_{H}_SHIFT
#define DNS_QR_MASK     0x8000 // 1000 0000 0000 0000
#define DNS_OPCODE_MASK 0x7800 // 0111 1000 0000 0000
#define DNS_AA_MASK     0x0400 // 0000 0100 0000 0000
#define DNS_TC_MASK     0x0200 // 0000 0010 0000 0000
#define DNS_RD_MASK     0x0100 // 0000 0001 0000 0000
#define DNS_RA_MASK     0x0080 // 0000 0000 1000 0000
#define DNS_Z_MASK      0x0070 // 0000 0000 0111 0000
#define DNS_RCODE_MASK  0x000F // 0000 0000 0000 1111

#define DNS_QR_SHIFT     15
#define DNS_OPCODE_SHIFT 11
#define DNS_AA_SHIFT     10
#define DNS_TC_SHIFT     9
#define DNS_RD_SHIFT     8
#define DNS_RA_SHIFT     7
#define DNS_Z_SHIFT      4
#define DNS_RCODE_SHIFT  0

typedef struct dns_header {
    __uint16_t id;
    __uint16_t flags;
    __uint16_t qdcount;
    __uint16_t ancount;
    __uint16_t nscount;
    __uint16_t arcount;
} dns_header;

typedef struct dns_question {
    char      *qname;
    __uint16_t qtype;
    __uint16_t qclass;
} dns_question;

typedef struct dns_rr {
    char      *name;
    __uint16_t type;
    __uint16_t rclass;
    __uint32_t ttl;
    __uint16_t rdlength;

    void *rdata;
} dns_rr;

typedef struct {
    dns_header header;

    dns_question **questions;   // [qdcount]
    dns_rr       **answers;     // [ancount]
    dns_rr       **authorities; // [nscount]
    dns_rr       **additionals; // [arcount]
} dns_message;

__uint8_t get_flag_qr(__uint16_t flags);
__uint8_t get_flag_opcode(__uint16_t flags);
__uint8_t get_flag_aa(__uint16_t flags);
__uint8_t get_flag_tc(__uint16_t flags);
__uint8_t get_flag_rd(__uint16_t flags);
__uint8_t get_flag_ra(__uint16_t flags);
__uint8_t get_flag_z(__uint16_t flags);
__uint8_t get_flag_rcode(__uint16_t flags);

dns_question *parse_dns_question(char *buffer, size_t len);
dns_rr       *parse_dns_rr(char *buffer, size_t len);
dns_message  *parse_dns_message(char *buffer, size_t len);
// __uint8_t    *serialize_dns_message(dns_message *message); // ? we may need it later

#endif // DNS_H