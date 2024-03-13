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

sList_t list = NULL;
int result = sList_new(&list, NULL, how_to_print, how_to_compare);

if (result == 0) {
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

sList_t list = NULL;
int result = sList_new(&list, free, how_to_print, how_to_compare);

if (result == 0) {
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

sList_t list = NULL;
int result = sList_new(&list, Book_destroy, how_to_print, how_to_compare);

// Adding books to the list
if (result == 0) {

   /* Let's say we have a function for creating a book that RETURNS a book on success */
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

/* The same hypothetical function for book creation */
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

The `sList_insert_after` function operates by using a provided search function, `sList_find`, to find the specified node in the linked list. Once the node is found, the function then traverses the list to find the appropriate position to insert the new node. 

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
   */
   return !(strcmp(book_1->title, book_2->title) == 0) && (strcpm(book_1->author, book_2->author) == 0)
}

/* ... */

sList_t list = NULL;
int result = sList_new(&list, Book_destroy, how_to_print, Book_compare);

/* Assume we added a very interesting book in the list */
Book interesting_book = Book_new("Basic Gambling Mathematics", "Mark Bollman");

/* First traversion happens here */
sNode_t node = NULL;
result = sList_find(list, &interesting_book, &node);

if (result == 0) { 
   /* If the node is in the list, the operation is linear */
   result = sList_insert_after(list, node, Book_new("How to Be Twice As Smart", "Scott Witt"));
}

/* ... */
```

Every node contains a reference to the list it belongs to, making insertion after the node (`sList_insert_after`) a linear operation. However, searching for a node with specified data still requires traversal.

#### ⚠️ Warning!

It is important to note that even if two linked lists contain nodes with the same data, the nodes themselves belong to different lists. This means that calling the `sList_find` function on one list and obtaining a node with the given data does not guarantee that data can be inserted into another list using the `sList_insert_after` function. The node must belong to the same list in order for data to be properly inserted.

#### 4️⃣ `sList_insert_before`

The `sList_insert_before` function is almost identical to the `sList_insert_after` function, with the only difference being that the data is inserted before the given node. The same rules and considerations apply to the `sList_insert_before` function as they do to the `sList_insert_after` function, except it traverses the list in order to find a proper position to insert a node.

### 🖼️ Printing

The function `sList_print` is used to output the content of a given list. This function requires the user to provide a function that handles the data stored in each node of the list.

```C
void Book_print(void* data) {

   Book* book = NULL;

   if (data != NULL) {
         book = (Book*) data;

         printf("Title: %s\tAuthor: %s\n", book->title, book->author);
   }
}

/* ... */

sList_t list = NULL;
int result = sList_new(&list, Book_destroy, Book_print, Book_compare);

sList_print(list);
```

### ⛏️ Data Extraction

According to the implementation, a list is a pointer to an incomplete type, which means one cannot directly access its members or its node data. In other words, there is no way to manually set a list size, access the list head element, or retrieve node data manually. However, there are situations where it is necessary to retrieve data stored in the list's nodes for processing. This can be achieved using the `sList_next` function, which, upon invocation, gets the data stored in the next list node. Consider an example:

```C
/* ... */

/* Imagine our list contains books we added in the previous examples */

size_t size = sList_size(list);

for (size_t i = 0; i < size; i++) {

   void* data = NULL;
   Book* book = NULL;

   /* Get data from the next node and store it in `data` */
   sList_next(list, &data);

   book = (Book*) data;

   /* Manually changing a book's title */
   free(book->title);
   book->title = strdup("Very Interesting Story");
}
```

Notice that the `sList_next` function stores data in the generic `void*` pointer. You must cast this pointer according to whatever your list contains. While working with data in the list, the list itself remains untouched; its internal details are protected/hidden and can be modified only via methods defined here.

### 🏥 Error Handling

There are times when a function fails, and one needs to find out what exactly happened. For such cases, there is a function named `sList_error` that takes a value returned from one of the functions in the `sList_` family and prints the meaningful message, I believe it is meaningful 😄. Let's consider the example below:

```C
/* ... */

int* a = NULL;

/* Let's say you want to store an integer value in the list but forgot to allocate memory for it */
int result = sList_insert_last(&list, a);

if (result != 0) {

   /*
      The message will tell you that there is no possibility to insert `NULL` in the list.
      Even though you DID allocate memory for an integer but forgot to check it against `NULL`, the
      function will still inform you that the insertion of `NULL` is not permitted
   */
   sList_error(result);
}
```
