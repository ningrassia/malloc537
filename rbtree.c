/*
 * rbtree.c
 * Implements lookup and insert methods for a red-black tree!
 */
#include <sys/types.h>

#include "rbtree.h"

/*
 * This is the root of our tree!
 */
node root;


node * lookup(size_t base, size_t bounds)
{
	/*
	 * Just calls the recursive function
	 * on the root of the tree.
	 */
	return lookup_r(base, bounds, root);
}

node * lookup_r(size_t base, size_t bounds, node * parent)
{
	/*
	 * return the parent node if it's within the given base
	 * and bounds!
	 */
	if((parent->base >= base) && (parent->base <= (base + bounds)))
	{
		return parent;
	}
	/*
	 * Call on the left (smaller) child if this node's base is too big!
	 */
	else if(parent->base > base)
	{
		if(parent->children[LEFT_CHILD] != NULL)
		{
			return lookup_r(base, bounds parent->children[LEFT_CHILD]);
		}
	}
	/* 
	 * Call on right (larger) child if this node's base is too small!
	 */
	else
	{
		if(parent->children[RIGHT_CHILD] != NULL)
		{
			return lookup_r(base, bounds,  parent->children[RIGHT_CHILD]);
		}
	}

	/*
	 * If we get here, the node doesn't exist!
	 */
	return NULL;
}

int insert(size_t base, size_t bounds)
{

	/*
	 * Special base case:
	 * empty tree!
	 * For an empty tree, we create a black node,
	 * and insert it as the tree's root!
	 */
	if(root == NULL)
	{
		root = create(base, bounds);
		root->red = 0;
		return 1;
	}


	return insert_r(base, bounds, root);
}

int insert_r(size_t base, size_t bounds, node * parent)
{
	int return_value = 0;

	/*
	 * We always want to insert at the bottom of the tree,
	 * so we insert the node at the bottom first,
	 * then fix problems!
	 */

	/*
	 * Replace the already existing node
	 * as long as it's been freed, otherwise
	 * return -1 (error).
	 */
	if(parent->base == base)
	{
		if(parent->free)
		{
			parent->bounds = bounds;
			free = 0;
			return_value = 1;
		}
		else
		{
			printf("Error on malloc537\nNode with address %d already exists! How did this happen?", base);
			return -1;
		}
	}

	/*
	 * If our base is bigger than the parent's base,
	 * look at the right node. If it's empty,
	 * make a new node and put it there.
	 * Otherwise, call insert_r on that right node!
	 * Recursion!
	 */
	else if(parent->base < base)
	{
		if(parent->children[RIGHT_CHILD] != NULL)
		{
			return insert_r(base, bounds, parent->children[RIGHT_CHILD]);
		}
		else
		{
			parent->children[RIGHT_CHILD] = create(base, bounds);
			return 1;
		}
	}
	
	/*
	 * The only thing left is to check the left node!
	 * Insert a new node if it's empty, or call insert_r
	 * with the left node as parent.
	 */

	else if(parent->base > base)
	{
		if(parent->children[LEFT] != NULL)
		{
			return insert_r(base, bounds, parent->children[LEFT_CHILD]);
		}
		else
		{
			parent->children[LEFT_CHILD] = create(base, bounds);
			return 1;
		}
	}
	return return_value;
}

node * create(size_t base, size_t bounds)
{
	node * temp;
	temp = malloc(sizeof(node));
	temp->base = base;
	temp->bounds = bounds;
	temp->red = 1;
	temp->free = 0;
	return temp;
}
