#include "../include/sll.h"

void print_int(void* data) {

    if (data != NULL)
        printf("%d", *((int*) data));

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

    if ((list = sList_new(free, print_int, match_int)) == NULL) {
        printf("%s\n", strerror(errno));
    }

    for (size_t i = 0; i < 100; i++) {
        
        int* value = NULL;

        if ((value = malloc(sizeof(int))) != NULL) {
            *value = rand() % 100;
        }

        if (i == 3) {
            *value = 10;
        }

        sList_insert_last(list, value);
    }

    printf("List size: %ld\n", sList_size(list));

    sList_print(list);

    int a = 10;
    sNode_t node = sList_find(list, &a);

    sList_foreach(list, sqr);

    if (node != NULL) {
        printf("there is a node with such data\n");

        // int* b = malloc(sizeof(int));
        // *b = 999;

        // sList_insert_before(list, node, b);

        void* data = sList_delete_Node(list, node);

        free(data);
    }

    printf("List size: %ld\n", sList_size(list));

    sList_print(list);

    sList_destroy(&list);

    return EXIT_SUCCESS;
}
