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
extern sList_t sList_new(void (*destroy)(void* data), void (*print)(void* data));

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

/* ================================================================ */

#endif /* list_h */
