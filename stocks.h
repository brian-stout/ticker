#ifndef STOCKS_H
#define STOCKS_H


#include <stdio.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

#include "tree.h"

/** file_input() takes lines from a file and puts them through input_validation()
*/
tree * file_input(tree * root, FILE * fp);

/** user_input() takes lines from stdin and puts them through input_validation()
*/
tree * user_input(tree * root);

/** input_validation() is a function which receives ticker update lines and parses them for bad
*       information.  If the information is bad it spits out the error and returns the passed tree
*       unmodified.  If the input is good it splits it up in to the correct data types and passes.
*       that data to the insert function and returns the modified tree.
*/
tree * input_validation(tree * root, char *buf);

/** is_symbol_bad() checks the first word of the line passed by file_input() or user_input() and
*       checks to see if it's a valid symbol.  It lets input_validation() know if it should quit out.
*/
bool is_symbol_bad(char *symbol);

#endif
