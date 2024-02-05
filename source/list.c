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

sList_t sList_new(void (*destroy)(void* data), void (*print)(void* data), int (*match)(void* data_1, void* data_2)) {

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
    list->methods->match = match;

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

    int result = 1;
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

    return (result = 0);
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
            list->data->tail->next = NULL;
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

    int result = 1;
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

    return (result = 0);
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

/* ================================ */

sNode_t sList_find(const sList_t list, void* data) {

    sNode_t temp = NULL;

    if (list == NULL) {
        return temp;
    }

    if (list->methods->match == NULL) {
        return temp;
    }

    if (data == NULL) {
        return temp;
    }

    for (temp = list->data->head; temp != NULL; temp = temp->next) {

        if (list->methods->match(temp->data, data) == 0) {
            return temp;
        }
    }

    return temp;
}

/* ================================ */

int sList_insert_after(const sList_t list, const sNode_t node, void* data) {

    sNode_t new_node = NULL;
    sNode_t temp = NULL;

    int result = 1;

    if (list == NULL) {
        return result;
    }

    if ((node == NULL) || (node == list->data->tail)) {
        return sList_insert_last(list, data);
    }

    if ((new_node = Node_new(data)) == NULL) {
        return result;
    }

    for (temp = list->data->head; temp != node && temp != NULL; temp = temp->next) ;

    if (temp == NULL) {
        return result;
    }

    new_node->next = temp->next;
    temp->next = new_node;

    list->data->size++;

    return (result = 0);
}

/* ================================ */

int sList_insert_before(const sList_t list, const sNode_t node, void* data) {

    sNode_t new_node = NULL;
    sNode_t temp = NULL;

    int result = 1;

    if (list == NULL) {
        return result;
    }

    if ((node == NULL) || (node == list->data->head)) {
        return sList_insert_first(list, data);
    }

    if ((new_node = Node_new(data)) == NULL) {
        return result;
    }

    for (temp = list->data->head; temp->next != node && temp != NULL; temp = temp->next) ;

    if (temp == NULL) {
        return result;
    }

    new_node->next = temp->next;
    temp->next = new_node;

    list->data->size++;

    return (result = 0);
}

/* ================================ */

void* sList_delete_Node(const sList_t list, sNode_t node) {

    sNode_t temp = NULL;

    void* data = NULL;

    if (list == NULL) {
        return data;
    }

    if (node == NULL) {
        return data;
    }

    if (node == list->data->head) {
        return sList_remove_first(list);
    }

    if (node == list->data->tail) {
        return sList_remove_last(list);
    }

    for (temp = list->data->head; temp->next != node && temp != NULL; temp = temp->next) ;

    if (temp == NULL) {
        return data;
    }

    temp->next = node->next;

    data = Node_destroy(&node);

    list->data->size--;

    return data;
}

/* ================================ */

void sList_print_verbose(const sList_t list) {

    sNode_t node = NULL;

    if (list == NULL) {
        return ;
    }

    if (list->methods->print == NULL) {
        return ;
    }

    for (node = list->data->head; node != NULL; node = node->next) {

        printf("[");
        printf("data: ");
        list->methods->print(node->data);
        printf("\n");

        printf("next: %p\n", (void*) node->next);
        printf("]");

        if (node != list->data->tail) {
            printf("\n");
        }
    }

    return ;
}

/* ================================================================ */
