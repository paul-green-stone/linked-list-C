# Singly-linked list 🚋-🚋-🚋

A singly-linked list. The simplest linked list, in which elements are linked by a single pointer. This structure allows the list to be traversed from its first element to its last

## ✏️ Description

The linked list data structure is designed as a pointer to an incomplete type, which prevents direct modification of its elements by accessing the list's items, such as `list->head = node`. Users can only access and modify the list through a specified interface, ensuring controlled and secure manipulation of the list's contents. For additional information, refer to the [documentation](https://paul-green-stone.github.io/linked-list-C/)

## 🛠️ Usage

### Initialization

Before creating a list, it's important to determine the type of data the list will hold. Depending on whether the list contains statically or dynamically allocated data, or any other complex user data, the appropriate destruction or cleanup mechanism should be employed.

#### 1️⃣ Static Data

If the list contains statically allocated data, the list should not attempt to destroy it when the list itself is being destroyed:

```C
/* ... */
 
int array[] = {0, 2, 4, 6, 8}; // Array of statically allocated integers

sList_t list = sList_new(NULL, how_to_print, how_to_compare);

if (list != NULL) {
  for (size_t i = 0; i < sizeof(array) / sizeof(array[0]); i++) {
    /*
      Even though we store statically-allocated integer variables in the list,
      what we actually hold are their memory addresses. Consequently,
      this allows users to alter the value of these integers
      by directly modifying them through their corresponding array indices
    */
    sList_insert_first(list, &array[i]);
  }

  /* ... */
}

sList_destroy(&list); // It's save to call it on NULL

/* ... */
```

#### 2️⃣ Dynamically Allocated Data

For dynamically allocated data, users should free the memory by calling `free` when removing elements from the list to prevent memory leaks:

```C
/* ... */

size_t size = 5;
sList_t list = sList_new(free, how_to_print, how_to_compare);

if (list != NULL) {
  for (size_t i = 0; i < size; i++) {
    int* value = NULL;

    if ((value = malloc(sizeof(int)) != NULL) {
      *value = i * i;

      /*
        There exists no legal method to modify data residing in the list's nodes
        externally beyond its defined interface because the data is located in the
        heap space, and one must possess a valid pointer to gain access to it.
        Owing to encapsulation principles, although we may state that the list holds
        data internally, all intricate implementation aspects remain concealed from
        our view as external consumers.
      */
      sList_insert_first(list, value);
    }
  }

  /* ... */
}

sList_destroy(&list);

/* ... */
```

#### 3️⃣ Complex User Data

In the case of complex user data, a user-defined function should be called to handle data deletions properly, ensuring that any associated resources are released in a controlled manner:

```C
typedef struct {
  // strdup() is used to dynamically duplicate and assign string values to struct members
  char* title; 
  char* author;
} Book;

void Book_destroy(void* b) {

  if ((b != NULL) && (*b != NULL)) {
    Book* book = (Book*) b;

    // call free() on every element that has been dynamically allocated
    free((*book)->title);
    free((*book)->author);

    free(*book);
  }
}

/* ... */

sList_t list = sList_new(Book_destroy, how_to_print, how_to_compare);

// Adding books to the list
if (list != NULL) {
  sList_insert_first(list, Book_new("Stuart Little", "E. B White"));
  sList_insert_last(list, Book_new("Martin Eden", "Jack London"));
  sList_insert_last(list, Book_new("1984", "George Orwell"));
}

sList_destroy(&list);

/* ... */
```
