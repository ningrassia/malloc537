/*
 * rbtree.c
 * Implements lookup and insert methods for a red-black tree!
 */
#include <sys/types.h>

#include "rbtree.h"

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
	return lookup_r(base, bounds, root);
}

node * lookup_r(void * base, node * parent)
{
	/*
	 * return the parent node if it's within the given base
	 * and bounds!
	 */
	if((parent->base = base))
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
	else if(base > parent->base)
	{
		if(base < (parent->base + parent->bounds) && !(parent->free))
		{
			return parent;
		}
		else
		{
			/*
			 * If the base is smaller and it's not contained,
			 */

			node * left_search;
			node * right_search;
			left_search = bounds_lookup_r(base, parent->children[LEFT_CHILD];
			right_search = bounds_lookup_r(base, parent->children[RIGHT_CHILD];
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
		return bounds_lookup_r(base, parent->children[LEFT_CHILD];
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
		temp->root = 1;
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
		if(parent->children[LEFT] != NULL)
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

	if(!(child->parent->red))
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
		

		if((child->parent->parent->child[0]->red == 1) && (child->parent->parent->child[1]->red == 1)){
			
			child->parent->parent->child[0]->red = 0;
			child->parent->parent->child[1]->red == 0;

			/* if the grandparents parent is NULL then it is the root node and we keep it black*/
			if(child->parent->parent->root == 1){

				return 1;

			}
			
			/*otherwise we change the grandparent's color to red and do a recursive call on that node*/
			else{

				child->parent->parent->red = 1;
				clean_tree(child->parent->parent);
		
			}

		}

		/*If this node is the left child of a left child, then we need to restructure*/
		if( (child->parent->parent->child[0] == child->parent) && (child == child->parent->child[0]) )		
		{
	
			
			child->parent->color = 0;
			child->parent->parent->color = 1;
			
			//TODO: Is this malloc valid? Does it do what I want it to? I want to 
			//	create a temporary node with all the grandparents attributes,
			//	otherwise we will lose its pointers
			node * tempGparent = malloc(sizeof(node *));
			tempGparent = child->parent->parent;
			
			/*If the grandparent's parent is null, it is the root*/
			if (child->parent->parent->parent = NULL)
			{

				root = child->parent;

			}
			/*Otherwise we set the great grandparents left child to the parent*/
			else
			{			
	
				child->parent->parent->parent->child[0] = child->parent;

			}
			
			//TODO Do not know if this next line is valid.
			child->parent->parent = child->parent->parent->parent;
			tempGparent->child[0] = child->parent->child[1];
			if (child->parent->child[1] != NULL)
			{

				child->parent->child[1]->parent = tempGparent;

			}
			
			child->parent->child[1] = tempGparent;
			tempGparent->parent = child->parent;
			
			return 1;

		}

		/*If this node is the right child of a right child, then we need to restructure*/
		if( (child->parent->parent->child[1] == child->parent) && (child == child->parent->child[1]) )		
		{
	
			/*Set parent to black and grandparent to red*/
			child->parent->color = 0;
			child->parent->parent->color = 1;
			
			//TODO: Is this malloc valid? Does it do what I want it to? I want to 
			//	create a temporary node with all the grandparents attributes,
			//	otherwise we will lose its pointers
			node * tempGparent = malloc(sizeof(node *));
			tempGparent = child->parent->parent;
			
			/*If the grandparent's parent is null, our parent becomes the root*/
			if (child->parent->parent->parent = NULL)
			{

				root = child->parent;

			}
			/*Otherwise we set the great grandparents right child to the parent*/
			else
			{			
	
				child->parent->parent->parent->child[1] = child->parent;

			}
			
			//TODO Do not know if this next line is valid. What I want to do is to
			//	set the parent's parent field equal to the granparent's parent. this will
			//	break the connection but will we get the information before it is lost?
			child->parent->parent = child->parent->parent->parent;
			
			/*Set the granparents right child to the parents left. RBT rules make this valid*/
			tempGparent->child[1] = child->parent->child[0];
			
			/*If the parent's left child is not null, we set it's parent to the gparent*/
			if (child->parent->child[0] != NULL)
			{

				child->parent->child[0]->parent = tempGparent;

			}
			/*set parents left child to the grandparent*/
			child->parent->child[0] = tempGparent;
			
			/*Set the temporary grandparent's parent to the parent of our child*/
			tempGparent->parent = child->parent;
			
			return 1;

		}
		/*If the parent is the right child and it's parent is red and a left child*/
		if((child == child->parent->child[1]) && (child->parent->parent->child[0] == child->parent))
		{
			/*Change this node's color to black and set the granparent to red*/
			child->color = 0;
			child->parent->parent->color = 1;
			
			//TODO: CAN I DO THIS?
			/*Create temporary node to store grandparent's parent*/
			node * tempNode = malloc(sizeof(node *));
			node * tempNode = child->parent->parent->parent;
			/*set the grandparent to the child's right child. Then we set the gparent's (now the child's child) left child to NULL and parent to child */			
			child->child[1] = child->parent->parent;
			child->child[1]->child[0] = NULL;
			child->child[1]->parent = child;

			/*set the parent to the child's left child. Then we set the parents right child to NULL and it's parent to the child*/
			child->child[0] = child->parent;
			child->child[0]->child[1] = NULL;
			child->child[0]->parent = child;
			
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
		/*If the parent is the left child and it's parent is red and a right child*/
		if((child == child->parent->child[0]) && (child->parent->parent->child[1] == child->parent))
		{
			/*Change this node's color to black and set the granparent to red*/
			child->color = 0;
			child->parent->parent->color = 1;
			
			//TODO: CAN I DO THIS?
			/*Create temporary node to store grandparent's parent*/
			node * tempNode = malloc(sizeof(node *));
			node * tempNode = child->parent->parent->parent;
			
			/*set the grandparent to the child's left child. Then we set the gparent's (now the child's child) right child to NULL and parent to child */
			child->child[0] = child->parent->parent;
			child->child[0]->child[1] = NULL;
			child->child[0]->parent = child;

			/*set the parent to the child's right child. Then we set the parents left child to NULL and it's parent to the child*/
			child->child[1] = child->parent;
			child->child[1]->child[1] = NULL;
			child->child[1]->parent = child;
			
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
}

node * create(void * base, size_t bounds)
{
	node * temp;
	temp = malloc(sizeof(node));
	temp->base = base;
	temp->bounds = bounds;
	temp->root = 0;	
	temp->red = 1;
	temp->free = 0;
	return temp;
}
