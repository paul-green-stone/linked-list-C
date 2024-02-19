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

    if (value = malloc(sizeof(int)) != NULL) {
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

### Insertion

Data can be inserted in the list in four distinct ways:

#### 1️⃣ `sList_insert_last`

```C
/* ... */

sList_insert_last(list, Book_new("1984", "George Orwell")); // Insert data at the end of the list

/* ... */
```

Time complexity of the function is $O(1)$


#### 2️⃣ `sList_insert_first`

```C
/* ... */

  sList_insert_first(list, Book_new("Stuart Little", "E. B White")); // Insert data at the beginning of the list

/* ... */
```

Time complexity of the function is $O(1)$

#### 3️⃣ `sList_insert_after`

The `sList_insert_after` function is used to insert a new node with specified data after a given node. The function takes a node as an indicator, specifying the node after which the new node should be inserted.

The `sList_insert_after` function operates by using a provided search function, `sList_find`, to find the specified node in the linked list. Once the node is found, the function then traverses the list to find the appropriate position to insert the new node. This results in two traversals of the list, one in the `sList_find` function and another in the `sList_insert_after` function. As a result, the time complexity of the function is $O(n^2)$.

```C
/* ... */

int Book_compare(void* data_1, void* data_2) {
   if ((data_1 == NULL) || (data_2 == NULL)) {
      return 1;
   }

   Book* book_1 = (Book*) data_1;
   Book* book_2 = (Book*) data_2;

   /*
      The function should return 0 when two pieces of data are equal. `strcmp` returns 0 on success,
      and when we combine its result with another successfull `strcmp` call, the 1 will be returned.
      By definition we need 0, so we negate the result to tell pieces of data are equal.

      A bit cryptic, feel free to use other methods, just make sure 0 is returned on success.
   */
   return !(strcmp(book_1->title, book_2->title) == 0) && (strcpm(book_1->author, book_2->author) == 0)
}

/* ... */

sList_t list = sList_new(Book_destroy, how_to_print, Book_compare);

/* Assume we added a very interesting book in the list */

Book interesting_book = Book_new("Basic Gambling Mathematics", "Mark Bollman");

/* First traversion happens here */
sNode_t node = sList_find(list, &interesting_book);

if (node != NULL) {
   /* Second traversion goes here */
   sList_insert_after(list, node, Book_new("How to Be Twice As Smart", "Scott Witt"));
}

/* ... */
```

The current implementation of the `sList_insert_after` function requires two traversals of the linked list, resulting in a time complexity of $O(n^2)$. However, there is a future plan to implement a workaround in the next major update of the data structure.

#### ⚠️ Warning!


It is important to note that even if two linked lists contain nodes with the same data, the nodes themselves belong to different lists. This means that calling the `sList_find` function on one list and obtaining a node with the given data does not guarantee that data can be inserted into another list using the `sList_insert_after` function. The node must belong to the same list in order for data to be properly inserted.

#### 4️⃣ `sList_insert_before`

The `sList_insert_before` function is almost identical to the `sList_insert_after` function, with the only difference being that the data is inserted before the given node. The same rules and considerations apply to the `sList_insert_before` function as they do to the `sList_insert_after` function.

### 🖼️ Printing

The function sList_print is used to output the content of a given list. This function requires the user to provide a function that handles the data stored in each node of the list.

```C
void Book_print(void* data) {

   Book* book = NULL;

   if (data != NULL) {
         book = (Book*) data;

         printf("Title: %s\tAuthor: %s\n", book->title, book->author);
   }
}

/* ... */

sList_t list = sList_new(Book_destroy, Book_print, Book_compare);

sList_print(list);
```
