/*
 *537malloc.c
 *Written by Nik Ingrassia (ingrassi) and Blake Martin (blakem)
 *
 *Adds memory checking bits and pieces to standard malloc/realloc/free,
 *and defines a new function 537memcheck to make sure a pointer
 *has been properly allocated before use!
*/

include "rbtree.h"


/*
 * Allocates memory using malloc, and stores a tuple of address and length
 * in a hash table.
 * Checks for zero size!
*/
void *malloc537(size_t size)
{
	void * return_ptr;

	if(size = 0)
	{
		printf("Allocating a pointer of size 0\n");
	}

	return_ptr = malloc(size);

	//HERE WE DO AN INSERT!
	insert(return_ptr, size);
	
	return return_ptr;
}

/*
 * Check to make sure a 537malloc-family command allocated the memory
 * in ptr, and then calls free on the pointer.
*/
void free537(void *ptr)
{
	node * temp;
	//HERE WE DO A REMOVE - errors handled there!
	temp = lookup(ptr);
	temp->free = 1;
	free(ptr);
}

/*
 * Functions similarly to realloc, but does checking on the input
 * pointer, and stores output pointer data in our hash table
*/
void *realloc537(void *ptr, size_t size)
{
	void * return_pointer;

	if(ptr != NULL)
	{
		//HERE WE DO A REMOVE/mark as unused/whatever
		node * temp;
		temp = lookup(ptr);
		temp->free = 1;
	}
	
	return_pointer = realloc(ptr, size);

	if(return_pointer != NULL)
	{
		//HERE WE DO AN INSERT 
		insert(ptr, size);
	}
	return return_pointer;
}

/*
 * Checks pointer ptr with address range size to see
 * if it has been allocated (and not freed) by 537malloc/537realloc.
 * Prints a nice, verbose error message and quits on an error.
*/
void memcheck537(void *ptr, size_t size)
{
	//Find the node!
	//If it doesn't exist, see if it's in the previous
	//node's range and print appropriate error
	//Otherwise, check the size.
	//If they don't match, print an error.
	node * temp = lookup(ptr, size);
	//BLA BLA BLA DO STUFF HERE!
}
