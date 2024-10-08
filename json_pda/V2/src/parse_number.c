#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "json_types.h"
#include "txt_to_F8.h"
#include "parse_number.h"

int
parse_number(
    char **ptr_X,
    size_t *ptr_nX,
    json_t *ptr_J
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *bak_X = *ptr_X; char *buf = NULL;
  char *X = *ptr_X; size_t nX = *ptr_nX;

  for ( ; ; ) { 
    if ( nX == 0 ) { go_BYE(-1); } 
    if ( isspace(*X) ) { break; }
    if ( !isdigit(*X) ) { break; }
    X++; nX--; // consume another character in string 
  }
  printf("number\n");
  size_t sz = X - bak_X + 1;
  ptr_J->jtype = j_number;
  buf = malloc(sz);
  memset(buf, 0,  sz);
  strncpy(buf, bak_X, sz-1);
  status = txt_to_F8(buf, &(ptr_J->data.num_val)); cBYE(status);

  *ptr_X = X; *ptr_nX = nX;

BYE:
  free_if_non_null(buf);
  return status;
}
