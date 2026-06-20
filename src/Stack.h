#include <List.h>

// Simply give it another alias
typedef List Stack;

// and we can simply create macros for the same
#define Stack_create List_create
#define Stack_destroy List_destroy
#define Stack_push List_push
#define Stack_pop List_pop
#define Stack_count List_count
#define STACK_FOREACH(A, V) LIST_FOREACH(A, last, prev, V)
#define Stack_peek List_last

// You can just think of it as providing a protocol to 
// use the List in a certain way
