/*
 * rbtree.c
 * Implements lookup and insert methods for a red-black tree!
 */
#include <sys/types.h>
#include <stdio.h>
#include <stdlib.h>
#include "rbtree.h"

#define LEFT_CHILD 0
#define RIGHT_CHILD 1

/*
 * This is the root of our tree!
 * Or maybe this should be in the malloc file???
 */
node * root;


node * lookup(void * base)
{
	/*
	 * Just calls the recursive function
	 * on the root of the tree.
	 */
	return lookup_r(base, root);
}

node * lookup_r(void * base, node * parent)
{
	/*
	 * return the parent node if it's within the given base
	 * and bounds!
	 */
	if((parent->base == base))
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
			return lookup_r(base, parent->children[LEFT_CHILD]);
		}
	}
	/* 
	 * Call on right (larger) child if this node's base is too small!
	 */
	else
	{
		if(parent->children[RIGHT_CHILD] != NULL)
		{
			return lookup_r(base,  parent->children[RIGHT_CHILD]);
		}
	}

	/*
	 * If we get here, the node doesn't exist!
	 */
	return NULL;
}

node * bounds_lookup(void * base)
{
	return bounds_lookup_r(base, root);
}

node * bounds_lookup_r(void * base, node * parent)
{
	/*
	 * If our base equals the parent's base, return the parent.
	 */
	if(base == parent->base && !(parent->free))
	{
		return parent;
	}

	/*
	 * If the parent's base is smaller, check if we're inside the parent's space!
	 * Return the parent if we are.
	 * Otherwise, continue our search.
	 */
	else if(base >= parent->base)
	{
		if((size_t)base <= ((size_t)parent->base + parent->bounds) && !(parent->free))
		{
			return parent;
		}
		else
		{
			/*
			 * If the base is smaller and it's not contained,
			 */

			node * left_search = NULL;
			node * right_search = NULL;
			if(parent->children[LEFT_CHILD] != NULL)
			left_search = bounds_lookup_r(base, parent->children[LEFT_CHILD]);
			
			if(parent->children[RIGHT_CHILD] != NULL)
			right_search = bounds_lookup_r(base, parent->children[RIGHT_CHILD]);

			if(right_search != NULL)
			{
				return right_search;
			}
			else
			{
				return left_search;
			}
		}
	}

	/*
	 * If the parent's base is bigger, just keep on looking.
	 */
	else
	{
		return bounds_lookup_r(base, parent->children[LEFT_CHILD]);
	}
}

int insert(void * base, size_t bounds)
{

	int insert_return;
	int clean_tree_return;

	/*
	 * First we make our node.
	 */
	node * temp;
	temp = create(base, bounds);

	/*
	 * Special base case:
	 * empty tree!
	 * For an empty tree, we create a black node,
	 * and insert it as the tree's root!
	 */
	if(root == NULL)
	{
		root = temp;
		root->red = 0;
		return 1;
	}

	/*
	 * Otherwise, pass along our created node
	 * to our recursive insert function.
	 */
	insert_return = insert_r(base, bounds, root, temp);
	if(insert_return < 0)
	{
		printf("Error on insert_r return!\n");
		return insert_return;
	}

	/*DEBUG - print the tree here. we're lazy.*/
	print(root, 0);


	/*
	 * And now, we clean up our messy tree!
	 */
	clean_tree_return = clean_tree(temp);
	if(clean_tree_return < 0)
	{
		printf("Error on clean_tree return!\n");
		return clean_tree_return;
	}

	/*
	 * Return 1 on a success.
	 */
	return 1;

}

int insert_r(void * base, size_t bounds, node * parent, node * temp)
{
	int return_value = 0;

	/*
	 * We always want to insert at the
 bottom of the tree,
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
			parent->free = 0;
			return_value = 1;
		}
		else
		{
			printf("Error on malloc537\nOccupied node with address %p already exists! How did this happen?\n", base);
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

			return_value =  insert_r(base, bounds, parent->children[RIGHT_CHILD], temp);
		}
		else
		{
			parent->children[RIGHT_CHILD] = temp;
			temp->parent = parent;
			return_value =  1;
		}
	}
	
	/*
	 * The only thing left is to check the left node!
	 * Insert a new node if it's empty, or call insert_r
	 * with the left node as parent.
	 */

	else if(parent->base > base)
	{
		if(parent->children[LEFT_CHILD] != NULL)
		{
			return_value =  insert_r(base, bounds, parent->children[LEFT_CHILD], temp);
		}
		else
		{
			parent->children[LEFT_CHILD] = temp;
			temp->parent = parent;
			return_value =  1;
		}
	}




	return return_value;
}

int clean_tree(node * child)
{

	node * tempNode;
	node * tempGparent;
	/*
	 * And here's the fun part!
	 * Now we check for a violation of red-black tree properties!
	 * Because we always insert red nodes, we only need to check
	 * for violations of one of the properties!
	 *
	 * Modeled on CS 367 lecture notes
	 */

	/*
	 * If the child's parent is black, we're fine!
	 */

	
	if((child->parent == NULL) || !(child->parent->red))
	{
		return 1;
	}
	else
	{
		/*
		 * If the child's parent and the parent's sibling is red, just change their color to black and
		 * change the grandparent to red. If the grandparent is the root node, then just keep it
		 * as black to keep the red/black properties
	 	*/	
		
		/*
		 * We need to handle insertion when the depth is low - i.e. we don't have enough nodes
		 * to actually have grandparents/great grandparents.
		 */

		/*NEED TO CHECK IF A CHILD/PARENT EXISTS BEFORE USING IT!*/

		if(child->parent->parent == NULL)
		{
			return 1;
		}

		/* if the grandparent has two children, and they're both red*/
		if((child->parent->parent->children[0] != NULL) && (child->parent->parent->children[1] != NULL) && (child->parent->parent->children[0]->red == 1) && (child->parent->parent->children[1]->red == 1)){
			
			child->parent->parent->children[0]->red = 0;
			child->parent->parent->children[1]->red = 0;

			/* if the grandparent is the root node, we're done!*/
			if(child->parent->parent->parent == NULL){

				return 1;

			}
			
			/*otherwise we change the grandparent's color to red and do a recursive call on that node*/
			else{

				child->parent->parent->red = 1;
				return clean_tree(child->parent->parent);
		
			}

		}

		/*If this node is the left child of a left child, then we need to restructure*/
		if((child->parent->parent->children[0] != NULL) && (child->parent->children[0] != NULL) && (child->parent->parent->children[0] == child->parent) && (child == child->parent->children[0]) )		
		{
			child->parent->red = 0;
			child->parent->parent->red = 1;

			tempGparent = child->parent->parent;
			
			/*If the grandparent's parent is null, it is the root*/
			if (child->parent->parent->parent == NULL)
			{

				root = child->parent;

			}
			/*Otherwise we set the great grandparents left child to the parent*/
			else
			{			
	
				child->parent->parent->parent->children[0] = child->parent;

			}
			
			/*TODO Do not know if this next line is valid. maybe???*/
			child->parent->parent = child->parent->parent->parent;
			tempGparent->children[0] = child->parent->children[1];
			if (child->parent->children[1] != NULL)
			{

				child->parent->children[1]->parent = tempGparent;

			}
			
			child->parent->children[1] = tempGparent;
			tempGparent->parent = child->parent;
			
			return 1;

		}

		/*If this node is the right child of a right child, then we need to restructure*/
		if((child->parent->parent->children[1] != NULL) && (child->parent->children[1] != NULL) && (child->parent->parent->children[1] == child->parent) && (child == child->parent->children[1]) )		
		{
	
			/*Set parent to black and grandparent to red*/
			child->parent->red = 0;
			child->parent->parent->red = 1;
			
			tempGparent = child->parent->parent;
			
			/*If the grandparent's parent is null, our parent becomes the root*/
			if (child->parent->parent->parent == NULL)
			{

				root = child->parent;
				/*something heree???*/

			}
			/*Otherwise we set the great grandparents right child to the parent*/
			else
			{			
	
				child->parent->parent->parent->children[1] = child->parent;

			}
			
			/*TODO Do not know if this next line is valid. What I want to do is to
			 	set the parent's parent field equal to the granparent's parent. this will
				break the connection but will we get the information before it is lost?*/
			child->parent->parent = child->parent->parent->parent;
			
			/*Set the granparents right child to the parents left. RBT rules make this okay*/
			tempGparent->children[1] = child->parent->children[0];
			
			/*If the parent's left child is not null, we set it's parent to the gparent*/
			if (child->parent->children[0] != NULL)
			{

				child->parent->children[0]->parent = tempGparent;

			}
			/*set parents left child to the grandparent*/
			child->parent->children[0] = tempGparent;
			
			/*Set the temporary grandparent's parent to the parent of our child*/
			tempGparent->parent = child->parent;
			
			return 1;

		}
		/*If the parent is a left child, and the child is a right child*/
		if((child->parent->children[1] != NULL) && (child->parent->parent->children[0] != NULL) && (child == child->parent->children[1]) && (child->parent->parent->children[0] == child->parent))
		{
			/*Change this node's color to black and set the granparent to red*/
			child->red = 0;
			child->parent->parent->red = 1;
			
			
			/*Create temporary node to store grandparent's parent*/
			tempNode = child->parent->parent->parent;
			/*set the grandparent to the child's right child. Then we set the gparent's (now the child's child) left child to NULL and parent to child */
			child->parent->parent->children[0] = child->children[1]; 			
			child->children[1] = child->parent->parent;
			child->children[1]->parent = child;

			/*set the parent to the child's left child. Then we set the parents right child to NULL and it's parent to the child*/
			child->parent->children[1] = child->children[0];
			child->children[0] = child->parent;
			child->children[0]->parent = child;
			
			/*If the grandparent's parent was null, then we set the child to the root*/
			if (tempNode == NULL)
			{

				child->parent = NULL;
				root = child;	
	
			}
			/*Otherwise we set the child's parent to the grandparents parent*/
			if (tempNode != NULL)
			{

				child->parent = tempNode;

			}

			
			return 1;
		}
		/*If the parent is a right child and the child is a left child*/
		if((child->parent->children[0] != NULL) && (child->parent->parent->children[1] != NULL) && (child == child->parent->children[0]) && (child->parent->parent->children[1] == child->parent))
		{
			/*Change this node's color to black and set the granparent to red*/
			child->red = 0;
			child->parent->parent->red = 1;
			
			/*Create temporary node to store grandparent's parent*/
			tempNode = child->parent->parent->parent;
			
			/*set the grandparent to the child's left child. Then we set the gparent's (now the child's child) right child to NULL and parent to child */
			child->parent->parent->children[1] = child->children[0]; 
			child->children[0] = child->parent->parent;
			child->children[0]->parent = child;

			/*set the parent to the child's right child. Then we set the parents left child to NULL and it's parent to the child*/

			child->parent->children[0] = child->children[1];
			child->children[1] = child->parent;
			child->children[1]->parent = child;
			
			/*If the grandparent's parent was null, then we set the child to the root*/
			if (tempNode == NULL)
			{

				child->parent = NULL;
				root = child;	
	
			}
			/*Otherwise we set the child's parent to the grandparents parent*/
			if (tempNode != NULL)
			{

				child->parent = tempNode;

			}
			return 1;
		}
	}

	return 0;
}

node * create(void * base, size_t bounds)
{
	node * temp;
	temp = malloc(sizeof(node));
	temp->base = base;
	temp->bounds = bounds;
	temp->red = 1;
	temp->free = 0;
	return temp;
}

void print(node * root, int depth)
{
	int i;
	if(root->children[LEFT_CHILD] != NULL)
	print(root->children[LEFT_CHILD], depth + 1);
	for(i = 0; i < depth; i++)
	{
		printf(".");
	}

	printf("red =  %d node at %p with size %i\n",root->red, root->base, (int)root->bounds);

	if(root->children[RIGHT_CHILD] != NULL)
	print(root->children[RIGHT_CHILD], depth + 1);
}
