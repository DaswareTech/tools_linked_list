
//
// list.h
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#ifndef __CLIBS_tools_list_H__
#define __CLIBS_tools_list_H__

#ifdef __cplusplus
extern "C" {
#endif

#include <stdlib.h>

// Library version

#define tools_list_VERSION "0.2.0"

// Memory management macros
#ifdef tools_list_CONFIG_H
#define _STR(x) #x
#define STR(x) _STR(x)
#include STR(tools_list_CONFIG_H)
#undef _STR
#undef STR
#endif

#ifndef tools_list_MALLOC
#define tools_list_MALLOC malloc
#endif

#ifndef tools_list_FREE
#define tools_list_FREE list_free_func
#endif

/*
 * tools_list_t iterator direction.
 */

typedef enum {
    TOOLS_LIST_HEAD
  , TOOLS_LIST_TAIL
} tools_list_direction_t;

/*
 * tools_list_t node struct.
 */

typedef struct tools_list_node {
  struct tools_list_node *prev;
  struct tools_list_node *next;
  void *val;
} tools_list_node_t;

/*
 * tools_list_t struct.
 */

typedef struct {
  tools_list_node_t *head;
  tools_list_node_t *tail;
  unsigned int len;
  void (*free)(void *val);
  int (*match)(void *a, void *b);
} tools_list_t;

/*
 * tools_list_t iterator struct.
 */

typedef struct {
  tools_list_node_t *next;
  tools_list_direction_t direction;
} tools_list_iterator_t;

// Node prototypes.

tools_list_node_t *
tools_list_node_new(void *val);

// tools_list_t prototypes.

tools_list_t *
tools_list_new(void);

tools_list_node_t *
tools_list_rpush(tools_list_t *self, tools_list_node_t *node);

tools_list_node_t *
tools_list_lpush(tools_list_t *self, tools_list_node_t *node);

tools_list_node_t *
tools_list_find(tools_list_t *self, void *val);

tools_list_node_t *
tools_list_at(tools_list_t *self, int index);

tools_list_node_t *
tools_list_rpop(tools_list_t *self);

tools_list_node_t *
tools_list_lpop(tools_list_t *self);

void
tools_list_remove(tools_list_t *self, tools_list_node_t *node);
void
tools_list_remove_with_opts(tools_list_t *self, tools_list_node_t *node, bool free_val);

void
tools_list_destroy(tools_list_t *self);

// tools_list_t iterator prototypes.

tools_list_iterator_t *
tools_list_iterator_new(tools_list_t *list, tools_list_direction_t direction);

tools_list_iterator_t *
tools_list_iterator_new_from_node(tools_list_node_t *node, tools_list_direction_t direction);

tools_list_node_t *
tools_list_iterator_next(tools_list_iterator_t *self);

void
tools_list_iterator_destroy(tools_list_iterator_t *self);

// Free
void list_free_func(void* ptr);

#ifdef __cplusplus
}
#endif

#endif /* __CLIBS_tools_list_H__ */
