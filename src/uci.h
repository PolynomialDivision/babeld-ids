#include <string.h>
#include <uci.h>

#include <libubox/list.h>
#include <owipcalc.h>

struct prefix_list_entry {
  struct list_head list;
  struct cidr *cidr;
};

extern struct list_head prefix_list;

int show_config_entry(char *path);
