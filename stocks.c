#include "stocks.h"

bool is_symbol_bad(char *symbol)
{
    bool ret = false;

    size_t wordLen = strlen(symbol);

    //Symbols should be no greater than 5 characters including dashes
    if(wordLen > 5)
    {
        printf("Error: Ticker symbol %s is %zd characters too long\n", symbol, wordLen - 5);
        ret = true;
        return ret;
    }

    //Makes sure all characters in the symbol are upper case or a -
    for(size_t i = 0; i < wordLen; ++i)
    {
        //If it isn't upper case check to see if it's a dash
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

    //Return if the symbol is bad or good so input_validation can handle it better
    return ret;
}

tree * input_validation(tree * root, char *buf)
{
    //Grabs the first word in the buf string
    char *token = NULL;

    //Delimitting by space and by tab just in case user seperated it by a tab character
    token = strtok(buf, " \t");

    //Declare wordLen variable that's going to be used for each token
    size_t wordLen = 0;
    wordLen = strlen(token);

    //Creates a char pointer for the symbol
    char *symbol = NULL;

    //Malloc's enough memory for symbol to contain the first token
    symbol = malloc(sizeof(char) * wordLen + 1);

    //Stops an unintialized value error, and null terminates the string
    symbol[wordLen] = '\0';

    //Copies the characters from the token in to symbol
    symbol = strncpy(symbol, token, wordLen);

    //Runs a function that does validation for symbol, if it is bad it free's the memory
    //  and returns the root as it is
    if (is_symbol_bad(symbol))
    {
        free(symbol);
        return root;
    }

    //Grabbing the next token, the value to be updated
    token = strtok(NULL, " \t");

    //Setting up variables to store the data
    double price = 0;
    signed long cents = 0;

    //Checks to see if the token grab was successful
    if(token)
    {
        //strtod will store any extra garbage at errPtr
        char *errPtr;

        //Converts string in to a double until it'll be assigned to an int for data
        //  consistency
        price = strtod(token, &errPtr);

        //If there's anything in errPtr that means there was things that couldn't be parsed
        if(*errPtr)
        {
            printf("Error:  Invalid stock dollar value\n");
            //Free memory malloc'd for symbol to avoid memory leaks
            free(symbol);
            return root;
        }

        //convert the double to an int for the lowest value, anything after hundredth is dropped
        cents = price * 100;
    }
    //If the token wasn't successful the memory needs to be free'd
    //  also checks to see if user even bothered to input data
    else
    {
        free(symbol);
        printf("Error:  Missing data!\n");
        return root;
    }

    //char pointer which will contain memory for the stock name
    char *name = NULL;

    token = strtok(NULL, "");

    //Checks to see if the user even inputted any data
    if(token)
    {
        wordLen = strlen(token);
        name = malloc(sizeof(char) * wordLen + 1);

        //Stops an unintialized value error, and null terminates the string
        name[wordLen] = '\0';

        //Copies the data in to the pointer
        name = strncpy(name, token, wordLen);
    }
    //If nothing was inputted the data is still valid so the pointer is just set to NULL
    //  and we move on
    else
    {
        name = NULL;
    }

    //Insert a node with all the new data in to the tree
    root = insert(root, symbol, name, cents);

    //Return updated tree
    return root;

}

tree * user_input(tree * root)
{
    char buf[82];

    while(fgets(buf, sizeof(buf), stdin))
    {
        //Prevents the loop from running the function with a NULL
        //  and also stops the loop if the user just hits enter
        if(buf[0] == '\n' || buf[0] == '\0')
        {
            break;
        }
        else
        {
            //If the first character is a # it's a comment so just move on
            if(buf[0] == '#')
            {
                continue;
            }
            //makes sure the line is null terminated
            buf[strlen(buf) - 1] = '\0';

            //Need to chop up the inputted line and grab pieces from it
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
        //Used to catch empty lines
        if(buf[0] == '\n')
        {
            continue;
        }
        buf[strlen(buf) - 1] = '\0';

        //Chopping up the data
        root = input_validation(root, buf);
    }
    return root;
}

