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
#define LEFT_CHILD 0
#define RIGHT_CHILD 1

typedef struct node
{
	struct node * parent;
	struct node * children[2];
	void * base;
	size_t bounds;
	int free;
	int red;
}node;

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
 * Does a standard BST insert on our new node, and then
 * fixes the tree for red-black tree properties.
 * Internal to the rbtree stuff.
 */
int insert_r(void * base, size_t bounds, node * parent, node * temp);

/*
 * Does all of the cleanup work after an insert.
 */
int clean_tree(node * child);

/* 
 * Another internal function!
 * Returns a red, unfreed node with the specified
 * base and bounds.
 */

node * create(void * base, size_t bounds);

/*
 * Print the current tree.
 */
void print(node * root, int depth);
