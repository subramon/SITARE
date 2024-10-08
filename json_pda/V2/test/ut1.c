#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include "macros.h"
#include "json_types.h"
#include "parse_json.h"
#include "free_json.h"

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  char *bak_str = NULL; 
  char *str = NULL; size_t sz = 0;
  json_t J; memset(&J, 0, sizeof(json_t));

  free_if_non_null(bak_str);
  str = strdup(""); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz, &J);  cBYE(status);
  //-----------------------------------------
  free_if_non_null(bak_str); 
  str = strdup(" "); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz, &J);  cBYE(status);
  free_json(&J);
  //-----------------------------------------
  free_if_non_null(bak_str); 
  str = strdup(" null "); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz, &J);  cBYE(status); 
  free_json(&J);
  //-----------------------------------------
  free_if_non_null(bak_str); 
  str = strdup("true"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz, &J);  cBYE(status); 
  free_json(&J);
  //-----------------------------------------
  free_if_non_null(bak_str); 
  str = strdup("\t\nfalse\n"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz, &J);  cBYE(status); 
  free_json(&J);
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("\t\n\"abc\"\n"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz, &J);  cBYE(status); 
  free_json(&J);
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("53\n"); sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz, &J);  cBYE(status); 
  free_json(&J);
#ifdef XXXX
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("[ true, false, \"def\", 1234] \n"); 
  sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status); 
  //-----------------------------------------
  free_if_non_null(bak_str);
  str = strdup("{ \"x\" : true, \"y\" : 6789, \"z\" : \"abcd\" } \n"); 
  sz = strlen(str); bak_str = str;
  status = parse_json(&str, &sz);  cBYE(status); 
  //-----------------------------------------
#endif 
  printf("SUCCESS\n");
BYE:
  free_if_non_null(bak_str);
  return status;
}
