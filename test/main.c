#include "../include/sll.h"

void print_int(void* data) {

    printf("%d", *((int*) data));

    return ;
}

int main (int argc, char** argv) {

    srand(time(NULL));

    sList_t list = NULL;

    if ((list = sList_new(free, print_int)) == NULL) {
        printf("%s\n", strerror(errno));
    }

    for (size_t i = 0; i < 10; i++) {
        
        int* value = NULL;

        if ((value = malloc(sizeof(int))) != NULL) {
            *value = rand() % 100;
        }

        sList_insert_last(list, value);
    }

    printf("List size: %ld\n", sList_size(list));

    sList_print(list);

    while (sList_size(list) > 0) {

        void* data = sList_remove_first(list);

        free(data);

        sList_print(list);
    }

    sList_destroy(&list);

    return EXIT_SUCCESS;
}
