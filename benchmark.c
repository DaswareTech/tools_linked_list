
#include <stdio.h>
#include "bench/bench.h"
#include "src/list.h"

static void
bm(char *label, void (*fn)()) {
  printf(" %18s", label);
  fflush(stdout);
  fn();
}

static int nnodes = 10000000;
static float startTime;


static void
start() {
  startTime = cpu();
}

static void
stop() {
  float duration = cpu() - startTime;
  printf(": \x1b[32m%.4f\x1b[0ms\n", duration);
}

static void
bm_rpush() {
  start();
  int n = nnodes;
  tools_list_t *list = tools_list_new();
  while (n--) {
    tools_list_rpush(list, tools_list_node_new("foo"));
  }
  stop();
}

static void
bm_lpush() {
  start();
  int n = nnodes;
  tools_list_t *list = tools_list_new();
  while (n--) {
    tools_list_lpush(list, tools_list_node_new("foo"));
  }
  stop();
}

static void
bm_find() {
  int n = nnodes;
  tools_list_t *list = tools_list_new();
  while (n--) {
    tools_list_lpush(list, tools_list_node_new("foo"));
  }
  tools_list_rpush(list, tools_list_node_new("bar"));
  start();
  tools_list_find(list, "bar");
  stop();
}

static void
bm_iterate() {
  int n = nnodes;
  tools_list_t *list = tools_list_new();
  while (n--) {
    tools_list_lpush(list, tools_list_node_new("foo"));
  }
  tools_list_iterator_t *it = tools_list_iterator_new(list, tools_list_HEAD);
  tools_list_node_t *node;
  start();
  while ((node = tools_list_iterator_next(it)))
    ;
  stop();
}

static void
bm_rpop() {
  int n = nnodes;
  tools_list_t *list = tools_list_new();
  while (n--) {
    tools_list_lpush(list, tools_list_node_new("foo"));
  }
  tools_list_node_t *node;
  start();
  while ((node = tools_list_rpop(list)))
    ;
  stop();
}

static void
bm_lpop() {
  int n = nnodes;
  tools_list_t *list = tools_list_new();
  while (n--) {
    tools_list_lpush(list, tools_list_node_new("foo"));
  }
  tools_list_node_t *node;
  start();
  while ((node = tools_list_lpop(list)))
    ;
  stop();
}

static tools_list_t *list;

static void
bm_at() {
  start();
  tools_list_at(list, 100000);
  stop();
}

static void
bm_at2() {
  start();
  tools_list_at(list, 1000000);
  stop();
}

static void
bm_at3() {
  start();
  tools_list_at(list, -100000);
  stop();
}

int
main(void){
  int n = nnodes;
  list = tools_list_new();
  while (n--) tools_list_lpush(list, tools_list_node_new("foo"));
  puts("\n 10,000,000 nodes\n");
  bm("lpush", bm_lpush);
  bm("rpush", bm_rpush);
  bm("lpop", bm_lpop);
  bm("rpop", bm_rpop);
  bm("find (last node)", bm_find);
  bm("iterate", bm_iterate);
  bm("at(100,000)", bm_at);
  bm("at(1,000,000)", bm_at2);
  bm("at(-100,000)", bm_at3);
  puts("");
  return 0;
}
