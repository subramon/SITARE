#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "parse_aux.h"
#include "parse_null.h"
#include "parse_boolean.h"
#include "parse_number.h"
#include "parse_string.h"
#include "parse_array.h"
#include "parse_object.h"
#include "parse_json.h"

//-------------------------------------------------------------
int
parse_json(
    char **ptr_X,
    size_t *ptr_nX
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
  char *X = *ptr_X; size_t nX = *ptr_nX;
  if ( X == NULL ) { go_BYE(-1); }

  if ( nX == 0 ) { // empty string => degenerate case
    return status;
  }
  status = skip_white_space(&X, &nX); cBYE(status);
  if ( nX == 0 ) { // empty string => degenerate case
    return status;
  }
  *ptr_X = X; *ptr_nX = nX;
  if ( *X == 'n' ) { 
    status = parse_null(ptr_X, ptr_nX);
  }
  else if ( ( *X == 't' ) || ( *X == 'f' ) ) { 
    status = parse_boolean(ptr_X, ptr_nX); 
  }
  else if ( *X == '"' ) { 
    status = parse_string(ptr_X, ptr_nX); 
  }
  else if ( ( isdigit(*X) ) || ( *X == '-' ) ) {
    status = parse_number(ptr_X, ptr_nX); 
  }
  else if ( *X == '[' ) { 
    status = parse_array(ptr_X, ptr_nX); 
  }
  else if ( *X == '{' ) { 
    status = parse_object(ptr_X, ptr_nX); 
  }
  else {
    printf("Unexpected character [%c] \n", *X);
    go_BYE(-1); 
  }
  cBYE(status);

BYE:
  return status;
}
