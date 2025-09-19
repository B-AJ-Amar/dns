cmake_minimum_required(VERSION 3.10)
project(dns_project C)
set(CMAKE_C_STANDARD 11)

include_directories(include)

add_library(dns_common
    src/dns.c
    src/net.c
    src/zone.c
    src/logger.c
)

add_executable(dns-resolver
    src/recursive/main.c
    src/recursive/resolver.c
)
target_link_libraries(dns-resolver dns_common)

add_executable(dns-authoritative
    src/authoritative/main.c
    src/authoritative/authoritative.c
)
target_link_libraries(dns-authoritative dns_common)

add_executable(dns-root-tld
    src/root_tld/main.c
    src/root_tld/root_tld.c
)
target_link_libraries(dns-root-tld dns_common)
