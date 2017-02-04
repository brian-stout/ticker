#ifndef TREE_H
#define TREE_H

#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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

void in_order_print(tree * root);
struct tree * sort_tree(tree * root);
struct tree * dismantle(tree * root);
struct tree *value_tree_insert(tree * root, tree * node);
void tree_destroy(tree * root);
struct tree *insert(tree * root, char *symbol, char *name, signed long cents);
int get_balance(tree * root);
tree * left_rotate(tree * root);
tree * right_rotate(tree * root);
int max(int a, int b);
int height(tree * root);
int symbol_compare(char *word, tree *root);

#endif
