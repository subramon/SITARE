#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse_number.h"
int
parse_number(
    char **ptr_X,
    size_t *ptr_nX
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *X = *ptr_X; size_t nX = *ptr_nX;

  for ( ; ; ) { 
    if ( nX == 0 ) { go_BYE(-1); } 
    if ( isspace(*X) ) { break; }
    if ( !isdigit(*X) ) { break; }
    X++; nX--; // consume another character in string 
  }
  printf("number\n");
  *ptr_X = X; *ptr_nX = nX;

BYE:
  return status;
}
