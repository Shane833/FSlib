#ifndef _DArray_h
#define _DArray_h

#include <stdlib.h>
#include <assert.h> // I don't understand why we have used assert.h, since we have not directly used assert function (check later)
#include <dbg.h>

// As the name suggests, Dynamic Arrays are arrays which can grow over time as per the needs of the data required

// Definition of the DArray Struct
typedef struct DArray{
	int end; // maintains the end pointer, the point where insertion takes place
	int max; // holds the maximum array size and then flags to grow it once the limit is reached
	size_t element_size; // fixed size of each element that it needs to store
	size_t expand_rate; // defines how much ammount you want to grow the array after its initial size
	void** contents; // the data of the array itself will be stored here
}DArray;

// Function to create an empty DArray with an initial size
DArray* DArray_create(size_t element_size, size_t initial_max);

// Function to deallocate memory from the array
void DArray_destroy(DArray* array);

// Function to clear the contents of the array, but no deallocation
void DArray_clear(DArray* array);

// Function to expand the array after its full
int DArray_expand(DArray* array);

// Shrink the array if its taking unnecessary space
int DArray_contract(DArray* array);

// Function to insert elements in the array
int DArray_push(DArray* array, void* el);

// Function to remove the elements in the array
void* DArray_pop(DArray* array);

// Function to clear data and then deallocate memory from it
void DArray_clear_destroy(DArray* array);

// Handy macros for simple DArray functions
#define DArray_last(A) ((A)->contents[(A)->end - 1])
#define DArray_first(A) ((A)->contents[0])
#define DArray_end(A) ((A)->end)
#define DArray_count(A) DArray_end(A)
#define DArray_max(A) ((A)->max)

// Defines the default rate at which the array must expand
#define DEFAULT_EXPAND_RATE 300

// Function to set value at the given index
static inline void DArray_set(DArray* array, int i, void* el)
{
	check(i < array->max, "DArray attempt to set past max"); // validates the index
	if(i > array->end){ // if the index exceeds ends we update it
		array->end = i;
	}
	array->contents[i] = el; // setting the value at that position
	
error:
	return;
}

// Function to get the value from the given index
static inline void* DArray_get(DArray* array, int i)
{
	check(i < array->max, "DArray attempt to get past max"); // validating the index
	return array->contents[i]; // returning the contents at the index
error:
	return NULL;
}

// Function to remove the value at the given index
static inline void* DArray_remove(DArray* array, int i)
{
	// Hmm... somehow we are not validating the access for this index ??
	// So lets do that
	
	/* Original Code
	void* el = array->contents[i]; 
	array->contents[i] = NULL;
	return el;
	*/
	
	// Modified Version
	check(i < array->max, "DArray attemp to get past max"); // validating the index
	void* el = array->contents[i]; // retreiving the required element
	array->contents[i] = NULL; // replacing element with NULL
	return el; // returning the element
	
error:
	return NULL;
	
}

// Don't know why we need this function, We don't even use it anywhere in the code
// Correction : I understand now why we are using this function
// It is basically to create a new element which can be added to the darray
static inline void* DArray_new(DArray* array)
{
	// However what this does is basically returns a pointer to a memory region
	// of the size of the element size, and if the element size is greater than 0, means we are storing
	// dynamically mapped elements and not just pointers or 32 bit values
	// we return NULL
	check(array->element_size > 0, "Can't use DArray_new on zero sized array");
	return calloc(1, array->element_size);
error:
	return NULL;
}

// Handy function to deallocate memory from the DArray pointer
#define DArray_free(E) free((E))

#endif
