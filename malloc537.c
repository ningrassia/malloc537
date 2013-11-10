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


/*TODO: make sure we remove nodes when we do an insert.
 * When we insert a node that fully covers a freed node,
 * remove the freed node.
 */


extern node * root;

void *malloc537(size_t size)
{
	void * return_ptr;
	node * remove_node = NULL;
	if(size == 0)
	{
		printf("Allocating a pointer of size 0\n");
	}

	return_ptr = malloc(size);

	/*printf("Inserting node: Pointer: %p, bounds %d\n", return_ptr, (int)size);*/
	/*
	 * Need to find all nodes within range base+1 to size, and delete them.
	 */
	
	remove_node = contained_lookup(return_ptr, size);
	while(remove_node != NULL)
	{
		delete_node(remove_node->base);	
		remove_node = contained_lookup(return_ptr, size);
	}
	

	/*HERE WE DO AN INSERT!*/
	insert(return_ptr, size);

	/*Debug! print the tree*/ 
	/*print(root, 0);
	printf("\n");*/
	
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

	/* check for a null pointer to free */
	if(ptr == NULL)
	{
		printf("Trying to free a null pointer!\n");
		exit(EXIT_FAILURE);
	}
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
	/*
	print(root, 0);
	printf("\n");
	*/
}

/*
 * Functions similarly to realloc, but does checking on the input
 * pointer, and stores output pointer data in our hash table
*/
void *realloc537(void *ptr, size_t size)
{
	void * return_pointer;
	node * remove_node = NULL;

	/* If the pointer is null, this is just a malloc! let malloc537 handle it.*/
	if(ptr == NULL)
	{
		return malloc537(size);
	}
	else
	{
		/*HERE WE DO A REMOVE/mark as unused/whatever*/
		node * temp;
		temp = lookup(ptr);
		temp->free = 1;
	}
	
	return_pointer = realloc(ptr, size);

	if(return_pointer != NULL)
	{

		/*
		 * Need to find all nodes within range base+1 to size, and delete them.
		 */
			
		remove_node = contained_lookup(return_pointer, size);
		while(remove_node != NULL)
		{
			delete_node(remove_node->base);	
			remove_node = contained_lookup(return_pointer, size);
		}
		
			
		/*HERE WE DO AN INSERT*/
		insert(ptr, size);
	}
	/*
	print(root, 0);
	printf("\n");
	*/
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
		temp = bounds_lookup(ptr);
		if(temp == NULL)
		{
			printf("Pointer at %p was never allocated!\n", ptr);
			exit(EXIT_FAILURE);
		}
		else
		{
			/* As long as the pointer we're looking up is within the bounds
			 * of the allocated space, we're fine!
			 */
			if((long)((long)ptr + size) <= (long)((long)temp->base + temp->bounds))
			{
				return;
			}
			else
			{
				printf("Pointer at %p is inside pointer %p of size %d, but there's not enough room in the allocated space!\n", ptr, temp->base, (int)temp->bounds);
			}
		}

	}

	/* If we find a pointer at ptr, check the size! */
	else if(size > temp->bounds)
	{
		printf("Trying to use %d bytes, but the pointer %p only has a size of %d bytes.\n", (int)size, ptr, (int)temp->bounds);
		exit(EXIT_FAILURE);

	}
}
