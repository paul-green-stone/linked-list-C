#ifndef list_h
#define list_h

/* ================================================================ */

struct singly_linked_list {

    Data data;
    Methods methods;
};

/* ================================ */

/**
 * 
 */
extern sList_t sList_new(void (*destroy)(void* data), void (*print)(void* data), int (*match)(void* data_1, void* data_2));

/* ================================ */

/**
 * 
 */
extern void sList_destroy(sList_t* list);

/* ================================ */

/**
 * 
 */
extern int sList_insert_last(const sList_t list, void* data);

/* ================================ */

/**
 * 
 */
extern int sList_insert_first(const sList_t list, void* data);

/* ================================ */

/**
 * 
 */
extern void* sList_remove_last(const sList_t list); 

/* ================================ */

/**
 * 
 */
extern void* sList_remove_first(const sList_t list); 

/* ================================ */

/**
 * 
 */
extern ssize_t sList_size(const sList_t list);

/* ================================ */

/**
 * 
 */
extern void sList_print(const sList_t list);

/* ================================ */

/**
 * 
 */
extern sNode_t sList_find(const sList_t list, void* data);

/* ================================ */

/**
 * 
 */
extern int sList_insert_after(const sList_t list, const sNode_t node, void* data);

/* ================================ */

/**
 * 
 */
extern int sList_insert_before(const sList_t list, const sNode_t node, void* data);

/* ================================ */

/**
 * 
 */
extern void* sList_delete_Node(const sList_t list, sNode_t node);

/* ================================ */

/**
 * 
 */
extern void sList_print_verbose(const sList_t list);

/* ================================================================ */

#endif /* list_h */
