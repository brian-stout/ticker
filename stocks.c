#include "stocks.h"

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
        return root;
    }

    token = strtok(NULL, " \t");
    if(token)
    {
        price = strtod(token, &errPtr);
        if(*errPtr)
        {
            printf("Error:  Invalid stock dollar value\n");
            return root;
        }
        
        cents = price * 100;
    }
    else
    {
        free(symbol);
        printf("Error:  Missing data!\n");
        return root;
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
            if(buf[0] == '#')
            {
                continue;
            }
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

