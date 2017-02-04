#ifndef STOCKS_H
#define STOCKS_H


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"

tree * file_input(tree * root, FILE * fp);
tree * user_input(tree * root);
tree * input_validation(tree * root, char *buf);
bool is_symbol_bad(char *symbol);

#endif
