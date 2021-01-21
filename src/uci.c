#include "uci.h"

#include <stdlib.h>
#include <string.h>

LIST_HEAD(prefix_list);

int show_config_entry(char *path) {
  struct uci_context *c;
  struct uci_ptr ptr;
  struct uci_element *e;

  c = uci_alloc_context();
  if (uci_lookup_ptr(c, &ptr, path, true) != UCI_OK) {
    uci_perror(c, "get_config_entry Error");
    return 1;
  }
  if (ptr.o->type != UCI_TYPE_LIST) {
    return 1;
  }

  uci_foreach_element(&ptr.o->v.list, e) {
    struct prefix_list_entry *prefix =
        calloc(1, sizeof(struct prefix_list_entry));
    printf("%s\n", e->name);
    prefix->cidr = cidr_parse6(e->name);
    list_add(&prefix->list, &prefix_list);
  }

  uci_free_context(c);
  return 0;
}
