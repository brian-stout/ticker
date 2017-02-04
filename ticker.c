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
    bool visited;
}   tree;

int symbol_compare(tree *root, char *word)
{
    return strncmp(word, root->symbol, strlen(word));
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
        }
    }
    else
    {
        int cmpValue = 0;
        cmpValue = symbol_compare(root, symbol);

        if (cmpValue == 0)
        {
            root->cents += cents;
            if(name)
            {
                free(root->name);
                root->name = name;
                free(symbol);
            }
        }
        else if (cmpValue < 1)
        {
            root->left = insert(root->left, symbol, name, cents);
        }
        else
        {
            root->right = insert(root->right, symbol, name, cents);
        }
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

bool is_symbol_bad(char *symbol)
{
    bool ret = false;

    size_t wordLen = strlen(symbol);
    if(wordLen > 5)
    {
        printf("Error: Ticker symbol %s is %zd characters too long\n", symbol, wordLen - 5);
        ret = true;
        return ret;
    }
    for(size_t i = 0; i < wordLen; ++i)
    {
        if(isupper(symbol[i]) == 0)
        {
            if(symbol[i] != '-')
            {
                printf("Error: ticker symbol should only contain uppercase alphabet! or dashes\n");
                ret = true;
                break;
            }
        }
    }
    return ret;
}

tree * input_validation(tree * root, char *buf)
{
    char *token = NULL;
    char *symbol = NULL;
    size_t wordLen = 0;
    char *errPtr;
    double price = 10;
    size_t cents = 0;
    char *name = NULL;


    token = strtok(buf, " \t");
    wordLen = strlen(token);
    symbol = malloc(sizeof(char) * wordLen + 1);

    //Stops an unintialized value error, and null terminates the string
    symbol[wordLen] = '\0';
    symbol = strncpy(symbol, token, wordLen);
    if (is_symbol_bad(symbol))
    {
        free(symbol);
        return NULL;
    }

    token = strtok(NULL, " \t");
    if(token)
    {
        price = strtod(token, &errPtr);
        if(*errPtr)
        {
            printf("Error:  Invalid stock dollar value\n");
            return NULL;
        }
        
        cents = price * 100;
    }
    else
    {
        free(symbol);
        printf("Error:  Missing data!\n");
        return NULL;
    }


    token = strtok(NULL, "");
    if(token)
    {
    wordLen = strlen(token);
    name = malloc(sizeof(char) * wordLen + 1);

    //Stops an unintialized value error, and null terminates the string
    name[wordLen] = '\0';
    name = strncpy(name, token, wordLen);
    }
    else
    {
        name = NULL;
    }

    root = insert(root, symbol, name, cents);
    return root;

}

tree * user_input(tree * root)
{
    char buf[82];

    while(fgets(buf, sizeof(buf), stdin))
    {
        if(buf[0] == '\n' || buf[0] == '\0')
        {
            break;
        }
        else
        {
            buf[strlen(buf) - 1] = '\0';
            root = input_validation(root, buf);
        }
    }
    return root;
}

tree * file_input(tree * root, FILE * fp)
{
    char buf[82];

    while(fgets(buf, sizeof(buf), fp))
    {
        if(buf[0] == '\n')
        {
            continue;
        }
        buf[strlen(buf) - 1] = '\0';
        root = input_validation(root, buf);
    }
    return root;
}

int main(int argc, char *argv[])
{
    if (argc > 2)
    {
        printf("Error, Usage: ./ticker [Stock document]\n");
        return 0;
    }

    tree *root = NULL;

    if (argc == 2)
    {
        FILE * fp = fopen(argv[1], "r");
        root = file_input(root, fp);
        fclose(fp);
    }

    root = user_input(root);

    if(!root)
    {
        printf("No data!\n");
        return 0;
    }

    tree *newTree = NULL;

    newTree = sort_tree(root);
    in_order_print(newTree);

    //Destroy's must be done in this order
    tree_destroy(newTree);
    tree_destroy(root);

}
