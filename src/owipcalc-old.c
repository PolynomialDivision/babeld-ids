/* Copied from OpenWrt owipcalc-Projekt" */

#include "owipcalc.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

struct cidr *cidr_parse6(const char *s) {
  char *p = NULL, *r;
  struct cidr *addr = malloc(sizeof(struct cidr));

  if (!addr || (strlen(s) >= sizeof(addr->buf.v6)))
    goto err;

  snprintf(addr->buf.v6, sizeof(addr->buf.v6), "%s", s);

  addr->family = AF_INET6;

  if ((p = strchr(addr->buf.v6, '/')) != NULL) {
    *p++ = 0;

    addr->prefix = strtoul(p, &r, 10);

    if ((p == r) || (*r != 0) || (addr->prefix > 128))
      goto err;
  } else {
    addr->prefix = 128;
  }

  if (p == addr->buf.v6 + 1)
    memset(&addr->addr.v6, 0, sizeof(addr->addr.v6));
  else if (inet_pton(AF_INET6, addr->buf.v6, &addr->addr.v6) != 1)
    goto err;

  return addr;

err:
  if (addr)
    free(addr);

  return NULL;
}

bool cidr_contains6(struct cidr *a, struct cidr *b) {
  struct in6_addr *x = &a->addr.v6;
  struct in6_addr *y = &b->addr.v6;
  uint8_t i = (128 - a->prefix) / 8;
  uint8_t m = ~((1 << ((128 - a->prefix) % 8)) - 1);
  uint8_t net1 = x->s6_addr[15 - i] & m;
  uint8_t net2 = y->s6_addr[15 - i] & m;

  if (a->prefix == 0 ||
      ((b->prefix >= a->prefix) && (net1 == net2) &&
       ((i == 15) || !memcmp(&x->s6_addr, &y->s6_addr, 15 - i)))) {
    return true;
  } else {
    return false;
  }
}