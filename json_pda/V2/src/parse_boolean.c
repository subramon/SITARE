#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse_aux.h"
#include "json_types.h"
#include "parse_boolean.h"
int
parse_boolean(
    char **ptr_X,
    size_t *ptr_nX,
    json_t *ptr_J
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *X = *ptr_X; size_t nX = *ptr_nX;
  bool bval;

  if ( nX < 4 ) { 
    go_BYE(-1); 
  }
  else if ( nX == 4 ) {
    if ( strncmp(X, "true", strlen("true")) == 0 ) { 
      // SUCCESS
      X  += strlen("true"); nX -= strlen("true");
      bval = true;
    }
    else {
      go_BYE(-1);
    }
  }
  else {
    if ( strncmp(X, "true", strlen("true")) == 0 ) {
      // SUCCESS
      X  += strlen("true"); nX -= strlen("true");
      bval = true;
    }
    else if ( strncmp(X, "false", strlen("false")) == 0 ) {
      // SUCCESS
      X  += strlen("false"); nX -= strlen("false");
      bval = false;
    }
    else {
      go_BYE(-1);
    }
  }
  memset(ptr_J, 0, 1 * sizeof(json_t));
  ptr_J->jtype = j_boolean;
  ptr_J->data.bval = bval;

  *ptr_X = X; *ptr_nX = nX;
  printf("bool\n"); 

BYE:
  return status;
}
