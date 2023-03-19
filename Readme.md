
# list

 C doubly linked list implementation.

## API

 Below is the public api currently provided by "list".

## tools_list_t *tools_list_new();

 Allocate and initialize a `list`.
 
    tools_list_t *mylist = tools_list_new();

## tools_list_node_t \*tools_list_node_new(void *val)

 Allocate and initialize a `tools_list_node_t` with the given _val_.

```c
tools_list_node_t *node = tools_list_node_new("my value");
node->val; // "my value"
```

## tools_list_node_t \* tools_list_rpush(tools_list_t \*self, tools_list_node_t *node)

 Append _node_ to _self_, returning _node_.
 
```c
tools_list_rpush(list, tools_list_node_new("value"));
list->tail->val; // "value"
```

## tools_list_node_t \* tools_list_rpop(tools_list_t \*self)

  Return / detach node from the end of the list, or __NULL__.

```c
tools_list_node_t *last = tools_list_rpop(names);
```

##  tools_list_node_t \*tools_list_lpush(tools_list_t \*self, tools_list_node_t *node)

 Prepend _node_ to _self_, returning _node_.

```c
tools_list_lpush(list, tools_list_node_new("value"));
list->head->val; // "value"
```

## tools_list_node_t \*tools_list_find(tools_list_t \*self, void *val)

 Return the `tools_list_node_t` containing _val_ or __NULL__.

```c
tools_list_node_t *node = tools_list_find(list, "some value");
```

## tools_list_node_t \*tools_list_at(tools_list_t *self, int index)

 Return the `tools_list_node_t` at the given _index_, where _index_
 may also be a negative integer indicating an index from the
 list _tail_.

```c
tools_list_at(list, 0);  // first
tools_list_at(list, 1);  // second
tools_list_at(list, -1); // last
tools_list_at(list, -3); // third last
```

## void tools_list_remove(tools_list_t \*self, tools_list_node_t *node)

  Remove _node_ from the list, freeing it and it's value.

```c
tools_list_remove(list, node);
```

## void tools_list_destroy(tools_list_t *self)

  Free the list and all nodes.

```c
tools_list_destroy(list);
```

## tools_list_iterator_t \*tools_list_iterator_new(tools_list_t *list, tools_list_direction_t direction)

  Allocate and initialize a `tools_list_iterator_t` with the given _direction_,
  where _direction_ may be __tools_list_HEAD__ or __tools_list_TAIL__.

```c
tools_list_node_t *node;
tools_list_iterator_t *it = tools_list_iterator_new(list, tools_list_HEAD);
while ((node = tools_list_iterator_next(it))) {
  puts(node->val);
}
```  

## tools_list_node_t \*tools_list_iterator_next(tools_list_iterator_t *self)

  Return the next `tools_list_node_t` or __NULL__.

## void tools_list_iterator_destroy(tools_list_iterator_t *self);

  Free the iterator only.

```c
tools_list_iterator_destroy(it);
```

## Examples

list iteration:

```c
tools_list_t *langs = tools_list_new();

tools_list_node_t *c = tools_list_rpush(langs, tools_list_node_new("c"));
tools_list_node_t *js = tools_list_rpush(langs, tools_list_node_new("js"));
tools_list_node_t *ruby = tools_list_rpush(langs, tools_list_node_new("ruby"));

tools_list_node_t *node;
tools_list_iterator_t *it = tools_list_iterator_new(langs, tools_list_HEAD);
while ((node = tools_list_iterator_next(it))) {
  puts(node->val);
}

tools_list_iterator_destroy(it);
tools_list_destroy(langs);
```

stdout:

    c
    js
    ruby

## Benchmarks

    $ make benchmark
    
     10,000,000 nodes
    
                  lpush: 0.5000s
                  rpush: 0.5000s
                   lpop: 0.0312s
                   rpop: 0.0312s
       find (last node): 0.0312s
                iterate: 0.0625s
            at(100,000): 0.0000s
          at(1,000,000): 0.0000s
           at(-100,000): 0.0000s



## Testing

    $ make test

## License 

(The MIT License)

Copyright (c) 2009-2010 TJ Holowaychuk &lt;tj@vision-media.ca&gt;

Permission is hereby granted, free of charge, to any person obtaining
a copy of this software and associated documentation files (the
'Software'), to deal in the Software without restriction, including
without limitation the rights to use, copy, modify, merge, publish,
distribute, sublicense, and/or sell copies of the Software, and to
permit persons to whom the Software is furnished to do so, subject to
the following conditions:

The above copyright notice and this permission notice shall be
included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED 'AS IS', WITHOUT WARRANTY OF ANY KIND,
EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY
CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT,
TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE
SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
