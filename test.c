
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include "src/list.h"

// Helpers

#define test(fn) \
  puts("... \x1b[33m" # fn "\x1b[0m"); \
  test_##fn(); //Call the test function

// For counting destroyed time
static int freeProxyCalls = 0;

void
freeProxy(void *val) {
  ++freeProxyCalls;
  free(val);
}

typedef struct {
  char *name;
} User;

static int
User_equal(User *a, User *b) {
  return 0 == strcmp(a->name, b->name);
}

// Tests

static void
test_tools_list_node_new() {
  char *val = "some value";
  tools_list_node_t *node = tools_list_node_new(val);
  assert(node->val == val);
  free(node);
}

static void
test_tools_list_rpush() {
  // Setup
  tools_list_t *list = tools_list_new();
  tools_list_node_t *a = tools_list_node_new("a");
  tools_list_node_t *b = tools_list_node_new("b");
  tools_list_node_t *c = tools_list_node_new("c");

  // a b c
  tools_list_rpush(list, a);
  tools_list_rpush(list, b);
  tools_list_rpush(list, c);

  // Assertions
  assert(a == list->head);
  assert(c == list->tail);
  assert(3 == list->len);
  assert(b == a->next);
  assert(NULL == a->prev);
  assert(c == b->next);
  assert(a == b->prev);
  assert(NULL == c->next);
  assert(b == c->prev);

  tools_list_destroy(list);
}

static void
test_tools_list_lpush() {
  // Setup
  tools_list_t *list = tools_list_new();
  tools_list_node_t *a = tools_list_node_new("a");
  tools_list_node_t *b = tools_list_node_new("b");
  tools_list_node_t *c = tools_list_node_new("c");

  // c b a
  tools_list_rpush(list, a);
  tools_list_lpush(list, b);
  tools_list_lpush(list, c);

  // Assertions
  assert(c == list->head);
  assert(a == list->tail);
  assert(3 == list->len);
  assert(NULL == a->next);
  assert(b == a->prev);
  assert(a == b->next);
  assert(c == b->prev);
  assert(b == c->next);
  assert(NULL == c->prev);

  tools_list_destroy(list);
}

static void
test_tools_list_at() {
  // Setup
  tools_list_t *list = tools_list_new();
  tools_list_node_t *a = tools_list_node_new("a");
  tools_list_node_t *b = tools_list_node_new("b");
  tools_list_node_t *c = tools_list_node_new("c");

  // a b c
  tools_list_rpush(list, a);
  tools_list_rpush(list, b);
  tools_list_rpush(list, c);

  // Assertions
  assert(a == tools_list_at(list, 0));
  assert(b == tools_list_at(list, 1));
  assert(c == tools_list_at(list, 2));
  assert(NULL == tools_list_at(list, 3));

  assert(c == tools_list_at(list, -1));
  assert(b == tools_list_at(list, -2));
  assert(a == tools_list_at(list, -3));
  assert(NULL == tools_list_at(list, -4));

  tools_list_destroy(list);
}

static void
test_tools_list_destroy() {
  // Setup
  tools_list_t *a = tools_list_new();
  tools_list_destroy(a);

  // a b c
  tools_list_t *b = tools_list_new();
  tools_list_rpush(b, tools_list_node_new("a"));
  tools_list_rpush(b, tools_list_node_new("b"));
  tools_list_rpush(b, tools_list_node_new("c"));
  tools_list_destroy(b);
  
  // Assertions
  tools_list_t *c = tools_list_new();
  c->free = freeProxy;
  tools_list_rpush(c, tools_list_node_new(tools_list_node_new("a")));
  tools_list_rpush(c, tools_list_node_new(tools_list_node_new("b")));
  tools_list_rpush(c, tools_list_node_new(tools_list_node_new("c")));
  tools_list_destroy(c);
  assert(3 == freeProxyCalls);
  freeProxyCalls=0;
}

static void
test_tools_list_destroy_complexver() {
  // Setup
  tools_list_t *a = tools_list_new();
  tools_list_destroy(a);

  // a b c
  tools_list_t *b = tools_list_new();
  tools_list_rpush(b, tools_list_node_new("a"));
  tools_list_rpush(b, tools_list_node_new("b"));
  tools_list_rpush(b, tools_list_node_new("c"));
  tools_list_destroy(b);

  // Assertions
  tools_list_t *c = tools_list_new();
  c->free = freeProxy;
  tools_list_rpush(c, tools_list_node_new(tools_list_node_new("a")));
  tools_list_rpush(c, tools_list_node_new(tools_list_node_new("b")));
  tools_list_rpush(c, tools_list_node_new(tools_list_node_new("c")));
  tools_list_destroy(c);
  assert(3 == freeProxyCalls);
  freeProxyCalls=0;
  tools_list_t *d = tools_list_new();
  d->free = freeProxy;
  tools_list_rpush(d, tools_list_node_new(tools_list_node_new("a")));
  tools_list_rpush(d, tools_list_node_new(tools_list_node_new("b")));
  tools_list_rpush(d, tools_list_node_new(tools_list_node_new("c")));
  tools_list_destroy(d);
  assert(3 == freeProxyCalls);
  freeProxyCalls=0;
}
static void
test_tools_list_empty_tools_list_destroy() {
  tools_list_t *list = tools_list_new();
  tools_list_destroy(list);
  freeProxyCalls=0;
}
static void
test_tools_list_find() {
  // Setup
  tools_list_t *langs = tools_list_new();
  tools_list_node_t *js = tools_list_rpush(langs, tools_list_node_new("js"));
  tools_list_node_t *ruby = tools_list_rpush(langs, tools_list_node_new("ruby"));

  tools_list_t *users = tools_list_new();
  users->match = User_equal;
  User userTJ = { "tj" };
  User userSimon = { "simon" };
  User userTaylor = { "taylor" };
  tools_list_node_t *tj = tools_list_rpush(users, tools_list_node_new(&userTJ));
  tools_list_node_t *simon = tools_list_rpush(users, tools_list_node_new(&userSimon));

  // Assertions
  tools_list_node_t *a = tools_list_find(langs, "js");
  tools_list_node_t *b = tools_list_find(langs, "ruby");
  tools_list_node_t *c = tools_list_find(langs, "foo");
  assert(js == a);
  assert(ruby == b);
  assert(NULL == c);

  tools_list_destroy(langs);

  a = tools_list_find(users, &userTJ);
  b = tools_list_find(users, &userSimon);
  c = tools_list_find(users, &userTaylor);
  assert(tj == a);
  assert(simon == b);
  assert(NULL == c);

  tools_list_destroy(users);
}

static void
test_tools_list_remove() {
  // Setup
  tools_list_t *list = tools_list_new();
  tools_list_node_t *a = tools_list_rpush(list, tools_list_node_new("a"));
  tools_list_node_t *b = tools_list_rpush(list, tools_list_node_new("b"));
  tools_list_node_t *c = tools_list_rpush(list, tools_list_node_new("c"));

  // Assertions
  assert(3 == list->len);

  tools_list_remove(list, b);
  assert(2 == list->len);
  assert(a == list->head);
  assert(c == list->tail);
  assert(c == a->next);
  assert(NULL == a->prev);
  assert(NULL == c->next);
  assert(a == c->prev);

  tools_list_remove(list, a);
  assert(1 == list->len);
  assert(c == list->head);
  assert(c == list->tail);
  assert(NULL == c->next);
  assert(NULL == c->prev);

  tools_list_remove(list, c);
  assert(0 == list->len);
  assert(NULL == list->head);
  assert(NULL == list->tail);

  tools_list_destroy(list);
}

static void
test_tools_list_rpop() {
  // Setup
  tools_list_t *list = tools_list_new();
  tools_list_node_t *a = tools_list_rpush(list, tools_list_node_new("a"));
  tools_list_node_t *b = tools_list_rpush(list, tools_list_node_new("b"));
  tools_list_node_t *c = tools_list_rpush(list, tools_list_node_new("c"));

  // Assertions
  assert(3 == list->len);

  assert(c == tools_list_rpop(list));
  assert(2 == list->len);
  assert(a == list->head);
  assert(b == list->tail);
  assert(a == b->prev);
  assert(NULL == list->tail->next && "new tail node next is not NULL");
  assert(NULL == c->prev && "detached node prev is not NULL");
  assert(NULL == c->next && "detached node next is not NULL");

  free(c);

  assert(b == tools_list_rpop(list));
  assert(1 == list->len);
  assert(a == list->head);
  assert(a == list->tail);

  free(b);

  assert(a == tools_list_rpop(list));
  assert(0 == list->len);
  assert(NULL == list->head);
  assert(NULL == list->tail);

  free(a);

  assert(NULL == tools_list_rpop(list));
  assert(0 == list->len);

  tools_list_destroy(list);
}

static void
test_tools_list_lpop() {
  // Setup
  tools_list_t *list = tools_list_new();
  tools_list_node_t *a = tools_list_rpush(list, tools_list_node_new("a"));
  tools_list_node_t *b = tools_list_rpush(list, tools_list_node_new("b"));
  tools_list_node_t *c = tools_list_rpush(list, tools_list_node_new("c"));

  // Assertions
  assert(3 == list->len);

  assert(a == tools_list_lpop(list));
  assert(2 == list->len);
  assert(b == list->head);
  assert(NULL == list->head->prev && "new head node prev is not NULL");
  assert(NULL == a->prev && "detached node prev is not NULL");
  assert(NULL == a->next && "detached node next is not NULL");

  free(a);

  assert(b == tools_list_lpop(list));
  assert(1 == list->len);

  free(b);

  assert(c == tools_list_lpop(list));
  assert(0 == list->len);
  assert(NULL == list->head);
  assert(NULL == list->tail);

  free(c);

  assert(NULL == tools_list_lpop(list));
  assert(0 == list->len);

  tools_list_destroy(list);
}

static void
test_tools_list_iterator_t() {
  // Setup
  tools_list_t *list = tools_list_new();
  tools_list_node_t *tj = tools_list_node_new("tj");
  tools_list_node_t *taylor = tools_list_node_new("taylor");
  tools_list_node_t *simon = tools_list_node_new("simon");

  // tj taylor simon
  tools_list_rpush(list, tj);
  tools_list_rpush(list, taylor);
  tools_list_rpush(list, simon);

  // Assertions

  // From head
  tools_list_iterator_t *it = tools_list_iterator_new(list, tools_list_HEAD);
  tools_list_node_t *a = tools_list_iterator_next(it);
  tools_list_node_t *b = tools_list_iterator_next(it);
  tools_list_node_t *c = tools_list_iterator_next(it);
  tools_list_node_t *d = tools_list_iterator_next(it);

  assert(a == tj);
  assert(b == taylor);
  assert(c == simon);
  assert(NULL == d);

  tools_list_iterator_destroy(it);

  // From tail
  it = tools_list_iterator_new(list, tools_list_TAIL);
  tools_list_node_t *a2 = tools_list_iterator_next(it);
  tools_list_node_t *b2 = tools_list_iterator_next(it);
  tools_list_node_t *c2 = tools_list_iterator_next(it);
  tools_list_node_t *d2 = tools_list_iterator_next(it);

  assert(a2 == simon);
  assert(b2 == taylor);
  assert(c2 == tj);
  assert(NULL == d2);
  tools_list_iterator_destroy(it);

  tools_list_destroy(list);
}

int
main(void){
  printf("\ntools_list_t: %ld\n", sizeof(tools_list_t));
  printf("tools_list_node_t: %ld\n", sizeof(tools_list_node_t));
  printf("tools_list_iterator_t: %ld\n\n", sizeof(tools_list_iterator_t));
  test(tools_list_node_new);
  test(tools_list_rpush);
  test(tools_list_lpush);
  test(tools_list_find);
  test(tools_list_at);
  test(tools_list_remove);
  test(tools_list_rpop);
  test(tools_list_lpop);
  test(tools_list_destroy);
  test(tools_list_empty_tools_list_destroy)
  test(tools_list_destroy_complexver)
  test(tools_list_iterator_t);
  puts("... \x1b[32m100%\x1b[0m\n");
  return 0;
}
