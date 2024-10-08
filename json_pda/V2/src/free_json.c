#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "macros.h"
#include "json_types.h"
#include "free_json.h"

int
free_json(
    json_t *ptr_J
    )
{
  int status = 0;
  if ( ptr_J == NULL ) { go_BYE(-1); }
  switch ( ptr_J->jtype ) { 
    case j_undefined : 
    case j_null : 
    case j_boolean : 
    case j_number : 
      break;
    case j_string : 
      free_if_non_null(ptr_J->data.str_val);
      break;
    case j_array : 
      go_BYE(-1); 
      break;
    case j_object : 
      go_BYE(-1); 
      break;
    default : 
      go_BYE(-1); 
      break;
  }
  memset(ptr_J, 0, sizeof(json_t));
BYE:
  return status;
}
