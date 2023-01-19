#include "List.h"

static Node* Node_create(const void* data) {
    Node* node = NULL;
    
    if ((node = (Node*) malloc(sizeof(Node))) != NULL) {
        node->next = NULL;
        node->data = (void*) data;
    }

    return node;
}

/* ================================================================ */

List* List_create(void) {
    List* list = NULL;

    if ((list = (List*) malloc(sizeof(List))) == NULL) {
        fprintf(stderr, "%s error: %s\n", __func__, strerror(errno));
    }

    return list;
}

/* ================================================================ */

void List_init(List* list, void (*destroy)(void* data), int (*match)(const void* key_1, const void* key_2)) {
    if ((list != NULL)) { /* Update the function to return a meaningful value */
        list->destroy = destroy;
        list->match = match;

        list->size = 0;

        list->head = list->tail = NULL;
    }

    return ;
}

/* ================================================================ */

void List_destroy(List** list) {
    List_clear(*list);

    free(*list);
    *list = NULL;

    return ;
}

/* ================================================================ */

void List_print(const List* list, void (*print)(const void* data), char* sep) {
    if ((list != NULL) && (print != NULL)) {
        printf("[");

        for (Node* temp = list->head; temp != NULL; temp = temp->next) {
            print(temp->data);

            if (temp->next != NULL) {
                printf("%s", sep);
            }
        }

        printf("]\n");
    }

    return ;
}

/* ================================================================ */

int List_insert_first(List* list, const void* data) {
    int result = EXIT_FAILURE;
    Node* node = NULL;

    if (list != NULL) {
        if ((node = Node_create(data)) != NULL) {
            switch (list->size) {
                case 0:
                    list->head = list->tail = node;
                    break ;

                default:
                    node->next = list->head;
                    list->head = node;
                    break ;
            }

            list->size++;
            result = EXIT_SUCCESS;
        }
    }

    return result;
}

/* ================================================================ */

int List_insert_last(List* list, const void* data) {
    int result = EXIT_FAILURE;
    Node* node = NULL;

    if (list != NULL) {
        if ((node = Node_create(data)) != NULL) {

            switch (list->size) {
                case 0:
                    list->head = list->tail = node;
                    break ;

                default:
                    list->tail->next = node;
                    list->tail = node;
                    break ;
            }

            list->size++;
            result = EXIT_SUCCESS;
        }
    }

    return result;
}

/* ================================================================ */

void* List_remove_first(List* list) {
    void* data = NULL;
    Node* node = NULL;

    if (list != NULL) {
        if (list->size > 0) {
            data = list->head->data;
            node = list->head;

            switch (list->size) {
                case 1:
                    list->head = list->tail = NULL;
                    break ;

                default:
                    list->head = list->head->next;
                    break ;
            }

            free(node);
            list->size--;
        }
    }

    return data;
}

/* ================================================================ */

void* List_remove_last(List* list) {
    void* data;
    Node* node = NULL;
    Node* temp = NULL;

    if (list != NULL) {
        if (list->size > 0) {
            data = list->tail->data;
            node = list->tail;

            switch (list->size) {
                case 1:
                    list->head = list->tail = NULL;
                    break ;

                default:
                    for (temp = list->head; temp->next != list->tail; temp = temp->next) ;

                    list->tail = temp;
                    temp->next = NULL;
            }

            free(node);
            list->size--;
        }
    }

    return data;
}

/* ================================================================ */

Node* List_find(const List* list, const void* data, int (*match)(const void* key_1, const void* key_2)) {
    int (*m)(const void* key_1, const void* key_2) = NULL;
    Node* node = NULL;

    if ((list != NULL) && (data != NULL)) {
        m = (match) ? match : list->match;

        if (m == NULL) {
            fprintf(stderr, "%s: neither the List_init() match parameter nor the List_find() match one is specified.\n", __func__);
            return node;
        }

        for (Node* temp = list->head; temp != NULL; temp = temp->next) {
            if (m(temp->data, data) == 0) {
                node = temp;
                break ;
            }
        }
    }

    return node;
}

/* ================================================================ */

int List_insert_after(List* list, const Node* node, const void* data) {
    int result = EXIT_FAILURE;
    Node* new_node = NULL;

    if ((list != NULL) && (node != NULL)) {
        if (node == list->tail) {
            return List_insert_last(list, data);
        }

        if ((new_node = Node_create(data)) != NULL) {
            for (Node* temp = list->head; temp != NULL; temp = temp->next) {
                if (temp == node) {
                    new_node->next = temp->next;
                    temp->next = new_node;

                    list->size++;
                    result = EXIT_SUCCESS;

                    break ;
                }
            }
        }
    }

    return result;
}

/* ================================================================ */

int List_insert_before(List* list, const Node* node, const void* data) {
    int result = EXIT_FAILURE;
    Node* new_node = NULL;

    if ((list != NULL) && (node != NULL)) {
        if (node == list->head) {
            return List_insert_first(list, data);
        }

        if ((new_node = Node_create(data)) != NULL) {
            for (Node* temp = list->head; temp != NULL; temp = temp->next) {
                if (temp->next == node) {
                    new_node->next = (Node*) node;
                    temp->next = new_node;

                    list->size++;
                    result = EXIT_SUCCESS;

                    break ;
                }
            }
        }
    }

    return result;
}

/* ================================================================ */

void List_clear(List* list) {
    void* data = NULL;

    if (list != NULL) {
        while (list->size > 0) {
            data = List_remove_first(list);

            if (list->destroy) {
                list->destroy(data);
            }
        }
    }

    return ;
}

/* ================================================================ */

int List_load_array(List* list, ssize_t size, ssize_t elm_size, void* array) {
    int result = EXIT_FAILURE;

    if ((array != NULL)) {
        if ((size > 0) && (elm_size > 0)) {
            for (size_t i = 0; i < size; i++) {
                if (List_insert_last(list, (array + i * elm_size)) == EXIT_FAILURE) {
                    break ;
                }
            }

            result = EXIT_SUCCESS;
        }
    }

    return result;
}