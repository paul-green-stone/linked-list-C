#include "../include/sll.h"

/* ================================================================ */

/**
 * 
 */
struct singly_linked_list_node {

    sNode_t next;   /**< A pointer to the next node in a singly-linked list */

    void* data;     /**< Node's data */

    sList_t list;   /**< The list a node belongs to */
};

/**
 * The `methods` struct encapsulates all the methods available for a list.
 * It serves as a container for the function pointers that define the behavior and operations supported by the list,
 * providing a unified interface to interact with the list's functionality.
 * 
 * The methods struct is defined as an incomplete data type, 
 * which means that its function pointers are not specified within the struct definition.
 * Instead, the function pointers are defined separately in the code where the methods struct is used.
 */
struct methods {

    /**
     * \brief Provides a way to free dynamically allocated data when \link sList_destroy \endlink is called.
     * 
     * Provides a way to free dynamically allocated data when \link sList_destroy \endlink is called.
     * For example, if the list contains data dynamically allocated using `malloc`,
     * destroy should be set to `free` to free the data as the linked list is destroyed. 
     * For structured data containing several dynamically allocated members, `destroy` should be set to a user-defined function
     * that calls `free` for each dynamically allocated member as well as for the structure itself.
     * For a linked list containing data that should not be freed, `destroy` should be set to `NULL`.
     * 
     * @param[in] data Node's data
     * 
     * \return None
     */
    void (*destroy)(void* data);

    /**
     * \brief Provides a way to display Node's data.
     * 
     * The `print` method is used to output the data held by each node in the singly-linked list.
     * It does this by traversing the list and calling the user-defined `print` function for each node's data.
     * 
     * @param[in] data Node's data.
     * 
     * \return None.
    */
    void (*print)(void* data);

    /**
     * \brief Provides a way to compare data stored in a node.
     * 
     * The `match` method is a user-defined function that compares the data held by a node with arbitrary data.
     * 
     * @param[in] data_1 The data held by the node
     * @param[in] data_2 The data to be compared with the node's data.
     * 
     * \return `0` if the two values are equal, indicating a successful match; any non-zero value if the two values are not equal, indicating a mismatch.
     */
    int (*match)(void* data_1, void* data_2);
};

/**
 * A singly-linked list data.
 */
struct data {
    ssize_t size;   /**< Number of elements in a singly-linked list */

    sNode_t head;   /**< The first node of the singly-linked list */
    sNode_t tail;   /**< The last node of the singly-linked list */
};

/* ================================ */

/**
 * \brief Creates a new instance of a list node.
 * 
 * This function creates a new instance of a list node and initializes its `data`
 * field with the provided value. If the `data` argument is NULL, the function
 * will return NULL, indicating an error.
 * 
 * @param[in] data A void pointer to the data to be stored in the node.
 * 
 * \return A pointer to the newly created node if successful, NULL otherwise.
 */
static sNode_t Node_new(void* data) {

    sNode_t node = NULL;

    if (data == NULL) {
        return NULL;
    }

    if ((node = calloc(1, sizeof(struct singly_linked_list_node))) == NULL) {
        return node;
    }

    node->data = data;

    return node;
}

/* ================================ */

/**
 * \brief Destroys a list node and frees its associated memory.
 * 
 * This function deallocates the memory occupied by the provided list node
 * and calls the user-defined `destroy` function to free the node's data.
 * 
 * @param[in] node A pointer to the list node to be destroyed.
 * 
 * @remark The `destroy` function should be implemented by the user and will be
 *         responsible for freeing the memory occupied by the data stored in the
 * 
 * \return None.
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

    if ((list = calloc(1, sizeof(struct singly_linked_list))) == NULL) {
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

    node->list = list;

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

extern void sList_print(const sList_t list, const char* d) {

    sNode_t node = NULL;

    if (list == NULL) {
        return ;
    }

    if (list->methods->print == NULL) {
        return ;
    }

    printf("[\n");

    for (node = list->data->head; node != NULL; node = node->next) {

        list->methods->print(node->data);

        if (node != list->data->tail) {
            printf("%s", (d != NULL) ? d : " -> ");
        }
    }

    printf("\n]\n");

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

    node->list = list;

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

    node->list = list;

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

    node->list = list;

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
        printf("current: %p\n", (void*) node);
        printf(" data: {");
        list->methods->print(node->data);
        printf("}\n");

        printf(" next: %p", (void*) node->next);
        printf("]\n");

        if (node != list->data->tail) {
            printf("\n");
        }
    }

    printf("\n");

    return ;
}

/* ================================================================ */

int sList_foreach(const sList_t list, int (*func)(void* data)) {

    int result = 1;

    sNode_t node = NULL;

    if (list == NULL) {
        return result;
    }

    if (func == NULL) {
        return result;
    }

    for (node = list->data->head; node != NULL; node = node->next) {
        func(node->data);
    }

    return (result = 0);
}

/* ================================================================ */

int sNode_belongs(const sNode_t node, const sList_t list) {

    if ((node == NULL) || (list == NULL)) {
        return -1;
    }

    return !(node->list == list);
}
