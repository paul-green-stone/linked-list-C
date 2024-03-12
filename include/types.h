#ifndef types_h
#define types_h

/* ================================================================ */

enum linked_list_error {
    E_OK = 0,           /* Success */
    E_NULL_V = 1,      /* NULL value */
    E_NOMEM = 2,       /* Out of memory */
    E_MISMET = 3,      /* Missing list method */
    E_MATCH = 4,       /* A node doesn't belong to the list */
};

/**
 * \brief A pointer to an incomplete data type, allowing user interaction only through a defined interface.
 */
typedef struct singly_linked_list_node* sNode_t;

/* ================================ */

/**
 * \brief A pointer to an incomplete data type, allowing user interaction only through a defined interface.
 */
typedef struct singly_linked_list* sList_t;

/* ================================ */

/* Singly-linked list methods */
typedef struct methods* Methods;

/* Singly-linked list data */
typedef struct data* Data;

/* ================================================================ */

#endif /* types_h */
