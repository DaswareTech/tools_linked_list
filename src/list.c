
//
// list.c
//
// Copyright (c) 2010 TJ Holowaychuk <tj@vision-media.ca>
//

#include "list.h"

/*
 * Allocate a new tools_list_t. NULL on failure.
 */

tools_list_t *
tools_list_new(void) {
  tools_list_t *self;
  if (!(self = tools_list_MALLOC(sizeof(tools_list_t))))
    return NULL;
  self->head = NULL;
  self->tail = NULL;
  self->free = NULL;
  self->match = NULL;
  self->len = 0;
  return self;
}

/*
 * Free the list.
 * @self: Pointer to the list 
 */

void
tools_list_destroy(tools_list_t *self) {
  unsigned int len = self->len;
  tools_list_node_t *next;
  tools_list_node_t *curr = self->head;

  while (len--) {
    next = curr->next;
    if (self->free) self->free(curr->val);
    tools_list_FREE(curr);
    curr = next;
  }

  tools_list_FREE(self);
}

/*
 * Append the given node to the list
 * and return the node, NULL on failure.
 * @self: Pointer to the list for popping node
 * @node: the node to push
 */

tools_list_node_t *
tools_list_rpush(tools_list_t *self, tools_list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->prev = self->tail;
    node->next = NULL;
    self->tail->next = node;
    self->tail = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return / detach the last node in the list, or NULL.
 * @self: Pointer to the list for popping node
 */

tools_list_node_t *
tools_list_rpop(tools_list_t *self) {
  if (!self->len) return NULL;

  tools_list_node_t *node = self->tail;

  if (--self->len) {
    (self->tail = node->prev)->next = NULL;
  } else {
    self->tail = self->head = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Return / detach the first node in the list, or NULL.
 * @self: Pointer to the list for popping node
 */

tools_list_node_t *
tools_list_lpop(tools_list_t *self) {
  if (!self->len) return NULL;

  tools_list_node_t *node = self->head;

  if (--self->len) {
    (self->head = node->next)->prev = NULL;
  } else {
    self->head = self->tail = NULL;
  }

  node->next = node->prev = NULL;
  return node;
}

/*
 * Prepend the given node to the list
 * and return the node, NULL on failure.
 * @self: Pointer to the list for pushing node
 * @node: the node to push
 */

tools_list_node_t *
tools_list_lpush(tools_list_t *self, tools_list_node_t *node) {
  if (!node) return NULL;

  if (self->len) {
    node->next = self->head;
    node->prev = NULL;
    self->head->prev = node;
    self->head = node;
  } else {
    self->head = self->tail = node;
    node->prev = node->next = NULL;
  }

  ++self->len;
  return node;
}

/*
 * Return the node associated to val or NULL.
 * @self: Pointer to the list for finding given value
 * @val: Value to find 
 */

tools_list_node_t *
tools_list_find(tools_list_t *self, void *val) {
  tools_list_iterator_t *it = tools_list_iterator_new(self, TOOLS_LIST_HEAD);
  tools_list_node_t *node;

  while ((node = tools_list_iterator_next(it))) {
    if (self->match) {
      if (self->match(val, node->val)) {
        tools_list_iterator_destroy(it);
        return node;
      }
    } else {
      if (val == node->val) {
        tools_list_iterator_destroy(it);
        return node;
      }
    }
  }

  tools_list_iterator_destroy(it);
  return NULL;
}

/*
 * Return the node at the given index or NULL.
 * @self: Pointer to the list for finding given index 
 * @index: the index of node in the list
 */

tools_list_node_t *
tools_list_at(tools_list_t *self, int index) {
  tools_list_direction_t direction = TOOLS_LIST_HEAD;

  if (index < 0) {
    direction = TOOLS_LIST_TAIL;
    index = ~index;
  }

  if ((unsigned)index < self->len) {
    tools_list_iterator_t *it = tools_list_iterator_new(self, direction);
    tools_list_node_t *node = tools_list_iterator_next(it);
    while (index--) node = tools_list_iterator_next(it);
    tools_list_iterator_destroy(it);
    return node;
  }

  return NULL;
}

/*
 * Remove the given node from the list, freeing it and it's value.
 * @self: Pointer to the list to delete a node 
 * @node: Pointer the node to be deleted
 */

void
tools_list_remove(tools_list_t *self, tools_list_node_t *node) {
  node->prev
    ? (node->prev->next = node->next)
    : (self->head = node->next);

  node->next
    ? (node->next->prev = node->prev)
    : (self->tail = node->prev);

  if (self->free) self->free(node->val);

  tools_list_FREE(node);
  --self->len;
}
