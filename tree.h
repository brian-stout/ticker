#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree
{
    char *symbol;
    char *name;
    signed long cents;
    struct tree *left;
    struct tree *right;
    int height;
    bool visited;
}   tree;

/** in_order_print() Steps in to the tree using a Left, Process, Right model and prints out 
*       all the data ensuring that the nodes are printed out how they were sorted
*/
void in_order_print(tree * root);

/** sort_tree() handles the logic used in conjuction with the returned node from dismantle() and
*       inserts each returned node from a tree in to a new tree using a seperate sort function
*/
struct tree * sort_tree(tree * root);

/** dismantle() steps through each node of a tree and returns nodes until every single node
*       has been returned.  It does this by setting a flag "visited" on nodes it returns
*/
struct tree * dismantle(tree * root);

/** value_tree_insert() sorts nodes by the value in cents as opposed to by the name.
*       This is done to sort a tree to be printed out right after wards which is why it doesn't
*       any form of balancing since it doesn't save time complexity.
*/
struct tree *value_tree_insert(tree * root, tree * node);

/** tree_destroy() free's all the memory inside of a tree.  Modified tree's should be free'd before
*       there originals
*/
void tree_destroy(tree * root);

/** insert() is the main function that handles creating nodes and sorting them based on there symbol
*       name.  The reason it sorts by symbol name is because the key never gets updated, and is the
*       searched for the most frequently.  Insert() also handles most of the AVL tree logic and
*       balances itself on updates.
*/
struct tree *insert(tree * root, char *symbol, char *name, signed long cents);

/** get_balance() is a function called by insert and is used to determine if a tree need balancing
*       or not.  It does this by checking the heights of the left and right sub trees.
*/
int get_balance(tree * root);

/** left_rotate() shifts the nodes in an unbalanced tree by rotating it counter clockwise.
*       used when a tree is imbalanced.
*/
tree * left_rotate(tree * root);

/** right_rotate() is the same as left_rotate() but it does the opposite
*/
tree * right_rotate(tree * root);

/** max() returns the largest value of two.  Quick implementation to avoid importing math.h.
*/
int max(int a, int b);

/** height() returns the height field in the tree structure
*/
int height(tree * root);

/** symbol_compare() is a function wrapper that returns if a string is bigger than the string at root
*/
int symbol_compare(char *word, tree *root);

#endif
