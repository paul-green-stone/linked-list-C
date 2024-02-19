#ifndef types_h
#define types_h

/* ================================================================ */

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
