/*
 *537malloc.c
 *Written by Nik Ingrassia (ingrassi) and Blake Martin (blakem)
 *
 *Adds memory checking bits and pieces to standard malloc/realloc/free,
 *and defines a new function 537memcheck to make sure a pointer
 *has been properly allocated before use!
*/

#include <stdio.h>
#include <stdlib.h>
#include "malloc537.h"
#include "rbtree.h"

/*
 * Allocates memory using malloc, and stores a tuple of address and length
 * in a hash table.
 * Checks for zero size!
*/
void *malloc537(size_t size)
{
	void * return_ptr;

	if(size == 0)
	{
		printf("Allocating a pointer of size 0\n");
	}

	return_ptr = malloc(size);

	/*HERE WE DO AN INSERT!*/
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
	temp = lookup(ptr);
	
	/*
	 * Check to make sure we have a node at that pointer!
	 */
	if(temp == NULL)
	{
		
		temp = bounds_lookup(ptr);
		if(temp == NULL)
		{
			printf("Pointer at %p was never allocated!", ptr);
			exit(EXIT_FAILURE);
		}
		else
		{
			printf("Attempting to free pointer at %p, but that pointer is in memory allocated starting at %p with bounds %d.\n", ptr, temp->base, (int)temp->bounds);
			exit(EXIT_FAILURE); 
		}

	}	
	/*
	 * If we've already freed this node, say that there's a double free! woo.
	 */
	if(temp->free == 1)
	{
		printf("Pointer at %p of previous size %i was already freed!\n", ptr, (int)temp->bounds);
		exit(EXIT_FAILURE);
	}

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
		/*HERE WE DO A REMOVE/mark as unused/whatever*/
		node * temp;
		temp = lookup(ptr);
		temp->free = 1;
	}
	
	return_pointer = realloc(ptr, size);

	if(return_pointer != NULL)
	{
		/*HERE WE DO AN INSERT*/ 
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
	/*
	 *Find the node!
	 *If it doesn't exist, see if it's in the previous
	 *node's range and print appropriate error
	 *Otherwise, check the size.
	 *If they don't match, print an error.
	 */	
	node * temp = lookup(ptr);
	
	if(temp == NULL)
	{
		/*
		 * This means we haven't allocated it, or it's not the first byte.
		 * I think we need a "find next smallest" to check the next thing's range.
		 * For now, just say it's an error!
		 */
	}

	/*
	 * Not done. I'm lazy.
	 */
}
