
//
// node.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocates a new tools_list_node_t. NULL on failure.
 */

tools_list_node_t *
tools_list_node_new(void *val) {
  tools_list_node_t *self;
  if (!(self = tools_list_MALLOC(sizeof(tools_list_node_t))))
    return NULL;
  self->prev = NULL;
  self->next = NULL;
  self->val = val;
  return self;
}