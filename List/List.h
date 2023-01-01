#ifndef LIST_H
#define LIST_H

#include <string.h>
#include <errno.h>
#include <stdarg.h>

#include "../Node/Node.h"

/* ================================================================ */
/* ================================================================ */
/*
      malloc             malloc
       head               tail
    ----------         ----------
    |  next  | ------> |  next  | ------> NULL
    |        |         |        |
    |  data  | ---     |  data  | ---
    ----------   |      ----------  |
                 V                  V
                Node               Node
              --------           --------
              |      |           |      |
              | data |           | data |
              |      |           |      |
              --------           --------   
               malloc             malloc
*/
/* ================================================================ */
/* ================================================================ */

typedef struct List {
    struct Node* head;
    struct Node* tail;

    size_t size;

    void (*destroy)(void* data);

    /* Default [match] method */
    int (*match)(const void* key_1, const void* key_2);
} List;

/* ================================================================ */
/* ================================================================ */
/* ================================================================ */

/*
    * Dynamically allocates memory for a linked list. The return value should be initialized by calling
    * the List_init() method.
*/
extern List* List_create(void);

/* ================================================================ */

/*
    * Initializes the linked list specified by "list". This operation must be called 
    * for a linked list before the list can be used with any other operation.
*/
extern void List_init(List* list, void (*destroy)(void* data), int (*match)(const void* key_1, const void* key_2));

/* ================================================================ */

/*
    * Deallocates memory for the dynamically allocated list. If destroy argument is specified,
    * the function deallocates node's data too.
*/
extern void List_destroy(List** list);

/* ================================================================ */

/*
    * Prints the list specified by "list". The List_print() operation moves through all elements in the 
    * linked list and calls function passed as "print" once for each Node's data.
*/
extern void List_print(const List* list, void (*print)(const void* data), char* sep);

/* ================================================================ */

/*
    * Inserts an element at the head of the list specified by "list".
*/
extern int List_insert_first(List* list, const void* data);

/* ================================================================ */

/*
    * Inserts an element at the tail of the list specified by "list".
*/
extern int List_insert_last(List* list, const void* data);

/* ================================================================ */

/*
    * Removes the first element from the list specified by "list".
    * A pointer to node's data is returned.
*/
extern void* List_remove_first(List* list);

/* ================================================================ */

/*
    * Removes the last element from the list specified by "list".
    * A pointer to node's data is returned.
*/
extern void* List_remove_last(List* list);

/*
    * Finds a node in the list "list" with the value "data".
    * Returns a pointer to that node if so.
*/
extern Node* List_find(const List* list, const void* data, int (*match)(const void* key_1, const void* key_2));

/* ================================================================ */

/*
    * Inserts a new node containing data "data" after the node "node" in the list specified by "list".
*/
extern int List_insert_after(List* list, const Node* node, const void* data);

/* ================================================================ */

/*
    * Inserts a new node containing data "data" before the node "node" in the list specified by "list".
*/
extern int List_insert_before(List* list, const Node* node, const void* data);

/* ================================================================ */

/*
    * Removes elements from the list specified by "list". Set its size to 0.
*/
extern void List_clear(List* list);

/* ================================================================ */

/*
    * Inserts elements from the specified array at the end of the list.
*/

extern int List_load_array(List* list, ssize_t size, ssize_t elm_size, void* array);

#endif