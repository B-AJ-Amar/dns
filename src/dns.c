#include <stdio.h>
#include <stdlib.h>
#include "dns.h"
#include "logger.h"


__uint8_t get_flag_qr(__uint16_t flags){
    return (flags & DNS_QR_MASK) >> DNS_QR_SHIFT;
}
__uint8_t get_flag_opcode(__uint16_t flags){
    return (flags & DNS_OPCODE_MASK) >> DNS_OPCODE_SHIFT;
}
__uint8_t get_flag_aa(__uint16_t flags){
    return (flags & DNS_AA_MASK) >> DNS_AA_SHIFT;
}
__uint8_t get_flag_tc(__uint16_t flags){
    return (flags & DNS_TC_MASK) >> DNS_TC_SHIFT;
}
__uint8_t get_flag_rd(__uint16_t flags){
    return (flags & DNS_RD_MASK) >> DNS_RD_SHIFT;
}
__uint8_t get_flag_ra(__uint16_t flags){
    return (flags & DNS_RA_MASK) >> DNS_RA_SHIFT;
}
__uint8_t get_flag_z(__uint16_t flags){
    return (flags & DNS_Z_MASK) >> DNS_Z_SHIFT;
}
__uint8_t get_flag_rcode(__uint16_t flags){
    return (flags & DNS_RCODE_MASK) >> DNS_RCODE_SHIFT;
}

dns_question* parse_dns_question(char* buffer,size_t len){
    dns_question question;
    unsigned long cursor = 0;

    question.qname = buffer + cursor;
    while (cursor < len && buffer[cursor] != '\0')  cursor++;
    
    if (cursor < len) {
        cursor++;
    } else {
        fprintf(stderr, "Buffer too small to contain qname\n");
    }

    if (len - cursor < 2) {
        fprintf(stderr, "Buffer too small to contain qtype\n");
    }
    question.qtype = (ntohs(*( (__uint16_t *)(buffer + cursor) )));
    question.qclass = (ntohs(*( (__uint16_t *)(buffer + cursor + 2) )));
   

    return &question;
}

dns_rr* parse_dns_rr(char* buffer,size_t len){
    dns_rr question;
    unsigned long cursor = 0;

    question.name = buffer + cursor;
    while (cursor < len && buffer[cursor] != '\0')  cursor++;
    
    if (cursor < len) {
        cursor++;
    } else {
        fprintf(stderr, "Buffer too small to contain qname\n");
    }

    if (len - cursor < 2) {
        fprintf(stderr, "Buffer too small to contain qtype\n");
    }
    question.type = (ntohs(*( (__uint16_t *)(buffer + cursor) )));
    question.rclass = (ntohs(*( (__uint16_t *)(buffer + cursor + 2) )));
    question.rclass = (ntohs(*( (__uint16_t *)(buffer + cursor + 4) )));
    question.ttl = (ntohs(*( (__uint32_t *)(buffer + cursor + 8) )));
    question.ttl = (ntohs(*( (__uint16_t *)(buffer + cursor + 12) )));
   

    return &question;
}


static dns_rr** parse_rr_section(char* buffer, size_t len, unsigned long* cursor, __uint16_t count) {
    if (count == 0) return NULL;
    
    dns_rr** records = malloc(count * sizeof(dns_rr*));
    if (!records) {
        log_fatal("failed to allocate memory for resource records\n");
    }
    
    for (int i = 0; i < count; i++) {
        records[i] = parse_dns_rr(buffer + *cursor, len - *cursor);
        *cursor += sizeof(dns_rr);
    }
    
    return records;
}

static dns_question** parse_question_section(char* buffer, size_t len, unsigned long* cursor, __uint16_t count) {
    if (count == 0) return NULL;
    
    dns_question** questions = malloc(count * sizeof(dns_question*));
    if (!questions) {
        log_fatal("failed to allocate memory for questions\n");
    }
    
    for (int i = 0; i < count; i++) {
        questions[i] = parse_dns_question(buffer + *cursor, len - *cursor);
        *cursor += sizeof(dns_question);
    }
    
    return questions;
}

dns_message* parse_dns_message(char* buffer, size_t len) {
    unsigned long cursor = sizeof(dns_header);
    if (len < cursor) {
        log_fatal("bufferoverflow : DNS header\n");
    }

    dns_message* message = calloc(1, sizeof(dns_message));
    if (!message) {
        log_fatal("failed to allocate memory for DNS message\n");
    }

    memcpy(&message->header, buffer, sizeof(dns_header));

    // ? here we need to convert the fields that are more than 1 byte from big to little endian
    message->header.id      = ntohs(message->header.id);
    message->header.flags   = ntohs(message->header.flags);
    message->header.qdcount = ntohs(message->header.qdcount);
    message->header.ancount = ntohs(message->header.ancount);
    message->header.nscount = ntohs(message->header.nscount);
    message->header.arcount = ntohs(message->header.arcount);

    message->questions   = parse_question_section(buffer, len, &cursor, message->header.qdcount);
    message->answers     = parse_rr_section(buffer, len, &cursor, message->header.ancount);
    message->authorities = parse_rr_section(buffer, len, &cursor, message->header.nscount);
    message->additionals = parse_rr_section(buffer, len, &cursor, message->header.arcount);
   
    return message;
}
