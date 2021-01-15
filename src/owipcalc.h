/* Copied from OpenWrt owipcalc-Projekt" */

#include <stdbool.h>

#include <arpa/inet.h>
#include <linux/types.h>

#include <libubox/list.h>

struct prefix_list_entry {
  struct list_head list;
  struct cidr *cidr;
};

struct cidr {
  uint8_t family;
  uint32_t prefix;
  union {
    struct in_addr v4;
    struct in6_addr v6;
  } addr;
  union {
    char v4[sizeof("255.255.255.255/255.255.255.255 ")];
    char v6[sizeof("FFFF:FFFF:FFFF:FFFF:FFFF:FFFF:255.255.255.255/128 ")];
  } buf;
  struct cidr *next;
};

struct cidr *cidr_parse6(const char *s);

bool cidr_contains6(struct cidr *a, struct cidr *b);
