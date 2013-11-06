/*
 * rbtree.h
 * Header for red-black tree.
 * Contains structs for the tree and the nodes,
 * and methods to find/create nodes.
 */
#include <sys/types.h>

/*
 * Use these constants to get which child you want!
 */
const int LEFT_CHILD = 0;
const int RIGHT_CHILD = 1;

typedef struct node
{
	node * parent;
	node * children[2];
	size_t base;
	size_t bounds;
	int free;
}

typedef struct tree
{
	node * root;
}

/*
 * Finds a node within the base and its bounds.
 */
node * lookup(size_t base, size_t bounds);

/*
 * Recursive function called by lookup - shouldn't be
 * user code!
 */
node * lookup_r(size_t base, size_t bounds, node * parent);

/* Inserts a node into the tree. Self-balancing!
 * Pass in the node's base and bounds.
 * Creates a new node if there's not a node at that base.
 * If there is a node there, and it's been freed,
 * update that node.
 * If the node is there and it hasn't been freed,
 * return an error.
 */
int insert(size_t base, size_t bounds);
