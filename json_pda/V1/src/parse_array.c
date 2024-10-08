#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse_aux.h"
#include "parse_json.h"
#include "parse_array.h"

int
parse_array(
    char **ptr_X,
    size_t *ptr_nX
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *X = *ptr_X; size_t nX = *ptr_nX;

  printf("start array\n");
  if ( nX < 2 ) { go_BYE(-1); }
  // consume opening square bracket
  if ( *X != '[' ) { go_BYE(-1); } X++; nX--;
  for ( int i = 0; ; i++ ) { 
    status = skip_white_space(&X, &nX); cBYE(status);
    if ( nX == 0 ) { go_BYE(-1); }
    if ( *X == ']' ) { X++; nX--; break; }
    if ( ( i > 0 ) && ( *X == ','  ) ) { X++; nX--; }
    *ptr_X = X; *ptr_nX = nX; 
    status = parse_json(ptr_X, ptr_nX); cBYE(status);
    X = *ptr_X; nX = *ptr_nX;
  }
  printf("stop array\n");
  *ptr_X = X; *ptr_nX = nX;

BYE:
  return status;
}
