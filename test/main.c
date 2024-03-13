#include "../include/sll.h"

#include <string.h>
#include <time.h>
#include <assert.h>

#define SIZE 10

int square(void* data) {

    int* n = ((int*) data);

    if (!(*n % 2)) {
        *n *= *n;

        return 1;
    }

    return 0;
}

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

    int result = 1;

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

    if (sList_new(&list1, NULL, print_int, match_int) != 0) {
        return EXIT_FAILURE;
    }

    int* t = NULL;

    int r = sList_insert_first(list1, t);

    if (r) {
        printf("%d\n", r);
        sList_error(r);
    }

    int values[SIZE];

    for (size_t i = 0; i < SIZE; i++) {
        values[i] = rand() % 100;
    }

    printf("values: ");
    for (size_t i = 0; i < SIZE; i++) {
        printf("%d", values[i]);

        if (i != SIZE - 1) {
            printf(", ");
        }
    }
    printf("\n");

    for(size_t i = 0; i < SIZE; i++) {
        if (sList_insert_first(list1, &values[i]) != 0) {
            printf("Something went wrong\n");
        }
    }

    printf("list1:  ");
    r = sList_print(list1, ", ");
    if (r != 0) {
        sList_error(r);
    }

    int result = sList_foreach(list1, sqr);

    printf("%lld\n", result);
    printf("list1:  ");

    sList_print(list1, ", ");

    void* data = NULL;

    for (size_t i = 0; i < 5; i++) {

        sList_next(list1, &data);

        *((int*) data) = -1;
    }

    sList_next(list1, &data);

        *((int*) data) = -1000;

    printf("list1:  ");
    sList_print(list1, ", ");

    int a = -1000;
    sNode_t n = NULL;
    sList_find(list1, &a, &n);

    a = 21242135;

    sList_insert_after(list1, n, &a);

    sList_print(list1, NULL);

    sList_destroy(&list1);

    assert(list1 == NULL);

    return EXIT_SUCCESS;
}
