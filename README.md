# Linked List

## About

Singly-linked list, simply called linked list, is a data structure composed of individual elements, each linked by a single pointer.

## API

### List_init

```C
void List_init(List* list, void (*destroy)(void* data), int (*match)(const void* key_1, const void* key_2))
```

Initializes the linked list specified by `list`. The function must be called for a list before the list can be used with any other operation. The `destroy` argument provides a way to free dynamically allocated data. For data that should not be freed `destroy` should be set to *NULL*. The `match` argument provides a **default** way to compare list data. [How to write `match` functions](#how-to-write-match-functions).

**Return value**: none.

### List_create

```C
List* List_create(void)
```

Dynamically allocates a list. After the successful function invocation, the return value must be passed to the `List_init`.

**Return value**: returns the list that was allocated or *NULL* on failure. Prints the error message in `stderr`.

### List_destroy

```C
void List_destroy(List** list);
```

The `List_destroy` function should be called on dynamically allocated lists. It deallocates the list specified by `list` and frees data in the list nodes if `destroy` method was provided. The `list` is set to *NULL* to prevent it from being used.

**Return value**: none.

### List_clear

```C
void List_clear(List* list);
```

The `List_clear` function clears the list specified by `list`. It deletes the list nodes and node data if `destroy` method was provided. The function doesn't *NULL* the `match` and `destroy` methods. You can reinitialize the list after calling `List_clear` function.

**Return value**: none.

### List_print

```C
void List_print(const List* list, void (*print)(const void* data), char* sep);
```

The `List_print` routine displays the list content specified by `list`. The `print` argument is a function to handle data within list nodes. See example on [how to write `print` functions](#how-to-write-print-functions). The `sep` argument is a delimiter that used to separate data in the list.

### List_insert_first

```C
int List_insert_first(List* list, const void* data);
```

The `List_insert_first` function inserts data at the beggining/head of the list specified by `list`. `data` is the pointer to data. The function dynamically allocates a node for data. If `data` is *NULL*, the function will still allocate a node and increase the list size. Any attempt to print such lists (with nodes that hold *NULL*) crashes your program. To prevent it from doing so, you need to let `print` function to handle *NULL*, for example:

```C
void print_data(const void* data) {
    if (data == NULL) {
        return ;
    }

    /* print data */
}
```

**Return value**: returns `EXIT_SUCCESS` on success or `EXIT_FAILURE` on failure.

### List_insert_last

```C
int List_insert_last(List* list, const void* data);
```

Similar to `List_insert_first` but inserts data at the end/tail of the list specified by `list`.

**Return value**: returns `EXIT_SUCCESS` on success or `EXIT_FAILURE` on failure.

### List_remove_first

```C
void* List_remove_first(List* list);
```

The `List_remove_first` function removes an element from the head of the list specified by `list`. The function returns a pointer to data contained in the node. The node itself is freed. It is the responsibility of the caller to manage the storage associated with the data.

**Return value**: a pointer to data that was stored in a node

### List_remove_last

```C
void* List_remove_last(List* list);
```

The `List_remove_first` function removes an element from the tail of the list specified by `list`. The function returns a pointer to data contained in the node. The node itself is freed. It is the responsibility of the caller to manage the storage associated with the data.

**Return value**: a pointer to data that was stored in a node

### List_find

```C
Node* List_find(const List* list, const void* data, int (*match)(const void* key_1, const void* key_2))
```

The `List_find` function traverses the list specified by `list` to find data `data`. `data` contains a pointer to data you want to find. [`match`](#how-to-write-match-functions) function is a callback that compares values in the list with specified data.

**Return value**: the function returns *NULL* if the list doesn't have a node with specified data `data`. On success it return a pointer to that node

### List_insert_after

```C
int List_insert_after(List* list, const Node* node, const void* data);
```

The `List_insert_after` function inserts a new node with data `data` in the list specified by `list`. If node `node` is *NULL*, the function does nothing. The `List_find` function can be passed to this one to find a node with specified data first.

**Return value**: on success it returns `EXIT_SUCCESS`, `EXIT_FAILURE` otherwise

### List_insert_before

```C
int List_insert_before(List* list, const Node* node, const void* data);
```

The `List_insert_before` function does the same as `List_insert_after` but inserts a node before specified one.

**Return value**: on success it returns `EXIT_SUCCESS`, `EXIT_FAILURE` otherwise

### List_load_array

```C
int List_load_array(List* list, ssize_t size, ssize_t elm_size, void* array);
```

The `List_load_array` function adds element from an array `array` at the tail of the list specified by `list`.
`size` is the number of elements in the array. `elm_size` is the size of data type stored in the array. In order to prevent automatic conversion of negative values for `size` and `elm_size`, they are defined as `ssize_t`.

****Return value**: on success it returns `EXIT_SUCCESS`, `EXIT_FAILURE` otherwise

## Examples

### How to write `match` functions

A `match` function is a callback that is passed as an argument to another function. In order to write a *match* function, one must assume that a list is *homogeneous* (i.e., contains data of the same type).

```C
int match(const void* key_1, const void* key_2) {
    return (*((int*) key_1) - *((int*) key_2));
}
```

The `match` function takes two parameters: `key_1` and `key_2`. The `key_1` parameter takes data of the list node, and `key_2` is data to compare with. Keep in mind that a *match* function accepts `void*` pointers, and a type casting takes place inside the function body.

In the example above, I assume a list contains `int`s as its data. In the function body I cast pointers to `int*` and then dereference them to take there values. In that particular example, the function returns *0* if `key_1` and `key_2` are the same.

**Overflow may occur!**

### How to write `print` functions

A `print` function is a callback one that's invoked when the `List_print` function is called. Its only argument is a `void*` pointer to data that resides in a list node. In the function body, one must cast and dereference a pointer to appropriate type to display it. Let's consider an example:

```C
void print_data(const void* data) {
    /* Assume the list we want to display contains strings */
    printf("%s", *((char*) data));

    return ;
}

List list;

/* ... */

List_print(&list, print_data, " "); /* Use match from example above and a space character as a separator */
```
