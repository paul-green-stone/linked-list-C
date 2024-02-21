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

int main (int argc, char** argv) {

    srand(time(NULL));
    clock_t start, end;

    sList_t list = NULL;

    if ((list = sList_new(free, print_int, match_int)) == NULL) {
        printf("%s\n", strerror(errno));
    }

    
    for (size_t i = 0; i < 10; i++) {
        int* v = malloc(sizeof(int));

        *v = rand() % 100;

        if (i == 8) {
            *v = 999;
        }

        sList_insert_first(list, v);
    }

    int target = 999;

    start = clock();
    sNode_t node = sList_find(list, &target);

    printf("The given node belongs to list: %s\n", sNode_belongs(node, list) == 0 ? "yes" : "no");
    
    if (node != NULL) {
        int *v = malloc(sizeof(int));

        *v = -12;

        sList_insert_after(list, node, v);
        end = clock();

        printf("Found and inserted in %f seconds\n", ((double) (end - start)) / CLOCKS_PER_SEC);
    }

    sList_print(list, NULL);

    sList_destroy(&list);

    return EXIT_SUCCESS;
}
