#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "json_types.h"
#include "parse_null.h"

int
parse_null(
    char **ptr_X,
    size_t *ptr_nX,
    json_t *ptr_J
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *X = *ptr_X; size_t nX = *ptr_nX;

  if ( nX < 4 ) { 
    go_BYE(-1); 
  }
  else {
    if ( strncmp(X, "null", strlen("null")) == 0 ) { 
      // SUCCESS
      X  += strlen("null"); nX -= strlen("null");
    }
    else {
      go_BYE(-1);
    }
  }
  memset(ptr_J, 0, 1 * sizeof(json_t));
  ptr_J->jtype = j_null;

  *ptr_X = X; *ptr_nX = nX;

BYE:
  return status;
}
