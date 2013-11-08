/*
 * rbtree.h
 * Header for red-black tree.
 * Contains structs for the tree and the nodes,
 * and methods to find/create nodes.
 *
 * Implementation based on, but not copied from
 * http://www.cs.auckland.ac.nz/software/AlgAnim/red_black.html
 * and
 * http://videolectures.net/mit6046jf05_demaine_lec10/
 *
 */
#include <sys/types.h>

/*
 * Use these constants to get which child you want!
 */
extern const int LEFT_CHILD = 0;
extern const int RIGHT_CHILD = 1;

typedef struct node
{
	node * parent;
	node * children[2];
	void * base;
	size_t bounds;
	int free;
	int red;
}

/*
 * Finds a node with a given base.
 * Returns null for a non-existant node!
 */
node * lookup(void * base);

/*
 * Recursive function called by lookup - shouldn't be
 * user code!
 */
node * lookup_r(void * base, node * parent);

/*
 * Check if a given base address is contained in a node,
 * returns the node if the node is active (not freed) and
 * and the address is contained in its base/bounds.
 * Ignores free nodes.
 */
node * bounds_lookup(void * base);

/*
 * Recursive function for bounds_lookup.
 */
node * bounds_lookup_r(void * base, node * parent);


/* 
 * Inserts a node into the tree. Self-balancing!
 * Pass in the node's base and bounds.
 * Creates a new node if there's not a node at that base.
 * If there is a node there, and it's been freed,
 * update that node.
 * If the node is there and it hasn't been freed,
 * return an error.
 */
int insert(void * base, size_t bounds);

/* 
 * This is the actual recursive function that does the insert!
 * Shouldn't be called by user code - so we don't need to specify the
 * parent node in the program! Just some magic!
 */
int insert_r(void * base, size_t bounds, node * parent);

/* 
 * Another internal function!
 * Returns a red, unfreed node with the specified
 * base and bounds.
 */
node * create(void * base, size_t bounds);

/*
 * Function to left-rotate a node and its child.
 */
void rotate_l(node * parent);

/*
 * Function to right-rotate a node and its child.
 */
void rotate_r(node * parent);

/*
 * Function to do a color-swap on a node and its children.
 */
void color_swap(node * parent);
