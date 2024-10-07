#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse_aux.h"
#include "parse_boolean.h"
int
parse_boolean(
    char **ptr_X,
    size_t *ptr_nX
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *X = *ptr_X; size_t nX = *ptr_nX;

  if ( nX < 4 ) { 
    go_BYE(-1); 
  }
  else if ( nX == 4 ) {
    if ( strncmp(X, "true", strlen("true")) == 0 ) { 
      // SUCCESS
      X  += strlen("true"); nX -= strlen("true");
    }
    else {
      go_BYE(-1);
    }
  }
  else {
    if ( strncmp(X, "true", strlen("true")) == 0 ) {
      // SUCCESS
      X  += strlen("true"); nX -= strlen("true");
    }
    else if ( strncmp(X, "false", strlen("false")) == 0 ) {
      // SUCCESS
      X  += strlen("false"); nX -= strlen("false");
    }
    else {
      go_BYE(-1);
    }
  }
  *ptr_X = X; *ptr_nX = nX;
  printf("bool\n"); 

BYE:
  return status;
}
