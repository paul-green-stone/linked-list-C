#include "../include/sll.h"

#include <string.h>
#include <time.h>

typedef struct {
    char* title;
    char* author;
} Book;

Book* Book_new(const char* t, const char* a) {
    Book* book = calloc(1, sizeof(Book));

    book->title = strdup(t);
    book->author = strdup(a);

    return book;
}

void Book_destroy(void* b) {
    if (b != NULL) {
        Book* book = (Book*) b;

        if (book != NULL) {
            free(book->title);
            free(book->author);

            free(book);
        }
    }

    return ;
}

void Book_print(void* b) {

    if (b != NULL) {
        Book* book = (Book*) b;

        if (book != NULL) {
            printf("Title: %s; Author: %s", book->title, book->author);
        }
    }

    return ;
}

int sqr(void* data) {

    int result = 0;

    *((int*) data) = *((int*) data) * *((int*) data);

    return result;
}

int match_int(void* data_1, void* data_2) {
    
    if ((data_1 == NULL) || (data_2 == NULL)) {
        return -1;
    }

    return !(*((int*) data_1) == *((int*) data_2));
}

void print_int(void* data) {

    if (data != NULL) {
        printf("%d", *((int*) data));
    }

    return ;
}

/* ================================================================ */

int main (int argc, char** argv) {

    srand(time(NULL));
    clock_t start, end;

    sList_t list1 = NULL;
    sList_t list2 = NULL;

    /* Creating a list */
    if ((list1 = sList_new(free, print_int, match_int)) == NULL) {
        printf("%s\n", strerror(errno));
    }

    /* Creating a list */
    if ((list2 = sList_new(free, print_int, match_int)) == NULL) {
        printf("%s\n", strerror(errno));
    }

    /* ================================ */
    /* Populating a list with values */

    for (size_t i = 0; i < 10; i++) {
        int* v = malloc(sizeof(int));

        *v = rand() % 100;

        if (i == 8) {
            *v = 999;
        }

        sList_insert_first(list1, v);
    }

    for (size_t i = 0; i < 10; i++) {
        int* v = malloc(sizeof(int));

        *v = rand() % 100;

        if (i == 8) {
            *v = 999;
        }

        sList_insert_first(list2, v);
    }

    /* ================================ */
    /* Printing the list */

    printf("list1: ");
    sList_print(list1, ", ");
    
    printf("list2: ");
    sList_print(list2, ", ");

    /* ================================ */
    /* Test whether a node belongs to the list or not */

    int target = 999;

    int* v = malloc(sizeof(int));
    *v = 666;

    sNode_t node = sList_find(list1, &target);

    printf("The given node belongs to list1: %s\n", sNode_belongs(node, list1) == 0 ? "yes" : "no");
    printf("The given node belongs to list2: %s\n", sNode_belongs(node, list2) == 0 ? "yes" : "no");

    /* There is a node with a value of `999` in the list `list2`, hoewever, it is located in the list `list1`, 
    so there won't we insertion in the list in the follwoing code. Insertion in the list `list1` takes place */
    sList_insert_before(list2, node, v);
    sList_insert_before(list1, node, v);

    /* ================================ */
    /* Printing the list */

    printf("list1 ");
    sList_print(list1, ", ");
    
    printf("list2 ");
    sList_print(list2, ", ");

    printf("\n\n\n");

    for(size_t i = 0; i < 100; i++) {

        printf("%d\n", *((int*) sList_next(list2)) );
    }

    printf("\n%d\n", *((int*) sList_next(NULL)) );
    printf("\n%d\n", *((int*) sList_next(list1)) );
    printf("%d\n", *((int*) sList_next(list2)) );

    /* ================================ */
    /* Destroy the list */
    
    sList_destroy(&list1);

    sList_destroy(&list2);

    return EXIT_SUCCESS;
}
