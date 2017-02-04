#include <stdio.h>
#include <stdlib.h>

#include "tree.h"
#include "stocks.h"

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
