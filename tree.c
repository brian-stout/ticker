#include "tree.h"

int symbol_compare(char *word, tree *root)
{
    //Returns alphabetical order
    return strncmp(word, root->symbol, strlen(word));
}

int height(tree * root)
{
    //Function just grabs the height and returns it
    //Used in the AVL portion of the tree
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
    //Don't want to import math.h just for this
    if (a > b) { return a; }
    else { return b; }
}

tree * right_rotate(tree * root)
{
    //Grabbing the left to move it
    tree * leftChild = root->left;

    //Storing the right of the left to move it
    tree * subTree = leftChild->right;

    //Doing the rotation
    leftChild->right = root;
    root->left = subTree;

    //Updating the heights
    root->height = max(height(root->left), height(root->right)) + 1;
    leftChild->height = max(height(leftChild->left), height(leftChild->right)) + 1;

    //Returning the new root
    return leftChild;
}

tree * left_rotate(tree * root)
{
    //Exactly the the same as up except we grab the right child and it's subTree
    tree * rightChild = root->right;
    tree * subTree = rightChild->left;

    rightChild->left = root;
    root->left = subTree;

    root->height = max(height(root->left), height(root->right)) + 1;
    rightChild->height = max(height(rightChild->left), height(rightChild->right)) + 1;

    return rightChild;
}

int get_balance(tree * root)
{
    //Used to determine if the tree or subtree is unbalanced by comparing the max height of
    //  the left and right branches

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
    //If we found a spot, create the node here
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

        //If the symbol is the same, we just add the cents and update the name if a name was
        //  provided
        if (cmpValue == 0)
        {
            root->cents += cents;
            //If the name was provided
            if(name)
            {
                //Free the memory for the current string
                free(root->name);
                //Replace with new string
                root->name = name;
            }
            //If the symbol string already exists discard the created one
            free(symbol);
        }
        //Name is smaller than root so go to the left
        else if (cmpValue < 0)
        {
            root->left = insert(root->left, symbol, name, cents);
        }
        //Name is bigger than root so go to the right
        else
        {
            root->right = insert(root->right, symbol, name, cents);
        }
    }
    //See's if the left or right branches are balanced or not
    int balance = get_balance(root);

    //Checks for the types of imbalances and fixes them if they are

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

    //Return the new node
    return root;
}

void tree_destroy(tree * root)
{
    //Free the malloc'd memory for the symbol string
    if(root->symbol)
    {
        free(root->symbol);       
    }
    //Free the malloc'd memory for the name string
    if(root->name)
    {
        free(root->name);
    }
    //Check the left
    if(root->left)
    {
        tree_destroy(root->left);
    }
    //Check the right
    if(root->right)
    {
        tree_destroy(root->right);
    }

    //Free the current node
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
            //This reassign the already existing pointers to the new tree
            //  This is the reason why you have to destroy the new tree before the original one
            //  to prevent free'ing already free'd memory
            root->symbol = node->symbol;
            root->name = node->name;
            root->cents = node->cents;

            //Sets the old tree symbol to NULL so the destroy function knows not to try and free it
            //  twice
            node->symbol = NULL;
            node->name = NULL;
            root->left = root->right = NULL;
        }
    }
    else
    {
        //Sorting by the cents from lowest to greatest
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

struct tree * dismantle(tree * root)
{
    //If there is a left and it hasn't been visited already
    if(root->left && !root->left->visited)
    {   //Step in to the left
        return dismantle(root->left);
    }
    //If there is a right and it hasn't been visited already
    else if(root->right && !root->right->visited)
    {   //Step in to the right
        return dismantle(root->right);
    }
    //If there isn't a left and there isn't a right but it hasn't been visited
    else if(!root->visited)
    {
        //Set the visitation to true and return this node
        root->visited = true;
        return root;
    }
    //If the current node is all these things the entire tree has been visited
    return NULL;
}

struct tree * sort_tree(tree * root)
{
    //The new tree that's going to be returned
    tree * newTree = NULL;

    //This isn't actually a tree but a temp storage for the node returned by the dismantle function
    tree * tmpTree = NULL;

    while(true)
    {
        //Grab a bottom node from the tree
        tmpTree = dismantle(root);

        //Until dismantle returns NULL keep grabbing nodes from the tree
        if(tmpTree)
        {
            //Insert the node in to a new tree sorted by cents
            newTree = value_tree_insert(newTree, tmpTree);      
        }
        else
        {
            break;
        }
    }

    //Returning the new tree
    return newTree;
}

void in_order_print(tree * root)
{
    
    //Avoids crashes
    if(root == NULL)
    {
        return;
    }
    if(root->left)
    {
        //Step in to the left to find the smallest values first
        in_order_print(root->left);
    }

    //Only print out stocks with a positive value
    if (root->cents > 0)
    {
        //Finds the amount of dollars from the cents and puts the remainder after a decimal point
        printf("%s %ld.%ld", root->symbol, root->cents/100, root->cents%100);
        //Checks to see if there is a name before trying to print it to avoid crashes
        if(root->name)
        {
            printf(" %s\n", root->name);
        }
        else
        {
            //Prints out the space
            printf("\n");
        }
    }

    //After the smallest values are printed and the middle values are printed, print out greater
    //  values
    if(root->right)
    {
        in_order_print(root->right);
    }
}
