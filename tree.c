#include "tree.h"

int symbol_compare(char *word, tree *root)
{
    return strncmp(word, root->symbol, strlen(word));
}

int height(tree * root)
{
    if (root != NULL)
    {
        return root->height;
    }
    else
    {
        return 0;
    }
}

int max(int a, int b)
{
    if (a > b) { return a; }
    else { return b; }
}

tree * right_rotate(tree * root)
{
    tree * x = root->left;
    tree * tmp = x->right;

    x->right = root;
    root->left = tmp;

    root->height = max(height(root->left), height(root->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    return x;
}

tree * left_rotate(tree * root)
{
    tree * y = root->right;
    tree * tmp = y->left;

    y->left = root;
    root->left = tmp;

    root->height = max(height(root->left), height(root->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    return y;
}

int get_balance(tree * root)
{
    if (root != NULL)
    {
        return height(root->left) - height(root->right);
    }
    else
    {
        return 0;
    }
}

struct tree *insert(tree * root, char *symbol, char *name, signed long cents)
{
    if (root == NULL)
    {
        root = (tree *) malloc(sizeof(tree));

        if (root == NULL)
        {
            printf("Memory Error\n");
            return NULL;
        }
        else
        {
            root->symbol = symbol;
            root->name = name;
            root->cents = cents;
            root->left = root->right = NULL;
            root->visited = false;
            root->height = 1;
        }
    }
    else
    {
        int cmpValue = 0;
        cmpValue = symbol_compare(symbol, root);

        if (cmpValue == 0)
        {
            root->cents += cents;
            if(name)
            {
                free(root->name);
                root->name = name;
            }
            free(symbol);
        }
        else if (cmpValue < 0)
        {
            root->left = insert(root->left, symbol, name, cents);
        }
        else
        {
            root->right = insert(root->right, symbol, name, cents);
        }
    }
    int balance = get_balance(root);

    //Zig Zig
    if (balance > 1 && (symbol_compare(symbol, root->left) < 0))
    {
        return right_rotate(root);
    }
    //Zag Zag
    if (balance < -1 && (symbol_compare(symbol, root->right) > 0))
    {
        return left_rotate(root);
    }
    //Zig Zag
    if (balance > 1 && (symbol_compare(symbol, root->left) > 0))
    {
        root->left = left_rotate(root->left);
        return right_rotate(root);
    }
    //Zag Zig
    if (balance < -1 && (symbol_compare(symbol, root->right) < 0))
    {
        root->right = right_rotate(root->right);
        return left_rotate(root);
    }

    return root;
}

void tree_destroy(tree * root)
{
    if(root->symbol)
    {
        free(root->symbol);       
    }
    if(root->name)
    {
        free(root->name);
    }
    if(root->left)
    {
        tree_destroy(root->left);
    }
    if(root->right)
    {
        tree_destroy(root->right);
    }

    free(root);
}

struct tree *value_tree_insert(tree * root, tree * node)
{
    if (root == NULL)
    {
        root = (tree *) malloc(sizeof(tree));

        if (root == NULL)
        {
            printf("Memory Error\n");
            return NULL;
        }
        else
        {
            root->symbol = node->symbol;
            root->name = node->name;
            root->cents = node->cents;
            node->symbol = NULL;
            node->name = NULL;
            root->left = root->right = NULL;
        }
    }
    else
    {
        if(node->cents < root->cents)
        {
            root->left = value_tree_insert(root->left, node);
        }
        else
        {
            root->right = value_tree_insert(root->right, node);
        }
    }
    return root;    
}

//I need to grab every node in a tree, and insert it into a new tree

struct tree * dismantle(tree * root)
{
    if(root->left && !root->left->visited)
    {
        return dismantle(root->left);
    }
    else if(root->right && !root->right->visited)
    {
        return dismantle(root->right);
    }
    else if(!root->visited)
    {
        root->visited = true;
        return root;
    }
    return NULL;
}

struct tree * sort_tree(tree * root)
{
    tree * newTree = NULL;
    tree * tmpTree = NULL;

    while(true)
    {
        tmpTree = dismantle(root);
        if(tmpTree)
        {
            newTree = value_tree_insert(newTree, tmpTree);      
        }
        else
        {
            break;
        }
    }
    return newTree;
}

void in_order_print(tree * root)
{
    if(root == NULL)
    {
        return;
    }
    if(root->left)
    {
        in_order_print(root->left);
    }
    if (root->cents >= 0)
    {
        printf("%s %ld.%ld", root->symbol, root->cents/100, root->cents%100);
        if(root->name)
        {
            printf(" %s\n", root->name);
        }
        else
        {
            printf("\n");
        }
    }
    if(root->right)
    {
        in_order_print(root->right);
    }
}
