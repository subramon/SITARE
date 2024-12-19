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
// connect to server
#include "setup_curl.h"
#include "execute.h"

#include "game_state.h"
#include "bridge_read_state.h"
#include "shuffle.h"
#include "mk_lua_state.h"

#define MAX_NUM_SELECTIONS 32 // max number to pick frm pool/words

static uint32_t
calc_num_iters(
    uint32_t i
)
{

  if ( i == 0 ) { 
    return 1; 
  }
  else if ( i == 2 ) { 
    return i;
  }
  return 8; // TODO
}
static int
select_strings(
    uint32_t n_to_pick,
    char **words, 
    uint32_t nwords, 
    char **ptr_buf,
    uint32_t *ptr_bufsz, 
    uint32_t *ptr_buflen
    )
{
  int status = 0;
  int8_t idxs_for_perm[MAX_NUM_SELECTIONS];

  if ( n_to_pick == 0 ) { return status; }
  if ( nwords == 0 ) { return status; }

  for ( uint32_t i = 0; i < nwords; i++ ) { 
    idxs_for_perm[i] = (int8_t)i; 
  }
  randomize_I1(idxs_for_perm, (int)nwords); cBYE(status); 
  for ( uint32_t i = 0; i < n_to_pick; i++ ) { 
    int idx = (int)idxs_for_perm[i]; 
    status = cat_to_buf(ptr_buf, ptr_bufsz, ptr_buflen, words[idx], 0);
    cBYE(status);
  }
BYE:
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
#define URL_LEN 1024-1
  char url[URL_LEN+1];  memset(url, 0, URL_LEN+1);
  game_state_t S; memset(&S, 0, sizeof(game_state_t));
  char *server_response = NULL;
  char *buf = NULL; uint32_t buflen = 0; uint32_t bufsz = 64; 
  curl_userdata_t curl_userdata; 
  memset(&curl_userdata, 0, sizeof(curl_userdata_t));

  if ( argc != 4 ) { go_BYE(-1); }
  int user = atoi(argv[1]);
  const char * const server = argv[2]; 
  int port = atoi(argv[3]);

  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  status = luaL_dostring(L, "require 'scramble/client/lua/read_state'");
  int chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

  // for curl 
  status = setup_curl(&curl_userdata, &ch); cBYE(status);

  buf = malloc(bufsz); memset(buf, 0, bufsz); 

  for ( int try = 0; ; try++ ) { 
    long http_code;
    /* for testing 
    char json_file[32];sprintf(json_file, "../test/%d.json", i); 
    server_response = file_as_str(json_file); 
    */
    // Get state from server in server_response
    sprintf(url, "http://%s:%d/GetState", server, port);
    memset(curl_userdata.base, 0, curl_userdata.size);
    curl_userdata.offset = 0; 
    status = execute(ch, url, &http_code); cBYE(status);
    if ( http_code != 200 ) { go_BYE(-1); }
    // Load state 
    status = bridge_read_state(L, curl_userdata.base, &S); 
    cBYE(status); 
    uint64_t t_start = get_time_usec(); 
    for ( uint32_t i = 0; i < S.nlttr; i++ ) {
      uint32_t outer_iters = calc_num_iters(i);
      for ( uint32_t ii = 0; ii < outer_iters; ii++ ) {
        // Select i letters
        status = select_strings(i, S.letters, S.nlttr, 
            &buf, &bufsz, &buflen);
        cBYE(status); 
        for ( uint32_t j = 0; j < S.ncurr; j++ ) { 
          uint32_t inner_iters = calc_num_iters(j);
          for ( uint32_t jj = 0; jj < inner_iters; jj++ ) {
            if ( ( j == 0 ) && ( i <= 2 ) ) { continue; } 
            // Select j words
            status = select_strings(j, S.curr_words, S.ncurr, 
                &buf, &bufsz, &buflen);
            cBYE(status); 
          }
        }
        printf("%s\n", buf);
        memset(buf, 0, bufsz); buflen = 0; 
      }
    }

    // Add letter 
    sprintf(url, "http://%s:%d/AddLetter", server, port);
    memset(curl_userdata.base, 0, curl_userdata.size);
    curl_userdata.offset = 0; 
    status = execute(ch, url, &http_code); cBYE(status);
    if ( http_code != 200 ) { go_BYE(-1); }
    // cleanup
    free_state(&S); 
    memset(curl_userdata.base, 0, curl_userdata.size);
    curl_userdata.offset = 0; 
    printf("Attempt %d \n", try);
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
  free_if_non_null(curl_userdata.base);
  // STOP: For curl 
  return status;
}
