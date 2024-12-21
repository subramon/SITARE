#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#ifdef DEBUG
#include <jansson.h>
#endif
#include "q_macros.h"
// utilities 
#include "cat_to_buf.h"
// connect to server
#include "game_state.h"
#include "http_make_word.h"


// assemble HTTP request for make_word
int 
http_make_word(
    game_state_t *ptr_S,
    int user,
    int8_t *lidx_selection, // index in S.pool of letter chosen 
    uint32_t nPminus,
    int8_t *widx_selection,// index in W.pool of letter chosen 
    uint32_t nWminus,
    char **Wplus,
    uint32_t nWplus,
    char **ptr_out
)
{
  int status = 0;
#ifdef DEBUG
  json_t *jroot = NULL; json_error_t jerror; 
#endif
  char lbuf[1024]; memset(lbuf, 0, 1024);
  char *buf = NULL; uint32_t sz_buf = 1024; uint32_t n_buf = 0;
  buf = malloc(sz_buf);
  memset(buf, 0, sz_buf);
  // add user to JSON 
  sprintf(lbuf, "{ \"User\" : %d ", user); 
  status = cat_to_buf(&buf, &sz_buf, &n_buf, lbuf, 0); cBYE(status);
  // add letters to JSON 
  if ( nPminus > 0 ) { 
    sprintf(lbuf, " , \"Pminus\" : [ ");
    status = cat_to_buf(&buf, &sz_buf, &n_buf, lbuf, 0); cBYE(status);
    for ( uint32_t i = 0; i < nPminus; i++ ) { 
      if ( i > 0 ) {
        status = cat_to_buf(&buf, &sz_buf, &n_buf, ", ", 2); cBYE(status);
      }
      sprintf(lbuf, "\"%s\"", ptr_S->letters[lidx_selection[i]]);
      status = cat_to_buf(&buf, &sz_buf, &n_buf, lbuf, 0); cBYE(status);
    }
    status = cat_to_buf(&buf, &sz_buf, &n_buf, "]", 1); 
  }
  if ( nWminus > 0 ) { 
    go_BYE(-1); // TODO 
  }
  // put new words into request
  if ( nWplus == 0 ) { go_BYE(-1); }
  sprintf(lbuf, " , \"Wplus\" : [ ");
  status = cat_to_buf(&buf, &sz_buf, &n_buf, lbuf, 0); cBYE(status);
  for ( uint32_t i = 0; i < nWplus; i++ ) { 
    if ( i > 0 ) {
      status = cat_to_buf(&buf, &sz_buf, &n_buf, ", ", 2); cBYE(status);
    }
    sprintf(lbuf, "\"%s\"", Wplus[i]);
    status = cat_to_buf(&buf, &sz_buf, &n_buf, lbuf, 0); cBYE(status);
  }
  status = cat_to_buf(&buf, &sz_buf, &n_buf, "]}", 2); 
  //----------------------------------
#ifdef DEBUG
  jroot = json_loads(buf, 0, &jerror);
  if ( jroot == NULL ) { go_BYE(-1); } else { json_decref(jroot); }
#endif
  *ptr_out = buf; 
BYE:
  return status;
}
