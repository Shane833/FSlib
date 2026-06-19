#include <darray.h>
#include <assert.h> // literally why is this here ??

// FUNCTION DEFINTIONS

// Create Function
DArray* DArray_create(size_t element_size, size_t initial_max)
{
	DArray* array = malloc(sizeof(DArray)); // Allocating and checking the new DArray pointer
	check_mem(array);
	array->max = initial_max; // Setting the initial max and checking if its valid
	check(array->max > 0, "You must set an initial max > 0");
	
	// Allocating the same initial amount as a contiguous memory and checking it
	array->contents = calloc(initial_max, sizeof(void *)); // What is size of void *, I assume 32bit/4 bytes as is with all the pointers
	check_mem(array->contents);
	
	array->end = 0; // initially the array will be empty hence it will be 0
	array->element_size = element_size; // setting the element size variable
	array->expand_rate = DEFAULT_EXPAND_RATE; // setting the default expansion rate
	
	return array; // simply return the DArray pointer
error:
	DArray_destroy(array); // In case of failure destroy the DArray and return NULL
	return NULL;
}

void DArray_clear(DArray* array)
{
	int i = 0; // out counter/iterator
	if(array->element_size > 0){ // I don't know why we check if the element_size is greater than 0
	// So the reason why we check for size size 0 is that the elements are defined on the stack instead of heap 
	// So if the elements that are statically added don't need to be deallocated 
		//for(i = 0;i < array->max; i++){ // simply iterate over the array
        for(i = 0;i < array->end;i++){ // Iterating over till the end not the max
			if(array->contents[i] != NULL){ // if elements exists
				free(array->contents[i]); // then deallocate memory from them
			}
		}
	}
}

static inline int DArray_resize(DArray* array, size_t newsize)
{
	array->max = newsize; // we set the new max size (either greater or smaller than existing)
	check(array->max > 0, "The newsize must be greater than 0"); // check if its valid
	
	void* contents = realloc(array->contents, array->max * sizeof(void*)); // using realloc function 
	check_mem(contents); // check contensts and assume realloc don't harm the original on error
	
	array->contents = contents; // simply providing it the new address of reallocated memory region
	
	return 0;
error:
	return -1;
}

int DArray_expand(DArray* array)
{
	size_t old_max = array->max; // we store the original max size
	// Then calling the resize function to actually expand that memory region
	check(DArray_resize(array, array->max + array->expand_rate) == 0, "Failed to expand array to new size : %d", array->max + array->expand_rate);
	// memset function allows us to set a memory region on size n with the given bytes in the second parameter
	// The function defintiion follows as :
	// void *memset(void *s, int c, size_t n) and returns the pointer to that memory area
	memset(array->contents + old_max, 0, array->expand_rate + 1);
	// Then we are initializing the newly added bytes to 0
	// I don't understand why the size is set at expand_rate + 1, maybe so that it don't go out of bounds ??
	// or as the size is n the indexes would only go to n-1, therefore the extra 1 at the end
	// Also why are we setting the new memory region with 0 upto 300 indexes ????
	// Ans 1) See the reason why we are initializing expand rate + 1 is bcz consider this, we have a base address of 1000 and we want to set bytes up to and including 1050 to 0, this means we have 51 mean memory locations not 50 
	// Ans 2) Yes somewhat I think my logic was not exactly right but I was getting there 
	// Ans 3) We are not starting from the base address, see if my base address was 1000 and I have 50 elements then I have nemory filled up to 1050 and say I want to add another 50 using realloc, well doing so would give me 50 garbage values, so we would start with 1050 (base + old size) up to 50 bytes ahead(51 if we include the address 1050 itself) which is basically the size
	
	return 0;
error:
	return -1;
}

int DArray_contract(DArray* array)
{
	// we compute the new size as per the expand rate and the array end
	// if the end is less than expand rate then we won't need to contract
	// however if the end is past that and its taking extra space than the initial size
	// then we should contract and resize it
	// Lets check this feature and if it is correct or not in its implementation
	
	// What if I make some changes bcz, this check seems redundant as we already this function is only called when end > expand_rate
	// No bcz atleast when we call this function manually we must be able to atleast, and since the expand rate is fixed
	// multiple calls to it won't change the new_size until we don't go over the expand rate
	int new_size = array->end < (int)array->expand_rate ? (int)array->expand_rate : array->end;
	printf("New Size : %d, Array End : %d\n", new_size + 1, array->end); 
	
	//int new_size = array->end;
	return DArray_resize(array, new_size + 1); // again passing the extra 1 for size to index conversion
}

void DArray_destroy(DArray* array)
{
	if(array){ // If the array pointer is valid
		if(array->contents){ // if the content array is valid
			free(array->contents); // deallocate the memory from the contents array
		} 
		free(array); // deallocate the array pointer
	}
}

void DArray_clear_destroy(DArray* array)
{
	// Simply both functions called one after the other
	DArray_clear(array);
	DArray_destroy(array);
}

int DArray_push(DArray* array, void* el)
{
	array->contents[array->end] = el; // adding the element at the end
	array->end++; // incrementing the end
	
	if(DArray_end(array) >= DArray_max(array)){ // if we have crossed the max size
		return DArray_expand(array); // simply expand the whole DArray
	}
	else{
		return 0;
	}
}

void* DArray_pop(DArray* array)
{
	check(array->end - 1  >= 0, "Attempt to pop from an empty list"); // validating for an empty list
	
	void* el = DArray_remove(array, array->end - 1); // we store the element
	array->end--; // decrement the end
	
	if(DArray_end(array) > (int)array->expand_rate && DArray_end(array) % (int)array->expand_rate){ 
		// I understood if the end is greater than the expand rate but why perfectly divisible by it ??
		// Correction* : It shouldn't be perfectly divisible by the expand rate but still why ??
		// And if we jump to the contract function it is checking if end < expand rate which will 
		// always be false and since this block will only run when end > expand rate so why the extra check ??
		// ANSWER : Well this function can be called manually thats why, it was so fucking simple you dumbfuck
		DArray_contract(array); 
	}
	
	return el;
error:
	return NULL;
}
