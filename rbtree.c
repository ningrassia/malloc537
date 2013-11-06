/*
 * rbtree.c
 * Implements lookup and insert methods for a red-black tree!
 */
#include <sys/types.h>

#include "rbtree.h"

/*
 * This is the root of our tree!
 */
tree rb_tree;


node * lookup(size_t base, size_t bounds)
{
	/*
	 * Just calls the recursive function
	 * on the root of the tree.
	 */
	return lookup_r(base, bounds, rb_tree.root);
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
	return NULL;
}
