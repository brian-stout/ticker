#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "stocks.h"

int main(int argc, char *argv[])
{

    //Checks to see if user inputted too many options in the command line
    if (argc > 2)
    {
        printf("Error, Usage: ./ticker [Stock document]\n");
        return 0;
    }

    tree *root = NULL;

    //If the user did input an option try to open it as a file
    if (argc == 2)
    {
        FILE * fp = fopen(argv[1], "r");

        //If the file fails to open quit out the program
        if(!fp)
        {
            printf("Error: Invalid file!\n");
            return 0;
        }

        //If the file successfully opened, parse it for data
        root = file_input(root, fp);
        fclose(fp);
    }

    //Ask the user for inputs
    root = user_input(root);

    //Checks to see if any data was successfully inputted, if not none of the remaining program
    //  will be effective, so quit out.
    if(!root)
    {
        printf("No data!\n");
        return 0;
    }

    //New pointer for the sorted tree
    tree *newTree = NULL;

    //Takes all the nodes from root and sorts them by there cents value
    newTree = sort_tree(root);

    //Print out the new sorted tree
    in_order_print(newTree);

    //Destroy's must be done in this order
    tree_destroy(newTree);
    tree_destroy(root);

}
