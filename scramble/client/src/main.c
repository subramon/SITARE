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
#include "get_time_usec.h"
#include "file_as_str.h"
#include "cat_to_buf.h"
#include "mk_lua_state.h"
#include "url.h"
// connect to server
#include "setup_curl.h"
#include "execute.h"

#include "game_state.h"
#include "bridge_read_state.h"
#include "generator.h"
#include "http_make_word.h"

static int
free_state(
    game_state_t *ptr_S
    )
{
  int status = 0;
  if ( ptr_S == NULL ) { go_BYE(-1); }
  free_2d_array(&(ptr_S->used_words), ptr_S->nused); 
  free_2d_array(&(ptr_S->curr_words), ptr_S->ncurr); 
  free_if_non_null(ptr_S->letters);
BYE:
  return status;
}

static int
load_state(
    lua_State *L, 
    CURL *ch,
    const char * const server,
    int port, 
    curl_userdata_t *ptr_curl_userdata,
    game_state_t *ptr_S
    )
{
  int status = 0;
  char url[MAX_URL_LEN+1];  memset(url, 0, MAX_URL_LEN+1);
  // Get state from server in server_response
  sprintf(url, "http://%s:%d/GetState", server, port);
  memset(ptr_curl_userdata->base, 0, ptr_curl_userdata->size);
  ptr_curl_userdata->offset = 0; 
  long http_code;
  status = execute(ch, url, &http_code); cBYE(status);
  if ( http_code != 200 ) { go_BYE(-1); }
  // Load state 
  status = bridge_read_state(L, ptr_curl_userdata->base, ptr_S); 
  cBYE(status); 
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
  uint64_t timeout = 1000000; // max time for generator 
  CURL *ch = NULL; // curl handle
  char **new_words = NULL;  uint32_t n_new_words = 0; 
  char url[MAX_URL_LEN+1];  memset(url, 0, MAX_URL_LEN+1);
  game_state_t S; memset(&S, 0, sizeof(game_state_t));
  char *server_response = NULL;
  curl_userdata_t curl_userdata; 
  memset(&curl_userdata, 0, sizeof(curl_userdata_t));
  char *http_req = NULL; 
  char *enc_http_req = NULL; 

  if ( argc != 4 ) { go_BYE(-1); }
  int user = atoi(argv[1]);
  const char * const server = argv[2]; 
  int port = atoi(argv[3]);

  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  status = luaL_dostring(L, "require 'read_state'");
  int chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

  status = luaL_dostring(L, "anagram = require 'anagram'");
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

  status = luaL_dostring(L, "fn = require 'words_to_anagrams'");
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

  status = luaL_dostring(L, "can_str_to_anagram = fn('../../word_list')"); 
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

  // for curl 
  status = setup_curl(&curl_userdata, &ch); cBYE(status);

  for ( int try = 0; ; try++ ) { 
    status = load_state(L, ch, server, port, &curl_userdata, &S);
    // try and create some words 
    int16_t lttr_selected_idxs[MAX_NUM_SELECTIONS]; uint16_t nPminus = 0;
    int16_t word_selected_idxs[MAX_NUM_SELECTIONS]; uint16_t nWminus = 0;
    free_2d_array(&new_words, n_new_words); n_new_words = 0; 
    status = generator(&S, L, timeout, lttr_selected_idxs, &nPminus, 
        word_selected_idxs, &nWminus, &new_words, &n_new_words);
    cBYE(status);
    if ( n_new_words > 0 ) { // make word 
      printf("Making word %s \n", new_words[0]); 
      // make http request to add word 
      status = http_make_word(&S, user, 
          lttr_selected_idxs, nPminus,
          word_selected_idxs, nWminus,
          new_words, n_new_words, &http_req);
      cBYE(status);
      // encode URL 
      status = url_encode(http_req, &enc_http_req); cBYE(status);
      snprintf(url, MAX_URL_LEN, 
          "http://%s:%d/MakeWord?%s", server, port, enc_http_req);
      // send request to server 
      memset(curl_userdata.base, 0, curl_userdata.size);
      curl_userdata.offset = 0; 
      long int http_code; 
      status = execute(ch, url, &http_code); cBYE(status);
      if ( http_code != 200 ) { 
        printf("bad request %s\n", http_req); go_BYE(-1); 
      }
      free_if_non_null(http_req);
      free_if_non_null(enc_http_req); 
      free_2d_array(&new_words, n_new_words); n_new_words = 0;
    }
    else { // Add letter 
      sprintf(url, "http://%s:%d/AddLetter", server, port);
      memset(curl_userdata.base, 0, curl_userdata.size);
      curl_userdata.offset = 0; 
      long int http_code; 
      status = execute(ch, url, &http_code); cBYE(status);
      if ( http_code != 200 ) { go_BYE(-1); }
    }

    // cleanup
    free_state(&S); 
    memset(curl_userdata.base, 0, curl_userdata.size);
    curl_userdata.offset = 0; 
    printf("Attempt %d \n", try);
  }

BYE:
  free_if_non_null(server_response);
  free_state(&S); 
  free_2d_array(&new_words, n_new_words);
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
     */
  curl_global_cleanup();
  free_if_non_null(curl_userdata.base);
  free_if_non_null(http_req);
  free_if_non_null(enc_http_req); 
  // STOP: For curl 
  return status;
}
