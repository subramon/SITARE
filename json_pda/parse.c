#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "q_macros.h"

typedef enum _jtype_t { 
  j_undefined,
  j_nil,
  j_boolean,
  j_number,
  j_string,
  j_object,
  j_array,

} jtype_t;

#define chkX(ptr_X, ptr_nX) { \
  if ( ptr_X == NULL ) { go_BYE(-1); } \
  if ( ptr_nX == 0 ) { go_BYE(-1); } \
}
static int
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

static int
parse_null(
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
  else {
    if ( strncmp(X, "null", strlen("null")) == 0 ) { 
      // SUCCESS
      X  += strlen("null"); nX -= strlen("null");
    }
    else {
      go_BYE(-1);
    }
  }
  *ptr_X = X; *ptr_nX = nX;

BYE:
  return status;
}

static int
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
    if ( isspace
    if ( !isascii(*X) ) { go_BYE(-1); }
    X++; nX--; // consume another character in string 
  }
  *ptr_X = X; *ptr_nX = nX;

BYE:
  return status;
}
static int
parse_string(
    char **ptr_X,
    size_t *ptr_nX
    )
{
  int status = 0;
  chkX(ptr_X, ptr_nX); 
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
  *ptr_X = X; *ptr_nX = nX;

BYE:
  return status;
}
static int
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

BYE:
  return status;
}
//-------------------------------------------------------------
static int
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
    // status = parse_number(ptr_X, ptr_nX); 
  }
  else if ( *X == '[' ) { 
    // status = parse_array(ptr_X, ptr_nX); 
  }
  else if ( *X == '{' ) { 
    // status = parse_object(ptr_X, ptr_nX); 
  }
  else {
    go_BYE(-1); 
  }
  cBYE(status);
  status = parse_json(ptr_X, ptr_nX); cBYE(status);

BYE:
  return status;
}


int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *bak_str = NULL; 
  char *str = NULL; size_t sz = 0;

  free_if_non_null(bak_str);
  str = strdup(""); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status);
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup(" "); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status);
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup(" null "); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status); 
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("true"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status); 
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("\t\nfalse\n"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status); 
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("\t\n\"abc\"\n"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status); 
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("53\n"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status); 
  //-----------------------------------------
  printf("SUCCESS\n");
BYE:
  free_if_non_null(bak_str);
  return status;
}
