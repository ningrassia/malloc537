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

/*These two functions help rotate our tree when we delete a node. It makes the correct
 *connections to parent's, children, etc.*/
void rotate_l(node * lnode)
{
        node * templ = NULL;

	templ = lnode->children[1];
        change_node(lnode, templ);
        lnode->children[1] = templ->children[LEFT_CHILD];
        if (templ->children[0] != NULL)
        {
                templ->children[0]->parent = lnode;
        }
        templ->children[LEFT_CHILD] = lnode;
        lnode->parent = templ;

}

void rotate_r(node * rnode)
{
        node * temp = NULL;

	temp = rnode->children[0];
        change_node(rnode, temp);
        rnode->children[0] = temp->children[RIGHT_CHILD];
        if (temp->children[RIGHT_CHILD]!= NULL)
        {
                temp->children[1]->parent = rnode;
        }
        temp->children[1] = rnode;
        rnode->parent = temp;

}


node * lookup(void * base)
{
	/*
	 * Just calls the recursive function
	 * on the root of the tree.
	 */
	if(root == NULL)
	{
		printf("root is null\n");
		exit(EXIT_FAILURE);
	}

	return lookup_r(base, root);
}

node * lookup_r(void * base, node * parent)
{
	/*
	 * return the parent node if it's within the given base
	 * and bounds!
	 */

	/* Sanity check. We shouldn't get a null node, but if we do, we can't find the node! */
	if(parent == NULL)
	{
		return NULL;
	}
	
	else if(parent->base == NULL)
	{
		printf("bad node encountered.");
		return NULL;
	}

	else if((parent->base == base))
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

node * contained_lookup(void * base, size_t bounds)
{
	return contained_lookup_r(base, bounds, root);
}

node * contained_lookup_r(void * base, size_t bounds, node * parent)
{
	/*
	 * If we somehow get a null node, return NULL.
	 */
	if(parent == NULL)
	return NULL;

	/*
	 * If the current node's base is too small, look at the right child.
	 */
	/*
	else if(((long)(parent->base) + parent->bounds) < (long)base)
	{
		
		if(parent->children[RIGHT_CHILD] != NULL)
		return contained_lookup_r(base, bounds, parent->children[RIGHT_CHILD]);
		else
		return NULL;
	}
	*/
	/*
	 * If the current node's base is too big, look at the left child.
	 */
	/*
	else if(((long)(base) + bounds) < (long)parent->base)
	{
		if(parent->children[LEFT_CHILD] != NULL)
		return contained_lookup_r(base, bounds, parent->children[LEFT_CHILD]);
		else
		return NULL;
	}
	*/
	/*
	 * If our node's base is in range, check the size and return if it's small enough and free.
	 * Otherwise, check both children if the exist.
	 */
	else if(parent->base > base && ((long)parent->base + parent->bounds) < ((long)base + bounds) && (parent->free == 1))
	{
		return parent;
	}

	/*
	 * Here we search both children, and return one of the results.
	 * We prefer the left child arbitrarily.
	 */
	else
	{
		node * left_return = NULL;
		node * right_return = NULL;

		if(parent->children[LEFT_CHILD] != NULL)
		left_return = contained_lookup_r(base, bounds, parent->children[LEFT_CHILD]);
		
		if(parent->children[RIGHT_CHILD] != NULL)
		right_return = contained_lookup_r(base, bounds, parent->children[RIGHT_CHILD]);

		if(left_return != NULL)
		return left_return;
		else
		return right_return;

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

	/*DEBUG - print the tree here. we're lazy.
	print(root, 0); */


	/*
	 * And now, we clean up our messy tree!
	 */

	clean_tree_return = clean_tree(temp);
	if(clean_tree_return < 0)
	{
		printf("Error on clean_tree return!\n");
		return clean_tree_return;
	}

	/*print the node we've added.
	printf("red = %d node at %p with base %p size %i, parent %p, and children %p %p\n",temp->red, (void *)temp ,temp->base, (int)temp->bounds, (void *)temp->parent, (void *)temp->children[LEFT_CHILD], (void *)temp->children[RIGHT_CHILD]);
	*/


	/*
	 * Return 1 on a success.
	 */
	return 1;

}

int insert_r(void * base, size_t bounds, node * parent, node * temp)
{
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
			return 1;
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
			return insert_r(base, bounds, parent->children[RIGHT_CHILD], temp);
		}
		else
		{
			parent->children[RIGHT_CHILD] = temp;
			temp->parent = parent;
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
		if(parent->children[LEFT_CHILD] != NULL)
		{
			return insert_r(base, bounds, parent->children[LEFT_CHILD], temp);
		}
		else
		{
			parent->children[LEFT_CHILD] = temp;
			temp->parent = parent;
			return 1;
		}
	}




	return 0;
}

int clean_tree(node * child)
{

	node * tempNode;
	node * tempGparent;
	int whichChild = 0;
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
		/*	printf("red = %d node at %p with base %p size %i, parent %p, and children %p %p\n",root->red, (void *)root,root->base, (int)root->bounds, (void *)root->parent, (void *)root->children[LEFT_CHILD], (void *)root->children[RIGHT_CHILD]);

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

			/*
			 * Also need to find which child the grandparent was! 0 is left, 1 is right
			 * This defaults to 0, so just set to 1 if it's a right child.
			 */
			if(child->parent->parent->parent->children[RIGHT_CHILD] == child->parent->parent)
			whichChild = 1;


			/*set the grandparent to the child's right child. Then we set the gparent's (now the child's child) left child to NULL and parent to child */
			child->parent->parent->children[0] = child->children[1]; 			
			child->children[1] = child->parent->parent;
			child->children[1]->parent = child;
			if(child->children[1]->children[0] != NULL)
			child->children[1]->children[0]->parent = child->children[1];

			/*set the parent to the child's left child. Then we set the parents right child to NULL and it's parent to the child*/
			child->parent->children[1] = child->children[0];
			child->children[0] = child->parent;
			child->children[0]->parent = child;
			if(child->children[0]->children[1] != NULL)
			child->children[0]->children[1]->parent = child->children[0];
			
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
				tempNode->children[whichChild] = child;

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

			/*
			 * Also need to find which child the grandparent was! 0 is left, 1 is right
			 * This defaults to 0, so just set to 1 if it's a right child.
			 */
			if(child->parent->parent->parent->children[RIGHT_CHILD] == child->parent->parent)
			whichChild = 1;

			
			/*set the grandparent to the child's left child. Then we set the gparent's (now the child's child) right child to NULL and parent to child */
			child->parent->parent->children[1] = child->children[0]; 
			child->children[0] = child->parent->parent;
			child->children[0]->parent = child;
			if(child->children[0]->children[1] != NULL)
			child->children[0]->children[1]->parent = child->children[0];


			/*set the parent to the child's right child. Then we set the parents left child to NULL and it's parent to the child*/

			child->parent->children[0] = child->children[1];
			child->children[1] = child->parent;
			child->children[1]->parent = child;
			if(child->children[1]->children[0] != NULL)
			child->children[1]->children[0]->parent = child->children[1];

			
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
				tempNode->children[whichChild] = child;


			}
			return 1;
		}
	}

	return 0;
}

int delete_node (void * base)
{
        node * child = NULL;
        node * temp = NULL;
        temp = lookup(base);
	printf("Deleting node at %p\n", (void *)temp);
        if (temp == NULL)
        {
                printf("You cannot delete a node for a base that is not in the tree.");
        }
        /*If node to be deleted has 2 children, give that node the value of its in order
 *          *predecessor. We will then delete that node.
 *                   */
        if (temp->children[LEFT_CHILD] != NULL && temp->children[RIGHT_CHILD] != NULL)
        {
                node * predecessor = NULL;
                predecessor = temp;
                while (predecessor->children[RIGHT_CHILD] != NULL)
                {
                        predecessor = predecessor->children[RIGHT_CHILD];
                }
                temp->base = predecessor->base;
                temp->bounds = predecessor->bounds;                
                temp = predecessor;
        }
        /*creating child node. We will repalce our temporary node with it later*/
        if (temp->children[LEFT_CHILD] == NULL)
        {
                child = temp->children[RIGHT_CHILD];
        }

        if (temp->children[RIGHT_CHILD] == NULL)
        {
                child = temp->children[LEFT_CHILD];
        }

        /*If the node we are now deleting (temp) is red, we are finished. If it is black, we have to 
 *          *rearrange the tree*/
        if (temp->red == 0)
        {
                temp->red = child->red;
                delete_rearrangement(temp);
        }

	change_node(temp, child);
	free(temp);
	return 1;
}



void delete_rearrangement(node * node)
{
        /*if the node has become the root, we are fine.*/

        if (node->parent == NULL)
        {
                return;
        }
	/*In these next two cases (mirror images of each other), node has a red sibling. We will change the
	 *color of n's sibling and rotate around the parent of node. This will not fix the problem, but instead
	 * it makes it so we can fix it later*/
        if(node->parent->children[0] == node)
        {
		if (node->parent->children[1]->red == 1)
		{
			node->parent->children[1]->red = 0;
			node->parent->red = 1;
			rotate_l(node->parent);
		}
        
        }

	/*As mentioned above, this is a mirror image of the previous case*/
	if(node->parent->children[1] == node)
        {
		if (node->parent->children[0]->red == 1)
		{
			node->parent->children[0]->red = 0;
			node->parent->red = 1;
			rotate_r(node->parent);
		}
        
        }
	
	/*The next two cases are mirror imgages of each other. If node's parent, sibling and siblings kids
	 *are all black, we color it's sibling red and run a recursive call on N's parent*/
	if(node->parent->children[0] == node)
        {
			if ((node->parent->red == 0) && (node->parent->children[RIGHT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[RIGHT_CHILD]->red == 0))
			{
				node->parent->children[RIGHT_CHILD]->red = 1;
				delete_rearrangement(node->parent);
			}
	}
	/*Mirror image of the previous case*/
	if(node->parent->children[1] == node)
        {
			if ((node->parent->red == 0) && (node->parent->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[RIGHT_CHILD]->red == 0))
			{
				node->parent->children[LEFT_CHILD]->red = 1;
				delete_rearrangement(node->parent);
			}
	}
	/*The node's parent is red, and it's sibling, and sibling's children are black. We simply exchange the
	 * color of the node's*/
	if(node->parent->children[0] == node)
        {
			if ((node->parent->red == 1) && (node->parent->children[RIGHT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[RIGHT_CHILD]->red == 0))
			{
				node->parent->red = 0;				
				node->parent->children[RIGHT_CHILD]->red = 1;
				
			}
	}
	/*Mirror image*/
	if(node->parent->children[1] == node)
        {
			if ((node->parent->red == 1) && (node->parent->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[RIGHT_CHILD]->red == 0))
			{
				node->parent->red = 0;				
				node->parent->children[LEFT_CHILD]->red = 1;
			}
	}
	/*The node's sibling is black, and it's(the child's sibling's) left child is red and right is black
	 *We switch the color of the sibling and and its left sibling and rotate_r*/
	if(node->parent->children[0] == node)
        {
			if ((node->parent->children[RIGHT_CHILD]->red == 0) && (node->parent->children[RIGHT_CHILD]->children[LEFT_CHILD]->red == 1) && (node->parent->children[RIGHT_CHILD]->children[RIGHT_CHILD]->red == 0))
			{
				node->parent->children[1]->red = 1;
				node->parent->children[1]->children[LEFT_CHILD]->red = 0;
				rotate_r(node->parent->children[1]);
				delete_rearrangement(node);
			}
	}
	/*Mirror Image*/
	if(node->parent->children[1] == node)
        {
			if ((node->parent->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[LEFT_CHILD]->red == 0) && (node->parent->children[LEFT_CHILD]->children[RIGHT_CHILD]->red == 1))
			{
				node->parent->children[0]->red = 1;
				node->parent->children[0]->children[RIGHT_CHILD]->red = 0;
				rotate_l(node->parent->children[0]);
				delete_rearrangement(node);
			}
	}
	/*The node's sibling is balck and that's sibling's right child is red. In this case the node
	 *to be deleted is the left child of it's parent. We switch the color of our nodes parent
	 * and our nodes sibling and rotate_l at our node's parent.*/
	if((node->parent->children[0] == node) && (node->parent->children[1]->red == node->red))
        {
		node->parent->red = 0;
		node->parent->children[1]->red = 0;
		rotate_l(node->parent);	
	}
	/*mirror image of above*/
	if((node->parent->children[1] == node) && (node->parent->children[0]->red == node->red))
        {
		node->parent->red = 0;
		node->parent->children[0]->red = 0;
		rotate_r(node->parent);	
	}
}
/*Change node takes a node and removes the connections from it's parent, replacing it with
 *the new node*/
void change_node(node * old, node * new)
{
        if (old->parent == NULL)
        {
                root = new;
        }
        else
        {
                if(old == old->parent->children[LEFT_CHILD])
                {
                        old->parent->children[LEFT_CHILD] = new;
                        
                }

                else
                {
                        old->parent->children[RIGHT_CHILD] = new;
                }
        }
        if (new != NULL)
        {
                new->parent = old->parent;
        }
}

node * create(void * base, size_t bounds)
{
	node * temp;
	temp = malloc(sizeof(node));
	temp->base = base;
	temp->bounds = bounds;
	temp->red = 1;
	temp->free = 0;
	temp->parent = NULL;
	temp->children[LEFT_CHILD] = NULL;
	temp->children[RIGHT_CHILD] = NULL;
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

	printf("red = %d, free = %d node at %p with base %p size %i, parent %p, and children %p %p\n",root->red, root->free, (void *)root,root->base, (int)root->bounds, (void *)root->parent, (void *)root->children[LEFT_CHILD], (void *)root->children[RIGHT_CHILD]);

	if(root->children[RIGHT_CHILD] != NULL)
	print(root->children[RIGHT_CHILD], depth + 1);
}

void print_func()
{
	print(root, 0);
}
