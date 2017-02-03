#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>

typedef struct tree
{
    char *symbol;
    char *name;
    size_t cents;
    struct tree *left;
    struct tree *right;
}   tree;

int symbol_compare(tree *root, char *word)
{
    return strncmp(word, root->symbol, strlen(word));
}

struct tree *insert(tree * root, char *symbol, char *name, size_t cents)
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
        }
    }
    else
    {
        int cmpValue = 0;
        cmpValue = symbol_compare(root, symbol);

        if (cmpValue < 1)
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

    printf("%s %zd %s\n", root->symbol, root->cents, root->name);

    if(root->right)
    {
        in_order_print(root->right);
    }
}

tree * input_validation(char *buf)
{
    tree *root = NULL;
    char *token = NULL;
    char *symbol = NULL;
    size_t wordLen = 0;
    char priceBuf[12];
    double price = 10;
    char *name = NULL;


    token = strtok(buf, " ");
    wordLen = strlen(token);
    symbol = malloc(sizeof(char) * wordLen + 1);

    //Stops an unintialized value error, and null terminates the string
    symbol[wordLen] = '\0';
    symbol = strncpy(symbol, token, wordLen);
    printf("%s\n", symbol);
    free(symbol);

    token = strtok(NULL, " ");
    wordLen = strlen(token);
    priceBuf = strncpy(priceBuf, token, wordLen);
    printf("%s\n", priceBuf);

    token = strtok(NULL, "");
    wordLen = strlen(token);
    name = malloc(sizeof(char) * wordLen + 1);

    //Stops an unintialized value error, and null terminates the string
    name[wordLen] = '\0';
    name = strncpy(name, token, wordLen);
    printf("%s\n", name);
    free(name);
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
            root = input_validation(buf);
        }
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
    if (argc == 2)
    {
        printf("Doing a file %s thing here\n", argv[1]);
    }

    printf("Regular program\n");
    //AAAAA +1000000.00  (+64 chars for company name)

    tree *root = NULL;

    root = user_input(root);

}
