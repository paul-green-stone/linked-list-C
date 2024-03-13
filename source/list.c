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
 * @param[out] node A pointer that the function writes into.
 * 
 * \return 0 on success, a non-zero value otherwise.
 */
static int Node_new(void* data, sNode_t* node) {

    sNode_t n = NULL;

    if (data == NULL) {
        return E_NULL_V;
    }

    if ((n = calloc(1, sizeof(struct singly_linked_list_node))) == NULL) {
        return E_NOMEM;
    }

    n->data = data;

    *node = n;

    return E_OK;
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
 * \return 0 on success, a non-zero value otherwise.
 */
static int Node_destroy(sNode_t* node, void** data) {

    if ((node == NULL) || (*node == NULL)) {
        return E_NULL_V;
    }

    *data = (*node)->data;
    
    free(*node);

    /* Upon return the node is NULL */
    *node = NULL;

    return E_OK;
}

/* ================================================================ */

int sList_new(sList_t* list, void (*destroy)(void* data), void (*print)(void* data), int (*match)(void* data_1, void* data_2)) {

    if ((*list = calloc(1, sizeof(struct singly_linked_list))) == NULL) {
        return E_NOMEM;
    }

    if (((*list)->methods = calloc(1, sizeof(struct methods))) == NULL) {
        sList_destroy(list);

        return E_NOMEM;
    }

    if (((*list)->data = calloc(1, sizeof (struct data))) == NULL) {
        sList_destroy(list);

        return E_NOMEM;
    }

    (*list)->methods->destroy = destroy;
    (*list)->methods->print = print;
    (*list)->methods->match = match;

    return E_OK;
}

/* ================================ */

int sList_destroy(sList_t* list) {

    int result = E_OK;

    void* data = NULL;

    if ((list == NULL) || (*list == NULL)) {
        return E_NULL_V;
    }

    while ((*list)->data->size > 0) {

        result = sList_remove_first(*list, &data);

        if ((*list)->methods->destroy != NULL) {
            (*list)->methods->destroy(data);
        }
    }

    free((*list)->data);
    free((*list)->methods);
    free(*list);

    *list = NULL;

    return result;
}

/* ================================ */

int sList_insert_last(const sList_t list, void* data) {

    int result = E_OK;
    sNode_t node = NULL;

    if (list == NULL) {
        return E_NULL_V;
    }

    if ((result = Node_new(data, &node)) != E_OK) {
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

    return result;
}

/* ================================ */

int sList_remove_last(const sList_t list, void** data) {

    int result = E_OK;

    ssize_t size;

    sNode_t node = NULL;
    sNode_t temp = NULL;

    if (list == NULL) {
        return E_NULL_V;
    }

    size = list->data->size;

    if (size > 0) {

        *data = list->data->tail->data;
        node = list->data->tail;

        if (size == 1) {
            list->data->head = (list->data->tail = NULL);
        }
        else {

            for (temp = list->data->head; temp->next != list->data->tail; temp = temp->next) ;

            list->data->tail = temp;
            list->data->tail->next = NULL;
        }

        result = Node_destroy(&node, data);

        list->data->size--;
    }

    return result;
}

/* ================================ */

ssize_t sList_size(const sList_t list) {

    if (list == NULL) {
        return -E_NULL_V;
    }

    return list->data->size;
}

/* ================================ */

int sList_print(const sList_t list, const char* delimiter) {

    sNode_t node = NULL;

    if (list == NULL) {
        return E_NULL_V;
    }

    /* The absence of a print method is not an error */
    if (list->methods->print == NULL) {
        return E_MISMET;
    }

    for (node = list->data->head; node != NULL; node = node->next) {

        list->methods->print(node->data);

        if (node != list->data->tail) {
            printf("%s", (delimiter != NULL) ? delimiter : " -> ");
        }
    }

    printf("\n");

    return 0;
}

/* ================================ */

int sList_insert_first(const sList_t list, void* data) {

    int result = E_OK;
    sNode_t node = NULL;

    if (list == NULL) {
        return E_NULL_V;
    }

    if ((result = Node_new(data, &node)) != E_OK) {
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

    return result;
}

/* ================================ */

int sList_remove_first(const sList_t list, void** data) {

    int result = E_OK;

    ssize_t size = 0;

    sNode_t node = NULL;

    if (list == NULL) {
        return E_NULL_V;
    }

    size = list->data->size;

    if (size > 0) {

        *data = list->data->head->data;
        node = list->data->head;

        if (size == 1) {
            list->data->head = list->data->tail = NULL;
        }
        else {
            list->data->head = list->data->head->next;
        }

        result = Node_destroy(&node, data);

        list->data->size--;
    }

    return result;
}

/* ================================ */

int sList_find(const sList_t list, void* data, sNode_t* node) {

    int result = E_OK;

    sNode_t temp = NULL;

    if (list == NULL) {
        return E_OK;
    }

    if (list->methods->match == NULL) {
        return E_MISMET;
    }

    if (data == NULL) {
        return E_NULL_V;
    }

    for (temp = list->data->head; temp != NULL; temp = temp->next) {

        if (list->methods->match(temp->data, data) == 0) {

            *node = temp;

            return result;
        }
    }

    return result;
}

/* ================================ */

int sList_insert_after(const sList_t list, const sNode_t node, void* data) {

    sNode_t new_node = NULL;

    int result = 0;

    if (list == NULL) {
        return E_NULL_V;
    }

    if ((node == NULL) || (node == list->data->tail)) {
        return sList_insert_last(list, data);
    }

    /* The node is simply belongs to another node, so there is no meaning in insertion of a node after "this" node in the given list */
    if (node->list != list) {
        return E_MATCH;
    }

    if ((result = Node_new(data, &new_node)) != 0) {
        return result;
    }

    new_node->next = node->next;
    node->next = new_node;

    list->data->size++;

    new_node->list = list;

    return E_OK;
}

/* ================================ */

int sList_insert_before(const sList_t list, const sNode_t node, void* data) {

    sNode_t new_node = NULL;
    sNode_t temp = NULL;

    int result = E_OK;

    if (list == NULL) {
        return E_NULL_V;
    }

    if ((node == NULL) || (node == list->data->head)) {
        return sList_insert_first(list, data);
    }

    if (node->list != list) {
        return E_MISMET;
    }

    for (temp = list->data->head; temp->next != node && temp != NULL; temp = temp->next) ;

    if ((result = Node_new(data, &new_node)) != 0) {
        return result;
    }

    new_node->next = temp->next;
    temp->next = new_node;

    list->data->size++;

    new_node->list = list;

    return result;
}

/* ================================ */

int sList_delete_Node(const sList_t list, sNode_t node, void** data) {

    int result = E_OK;

    sNode_t temp = NULL;

    if (list == NULL) {
        return E_NULL_V;
    }

    if (node == NULL) {
        return E_NULL_V;
    }

    if (node == list->data->head) {
        return sList_remove_first(list, data);
    }

    if (node == list->data->tail) {
        return sList_remove_last(list, data);
    }

    if (node->list != list) {
        return E_MATCH;
    }

    for (temp = list->data->head; temp->next != node && temp != NULL; temp = temp->next) ;

    temp->next = node->next;

    result = Node_destroy(&node, data);

    list->data->size--;

    return result;
}

/* ================================ */

int sList_print_verbose(const sList_t list) {

    sNode_t node = NULL;

    if (list == NULL) {
        return -E_NULL_V;
    }

    if (list->methods->print == NULL) {
        return E_MISMET;
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

    return E_OK;
}

/* ================================ */

int sList_foreach(const sList_t list, int (*func)(void* data)) {

    int result = E_OK;

    sNode_t node = NULL;

    if (list == NULL) {
        return E_NULL_V;
    }

    if (func == NULL) {
        return E_NULL_V;
    }

    for (node = list->data->head; node != NULL; node = node->next) {
        result += func(node->data);
    }

    return result;
}

/* ================================ */

int sNode_belongs(const sNode_t node, const sList_t list) {

    if ((node == NULL) || (list == NULL)) {
        return E_NULL_V;
    }

    return !(node->list == list);
}

/* ================================ */

int sList_next(const sList_t list, void** data) {

    /* List that is being operated on */
    static sList_t l = NULL;

    /* The list node whose data was returned by the previous call */
    static sNode_t n = NULL;

    // The first time the function is being called requires us to set up its internals
    // or
    // when the new list specified does not match the one specified in the iterator.
    if ((l == NULL) || ((l != list) && (list != NULL))) {

        l = list;

        if (list->data->size > 0) {
            n = list->data->head;
        }
    }

    if (l == NULL) {
        return E_NULL_V;
    }

    *data = n->data;

    n = n->next;

    if (n == NULL) {
        n = list->data->head;
    }

    return E_OK;
}

/* ================================ */

void sList_error(int code) {

    struct error {
        int code;
        char* msg;
    } errors[] = {
        {E_OK, "\033[0;32mSuccess\033[0;37m"},
        {E_NULL_V, "\033[0;35mWarning\033[0;37m: NULL value provided"},
        {E_NOMEM, "\033[0;31mError\033[0;37m: Out of memory"},
        {E_MISMET, "\033[0;35mWarning\033[0;37m: List method is missing"},
        {E_MATCH, "Foreign node"}
    };

    fprintf(stderr, "%s\n", errors[code].msg);

    return ;
}

/* ================================================================ */
