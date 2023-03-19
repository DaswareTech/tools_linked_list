
//
// iterator.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocate a new tools_list_iterator_t. NULL on failure.
 * Accepts a direction, which may be tools_list_HEAD or tools_list_TAIL.
 */

tools_list_iterator_t *
tools_list_iterator_new(tools_list_t *list, tools_list_direction_t direction) {
  tools_list_node_t *node = direction == TOOLS_LIST_HEAD
    ? list->head
    : list->tail;
  return tools_list_iterator_new_from_node(node, direction);
}

/*
 * Allocate a new tools_list_iterator_t with the given start
 * node. NULL on failure.
 */

tools_list_iterator_t *
tools_list_iterator_new_from_node(tools_list_node_t *node, tools_list_direction_t direction) {
  tools_list_iterator_t *self;
  if (!(self = tools_list_MALLOC(sizeof(tools_list_iterator_t))))
    return NULL;
  self->next = node;
  self->direction = direction;
  return self;
}

/*
 * Return the next tools_list_node_t or NULL when no more
 * nodes remain in the list.
 */

tools_list_node_t *
tools_list_iterator_next(tools_list_iterator_t *self) {
  tools_list_node_t *curr = self->next;
  if (curr) {
    self->next = self->direction == TOOLS_LIST_HEAD
      ? curr->next
      : curr->prev;
  }
  return curr;
}

/*
 * Free the list iterator.
 */

void
tools_list_iterator_destroy(tools_list_iterator_t *self) {
  tools_list_FREE(self);
  self = NULL;
}
