#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <lualib.h>
#include <curl/curl.h>
#include "q_macros.h"
// utilities 
#include "free_2d_array.h"
#include "file_as_str.h"
#include "cat_to_buf.h"

#include "game_state.h"
#include "bridge_read_state.h"
#include "shuffle.h"
#include "mk_lua_state.h"

static int
select_strings(
    int n_to_pick,
    char **words, 
    uint32_t nwords, 
    char **ptr_buf,
    uint32_t *ptr_bufsz, 
    uint32_t *ptr_buflen
    )
{
  int status = 0;
  if ( n_to_pick == 0 ) { return status; }
  if ( nwords == 0 ) { return status; }
  int8_t *widx = NULL; 

  widx = malloc(nwords * sizeof(int8_t)); // TODO P3 avoid malloc 
  for ( uint32_t i = 0; i < nwords; i++ ) { widx[i] = (int8_t)i; }
  randomize_I1(widx, (int)nwords); cBYE(status); 
  for ( int i = 0; i < n_to_pick; i++ ) { 
    int idx = widx[i]; 
    status = cat_to_buf(ptr_buf, ptr_bufsz, ptr_buflen, words[idx], 0);
    cBYE(status);
  }
BYE:
  free_if_non_null(widx);
  return status;
}


static int
free_state(
    game_state_t *ptr_S
    )
{
  int status = 0;
  if ( ptr_S == NULL ) { go_BYE(-1); }
  free_2d_array(&(ptr_S->prev_words), ptr_S->nprev); 
  free_2d_array(&(ptr_S->curr_words), ptr_S->ncurr); 
  free_if_non_null(ptr_S->letters);
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
  lua_State *L = NULL;
  CURL *ch = NULL; // curl handle
#define CMD_LEN 1024-1
  char cmd[CMD_LEN+1];  memset(cmd, 0, CMD_LEN+1);
  game_state_t S; memset(&S, 0, sizeof(game_state_t));
  char *server_response = NULL;
  char *buf = NULL; uint32_t buflen = 0; uint32_t bufsz = 64; 

  if ( argc != 4 ) { go_BYE(-1); }
  int user = atoi(argv[1]);
  const char * const server = argv[2]; 
  int port = atoi(argv[3]);

  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  status = luaL_dostring(L, "require 'scramble/client/lua/read_state'");
  int chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

  // for curl 
  status = setup_curl(&ch); cBYE(status);

  buf = malloc(bufsz); memset(buf, 0, bufsz); 

  for ( int i = 1; i <= 6; i++ ) { 
    char json_file[32];sprintf(json_file, "../test/%d.json", i); 
    server_response = file_as_str(json_file); 
    // Get state from server in server_response
    sprintf(cmd, "http://%s:%d/GetState", server, port);
    // Load state 
    status = bridge_read_state(L, server_response, &S); 
    cBYE(status); 
    // Select nw words
    int nw = 1; int nl = 1; 
    status = select_strings(nw, S.curr_words, S.ncurr, &buf, &bufsz, &buflen);
    cBYE(status); 
    // Select nl letters
    status = select_strings(nl, S.letters, S.nlttr, &buf, &bufsz, &buflen);
    cBYE(status); 

    // cleanup
    free_state(&S); 

    free_if_non_null(server_response);
    sleep(1);
    printf("Attempt %d \n", i);
  }

BYE:
  free_if_non_null(server_response);
  free_state(&S); 
  // For Lua
  if ( L != NULL ) { lua_close(L); L = NULL; }
  // START: For curl 
  if ( ch != NULL ) {
    curl_easy_cleanup(ch);  ch = NULL;
  }
  /*
  if ( g_curl_hdrs != NULL ) {
    curl_slist_free_all(g_curl_hdrs); g_curl_hdrs = NULL;
  }
  if ( g_ss_ch != NULL ) {
    curl_easy_cleanup(g_ss_ch);  g_ss_ch = NULL;
  }
  if ( g_ss_curl_hdrs != NULL ) {
    curl_slist_free_all(g_ss_curl_hdrs); g_ss_curl_hdrs = NULL;
  }
  */
  curl_global_cleanup();
  // STOP: For curl 
  return status;
}
