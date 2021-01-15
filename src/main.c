#include <linux/types.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>
#include <unistd.h>

#include <libubox/blobmsg.h>
#include <libubox/blobmsg_json.h>
#include <libubus.h>

#include "owipcalc.h"
#include "uci.h"

static struct ubus_context *ctx;
// static struct blob_buf b;

struct babeld_subscriber {
  uint32_t id;
  struct ubus_subscriber subscriber;
  struct ubus_event_handler wait_handler;
  bool subscribed;
};

struct babeld_subscriber babeld_sub;

static inline int subscription_wait(struct ubus_event_handler *handler) {
  return ubus_register_event_handler(ctx, handler, "ubus.object.add");
}

static void babeld_handle_remove(struct ubus_context *ctx,
                                 struct ubus_subscriber *s, uint32_t id) {
  babeld_sub.subscribed = false;
  // ToDO
  return;
}

static void wait_cb(struct ubus_context *ctx,
                    struct ubus_event_handler *ev_handler, const char *type,
                    struct blob_attr *msg) {
  // ToDo
  return;
}

static bool check_cidr(struct cidr *a) {
  struct prefix_list_entry *cur, *tmp;
  list_for_each_entry_safe(cur, tmp, &prefix_list, list) {
    if (cidr_contains6(cur->cidr, a))
      return true;
  }

  return false;
}

static int babeld_notify(struct ubus_context *ctx, struct ubus_object *obj,
                         struct ubus_request_data *req, const char *method,
                         struct blob_attr *msg) {
  /*char *str;
  str = blobmsg_format_json(msg, true);
  printf("Method new: %s : %s\n", method, str);
  free(str);*/

  struct blob_attr *attr;
  struct blobmsg_hdr *hdr;
  int len;

  len = blobmsg_data_len(msg);

  __blob_for_each_attr(attr, blobmsg_data(msg), len) {
    hdr = blob_data(attr);
    char *dst_prefix = (char *)hdr->name;

    if (strchr(dst_prefix, ':')) {
      struct cidr *b;
      b = cidr_parse6(dst_prefix);
      if (check_cidr(b))
        printf("Prefix in babel network announced that is in watchlist: "
               "Method: %s Dst-Prefix: %s\n",
               method, dst_prefix);
      // it would be awesome to have here some scripts running
      // https://github.com/br101/pingcheck/blob/master/scripts.c
      else
        printf("Not!\n");
    }
  }

  return 0;
}

static bool subscribe() {
  babeld_sub.subscriber.cb = babeld_notify;
  babeld_sub.subscriber.remove_cb = babeld_handle_remove;
  babeld_sub.wait_handler.cb = wait_cb;
  babeld_sub.subscribed = false;

  if (ubus_register_subscriber(ctx, &babeld_sub.subscriber)) {
    fprintf(stderr, "Failed to register subscriber!");
    return false;
  }

  if (ubus_lookup_id(ctx, "babeld", &babeld_sub.id)) {
    fprintf(stderr, "Failed to lookup ID!");
    subscription_wait(&babeld_sub.wait_handler);
    return false;
  }

  if (ubus_subscribe(ctx, &babeld_sub.subscriber, babeld_sub.id)) {
    fprintf(stderr, "Failed to register subscriber!");
    return false;
  }

  babeld_sub.subscribed = true;

  return true;
}

int main(int argc, char **argv) {
  const char *ubus_socket = NULL;

  char path[] = "babeld-ids.watch.prefix";
  show_config_entry(path);

  uloop_init();

  ctx = ubus_connect(ubus_socket);
  if (!ctx) {
    fprintf(stderr, "Failed to connect to ubus\n");
    return -1;
  }

  ubus_add_uloop(ctx);

  subscribe();

  uloop_run();

  ubus_free(ctx);
  uloop_done();

  return 0;
}
