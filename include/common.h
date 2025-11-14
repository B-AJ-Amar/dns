#ifndef COMMON_H
#define COMMON_H

#define RET_FAILURE 0
#define RET_SUCCESS 1

#define FALSE 0
#define TRUE  1

const enum RECORD_TYPE {
    A       = 1,  //? 1   a host address
    NS      = 2,  //? 2   an authoritative name server
    MD      = 3,  //? 3   a mail destination (Obsolete - use MX)
    MF      = 4,  //? 4   a mail forwarder (Obsolete - use MX)
    CNAME   = 5,  //? 5   the canonical name for an alias
    SOA     = 6,  //? 6   marks the start of a zone of authority
    MB      = 7,  //? 7   a mailbox domain name (EXPERIMENTAL)
    MG      = 8,  //? 8   a mail group member (EXPERIMENTAL)
    MR      = 9,  //? 9   a mail rename domain name (EXPERIMENTAL)
    NULL_RR = 10, //? 10  a null RR (EXPERIMENTAL)
    WKS     = 11, //? 11  a well known service description
    PTR     = 12, //? 12  a domain name pointer
    HINFO   = 13, //? 13  host information
    MINFO   = 14, //? 14  mailbox or mail list information
    MX      = 15, //? 15  mail exchange
    TXT     = 16, //? 16  text strings
    AAAA    = 28  //? 28  an IPv6 host address
};

const enum CLASS_TYPE {
    IN = 1, //? (mainly we will use this)
    CS = 2,
    CH = 3,
    HS = 4
};

#endif // COMMON_H