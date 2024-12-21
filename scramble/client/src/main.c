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
#include "shuffle.h"
#include "make_new_words.h"
#include "http_make_word.h"


static uint32_t
calc_num_iters(
    uint32_t i,
    uint32_t n
)
{

  if ( i == 0 ) { 
    return 1; 
  }
  else if ( i == 1 ) { 
    return n;
  }
  else if ( i == 2 ) { 
    return (n * (n-1) / 2);
  }
  return 8; // TODO
}
static int
select_strings(
    struct drand48_data *ptr_rand_buf,
    uint32_t n_to_pick,
    char **strings, 
    uint32_t nstrings, 
    char **ptr_buf,
    uint32_t *ptr_bufsz, 
    uint32_t *ptr_buflen,
    int16_t selected_idxs[MAX_NUM_SELECTIONS]
    )
{
  int status = 0;

  if ( n_to_pick == 0 ) { return status; }
  if ( nstrings == 0 ) { return status; }

  memset(selected_idxs, 0, sizeof(int16_t)*MAX_NUM_SELECTIONS);
  for ( uint32_t i = 0; i < nstrings; i++ ) { 
    selected_idxs[i] = (int16_t)i; 
  }
  randomize_I2(ptr_rand_buf, selected_idxs, (int)nstrings); cBYE(status); 
#ifdef DEBUG
  for ( uint32_t i = 0; i < nstrings; i++ ) { 
    if ( ( selected_idxs[i] < 0 ) || 
        ( selected_idxs[i] > (int16_t)nstrings ) ) {
      go_BYE(-1);
    }
  }
#endif
  for ( uint32_t i = 0; i < n_to_pick; i++ ) { 
    int16_t idx = selected_idxs[i]; 
    status = cat_to_buf(ptr_buf, ptr_bufsz, ptr_buflen, strings[idx], 0);
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
  free_2d_array(&(ptr_S->used_words), ptr_S->nused); 
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
  char **new_words = NULL;  uint32_t n_new_words = 0; 
#define MAX_URL_LEN 2048-1
  char url[MAX_URL_LEN+1];  memset(url, 0, MAX_URL_LEN+1);
  game_state_t S; memset(&S, 0, sizeof(game_state_t));
  char *server_response = NULL;
  char *buf = NULL; uint32_t buflen = 0; uint32_t bufsz = 64; 
  curl_userdata_t curl_userdata; 
  memset(&curl_userdata, 0, sizeof(curl_userdata_t));
  struct drand48_data rand_buf; 
  memset(&rand_buf, 0, sizeof(struct drand48_data));
  char *http_req = NULL; 
  char *enc_http_req = NULL; 

  srand48_r((long int)time(NULL), &rand_buf); 
  srand48_r(123456789, &rand_buf); 
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

  buf = malloc(bufsz); memset(buf, 0, bufsz); 

  int counter = 0;
  for ( int try = 0; ; try++ ) { 
    long http_code;
    n_new_words = 0;
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
    // #define pragma omp parallel for 
    uint32_t nPminus = 0, nWminus = 0;
    int16_t lttr_selected_idxs[MAX_NUM_SELECTIONS];
    memset(lttr_selected_idxs, 0, sizeof(int16_t)*MAX_NUM_SELECTIONS);
    int16_t word_selected_idxs[MAX_NUM_SELECTIONS];
    memset(word_selected_idxs, 0, sizeof(int16_t)*MAX_NUM_SELECTIONS);
    for ( uint32_t i = 0; i < S.nlttr; i++ ) {
      nPminus = i;
      uint32_t outer_iters = calc_num_iters(i, S.nlttr);
      for ( uint32_t ii = 0; ii < outer_iters; ii++ ) {
        memset(buf, 0, bufsz); buflen = 0; 
        // Select i letters
        status = select_strings(&rand_buf, nPminus, S.letters, S.nlttr, 
            &buf, &bufsz, &buflen, lttr_selected_idxs);
        cBYE(status); 
        if ( nPminus == 3 ) { 
          printf("hello world\n");
        }
        if ( strlen(buf) != nPminus ) { go_BYE(-1); }
        // Note the <= below 
        for ( uint32_t j = 0; j <= S.ncurr; j++ ) {
          uint32_t inner_iters = calc_num_iters(j, S.ncurr);
          nWminus = j;
          for ( uint32_t jj = 0; jj < inner_iters; jj++ ) {
            // Select j words
            uint32_t before_buflen = buflen;
            status = select_strings(&rand_buf, nWminus, S.curr_words, S.ncurr, 
                &buf, &bufsz, &buflen, word_selected_idxs);
            cBYE(status); 
            status = make_new_words(L, buf, &S, &new_words, &n_new_words); 
            cBYE(status); 
            counter++; 
            if (( new_words == NULL ) && ( n_new_words > 0 )) { go_BYE(-1); }
            if (( new_words != NULL ) && ( n_new_words == 0 )) { go_BYE(-1); }
            if ( n_new_words > 0 ) { 
              printf("found %d candidates [", n_new_words);
              printf("[ ");
              for ( uint32_t kk = 0; kk < n_new_words; kk++ ) { 
                printf(" %s ", new_words[kk]);
              }
              printf("]\n");
              memset(buf, 0, bufsz); buflen = 0; 
              break; 
            }
            // undo the selection you just made 
            for ( uint32_t bb = before_buflen; bb < buflen; bb++ ) {
              buf[bb] = '\0';
            }
            buflen = before_buflen;

          }
          if ( n_new_words > 0 ) { break; }
        }
        if ( n_new_words > 0 ) { break; }
      }
      if ( n_new_words > 0 ) { break; }
    }
    if ( n_new_words > 0 ) { // make word 
      printf("Making word %s \n", new_words[0]); 
      // make http request to add word 
      // -------------------------
      status = http_make_word(&S, user, 
          lttr_selected_idxs, nPminus,
          word_selected_idxs, nWminus,
          new_words, n_new_words, &http_req);
      cBYE(status);
      status = url_encode(http_req, &enc_http_req); cBYE(status);
      snprintf(url, MAX_URL_LEN, 
          "http://%s:%d/MakeWord?%s", server, port, enc_http_req);
      memset(curl_userdata.base, 0, curl_userdata.size);
      curl_userdata.offset = 0; 
      status = execute(ch, url, &http_code); cBYE(status);
      if ( http_code != 200 ) { 
        printf("bad request %s\n", http_req);
        go_BYE(-1); 
      }
      free_if_non_null(http_req);
      free_if_non_null(enc_http_req); 
      free_2d_array(&new_words, n_new_words);
      n_new_words = 0;
    }
    else { // Add letter 
      sprintf(url, "http://%s:%d/AddLetter", server, port);
      memset(curl_userdata.base, 0, curl_userdata.size);
      curl_userdata.offset = 0; 
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
  free_2d_array(&new_words, n_new_words);
  free_if_non_null(http_req);
  free_if_non_null(enc_http_req); 
  // STOP: For curl 
  return status;
}
