#include "../include/sll.h"

/* ================================================================ */

/**
 * 
 */
struct singly_linked_list_node {

    sNode_t next;

    void* data;
};

/**
 * 
 */
struct methods {

    void (*destroy)(void* data);
    void (*print)(void* data);
    int (*match)(void* data_1, void* data_2);
};

/**
 * 
 */
struct data {
    ssize_t size;

    sNode_t head;
    sNode_t tail;
};

/* ================================ */

/**
 * The function allocates memory for a linked list node.
 * If the node's data is NULL, the function execution is discarded.
 */
static sNode_t Node_new(void* data) {

    sNode_t node = NULL;

    if (data == NULL) {
        return NULL;
    }

    if ((node = calloc(1, sizeof(sNode))) == NULL) {
        return node;
    }

    node->data = data;

    return node;
}

/* ================================ */

/**
 * The function deallocates memory allocated to fit the list node.
 * Data a node contains is returned, so it's up to the caller to delete it.
 */
static void* Node_destroy(sNode_t* node) {

    void* data = NULL;

    if ((node == NULL) || (*node == NULL)) {
        return data;
    }

    data = (*node)->data;
    
    free(*node);

    /* Upon return the node is NULL */
    *node = NULL;

    return data;
}

/* ================================================================ */

sList_t sList_new(void (*destroy)(void* data), void (*print)(void* data)) {

    sList_t list = NULL;

    if ((list = calloc(1, sizeof(sList))) == NULL) {
        return NULL;
    }

    if ((list->methods = calloc(1, sizeof(struct methods))) == NULL) {
        sList_destroy(&list);
    }

    if ((list->data = calloc(1, sizeof (struct data))) == NULL) {
        sList_destroy(&list);
    }

    list->methods->destroy = destroy;
    list->methods->print = print;

    return list;
}

/* ================================ */

void sList_destroy(sList_t* list) {

    void* data = NULL;

    if ((list == NULL) || (*list == NULL)) {
        return ;
    }

    while ((*list)->data->size > 0) {

        data = sList_remove_first(*list);

        if ((*list)->methods->destroy != NULL) {
            (*list)->methods->destroy(data);
        }
    }

    free((*list)->data);
    free((*list)->methods);
    free(*list);

    *list = NULL;

    return ;
}

/* ================================ */

int sList_insert_last(const sList_t list, void* data) {

    int result = 0;
    sNode_t node = NULL;

    if (list == NULL) {
        return result;
    }

    if ((node = Node_new(data)) == NULL) {
        return result;
    }

    if (list->data->size == 0) {
        list->data->head = list->data->tail = node;
    }
    else {
        list->data->tail->next = node;
        list->data->tail = node;
    }

    list->data->size++;

    return (result = 1);
}

/* ================================ */

void* sList_remove_last(const sList_t list) {

    ssize_t size;

    void* data = NULL;
    sNode_t node = NULL;
    sNode_t temp = NULL;

    if (list == NULL) {
        return data;
    }

    size = list->data->size;

    if (size > 0) {

        data = list->data->tail->data;
        node = list->data->tail;

        if (size == 1) {
            list->data->head = list->data->tail = NULL;
        }
        else {

            for (temp = list->data->head; temp->next != list->data->tail; temp = temp->next) ;

            list->data->tail = temp;
        }

        Node_destroy(&node);

        list->data->size--;
    }

    return data;
}

/* ================================ */

ssize_t sList_size(const sList_t list) {

    if (list == NULL) {
        return -1;
    }

    return list->data->size;
}

/* ================================ */

extern void sList_print(const sList_t list) {

    sNode_t node = NULL;

    if (list == NULL) {
        return ;
    }

    if (list->methods->print == NULL) {
        return ;
    }

    printf("[");

    for (node = list->data->head; node != NULL; node = node->next) {

        list->methods->print(node->data);

        if (node != list->data->tail) {
            printf(" -> ");
        }
    }

    printf("]\n");

    return ;
}

/* ================================ */

int sList_insert_first(const sList_t list, void* data) {

    int result = 0;
    sNode_t node = NULL;

    if (list == NULL) {
        return result;
    }

    if ((node = Node_new(data)) == NULL) {
        return result;
    }

    if (list->data->size == 0) {
        list->data->head = list->data->tail = node;
    }
    else {
        node->next = list->data->head;
        list->data->head = node;
    }

    list->data->size++;

    return (result = 1);
}

/* ================================ */

void* sList_remove_first(const sList_t list) {

    ssize_t size = 0;

    void* data = NULL;
    sNode_t node = NULL;

    if (list == NULL) {
        return data;
    }

    size = list->data->size;

    if (size > 0) {

        data = list->data->head->data;
        node = list->data->head;

        if (size == 1) {
            list->data->head = list->data->tail = NULL;
        }
        else {
            list->data->head = list->data->head->next;
        }

        Node_destroy(&node);

        list->data->size--;
    }

    return data;
}

/* ================================================================ */
