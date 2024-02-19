#include "../include/sll.h"
#include <string.h>

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

int main (int argc, char** argv) {

    srand(time(NULL));

    sList_t list = NULL;

    if ((list = sList_new(Book_destroy, Book_print, match_int)) == NULL) {
        printf("%s\n", strerror(errno));
    }

    sList_insert_first(list, Book_new("Stuart Little", "E. B White"));
    sList_insert_last(list, Book_new("Martin Eden", "Jack London"));
    sList_insert_last(list, Book_new("1984", "George Orwell"));

    sList_print(list, NULL);

    sList_destroy(&list);

    return EXIT_SUCCESS;
}
