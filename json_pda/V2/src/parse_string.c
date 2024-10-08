#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse_aux.h"
#include "json_types.h"
#include "parse_string.h"
int
parse_string(
    char **ptr_X,
    size_t *ptr_nX,
    json_t *ptr_J
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *bak_X = *ptr_X; 
  char *X = *ptr_X; size_t nX = *ptr_nX;

  if ( nX < 2 ) { go_BYE(-1); }
  // consume opening dquote 
  if ( *X != '"' ) { go_BYE(-1); } X++; nX--;
  for ( ; ; ) { 
    if ( nX == 0 ) { go_BYE(-1); } 
    if ( *X == '"' ) { // consume closing dquote 
      X++; nX--; 
      break;
    }
    if ( *X == '\\' ) { go_BYE(-1); } // not handling escapes
    if ( !isascii(*X) ) { go_BYE(-1); }
    X++; nX--; // consume another character in string 
  }
  size_t sz = X - bak_X + 1;
  ptr_J->jtype = j_string;
  ptr_J->data.str_val = malloc(sz);
  memset(ptr_J->data.str_val, 0,  sz);
  strncpy(ptr_J->data.str_val, bak_X, sz-1);

  *ptr_X = X; *ptr_nX = nX;
  printf("string\n"); 

BYE:
  return status;
}
