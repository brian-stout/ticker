#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

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
            root->left = root->right = NULL;
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

    printf("%s %zd", root->symbol, root->cents);
    if(root->name)
    {
        printf(" %s\n", root->name);
    }
    else
    {
        printf("\n");
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
        printf("Error: Ticker symbol is too long\n");
        ret = true;
        return ret;
    }
    for(size_t i = 0; i < wordLen; ++i)
    {
        if(isupper(symbol[i]) == 0)
        {
            printf("Error: ticker symbol should only contain uppercase alphabet!\n");
            ret = true;
            break;
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


    token = strtok(buf, " ");
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

    token = strtok(NULL, " ");
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

    in_order_print(root);

}
