#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse_aux.h"

int
skip_white_space(
    char **ptr_X, 
    size_t *ptr_nX
    )
{
  int status = 0;
  if ( ptr_X == NULL ) { go_BYE(-1); }
  if ( ptr_nX == NULL ) { go_BYE(-1); }
  char *X = *ptr_X;
  size_t nX = *ptr_nX;
  if ( X == NULL ) { go_BYE(-1); }

  for ( ; ; ) { 
    if ( nX == 0 ) { break; }
    if ( !isspace(*X) ) { break; }
    X++;
    nX--;
  }
  *ptr_X = X;
  *ptr_nX = nX;


BYE:
  return status;
}
