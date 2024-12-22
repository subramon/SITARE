#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <lualib.h>
#include "q_macros.h"
#include "game_state.h"
#include "select_strings.h"
#include "make_new_words.h"
#include "get_time_usec.h"
#include "generator.h"

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

int
generator(
    const game_state_t * const ptr_S,
    lua_State *L, 
    uint64_t timeout, 
    char ***ptr_new_words,
    uint32_t *ptr_n_new_words
    )
{
  int status = 0;
  char **new_words = NULL; uint32_t n_new_words = 0;
  struct drand48_data rand_buf; 
  memset(&rand_buf, 0, sizeof(struct drand48_data));
  char *buf = NULL; uint32_t buflen = 0; uint32_t bufsz = 64; 

  *ptr_new_words = NULL; *ptr_n_new_words = 0;
  uint64_t t_start = get_time_usec(); 
  uint32_t nPminus = 0, nWminus = 0;
  int16_t lttr_selected_idxs[MAX_NUM_SELECTIONS];
  memset(lttr_selected_idxs, 0, sizeof(int16_t)*MAX_NUM_SELECTIONS);
  int16_t word_selected_idxs[MAX_NUM_SELECTIONS];
  memset(word_selected_idxs, 0, sizeof(int16_t)*MAX_NUM_SELECTIONS);
  // #define pragma omp parallel for 
  for ( uint32_t i = 0; i < ptr_S->nlttr; i++ ) {
    nPminus = i;
    uint32_t outer_iters = calc_num_iters(i, ptr_S->nlttr);
    for ( uint32_t ii = 0; ii < outer_iters; ii++ ) {
      memset(buf, 0, bufsz); buflen = 0; 
      // Select i letters
      status = select_strings(&rand_buf, nPminus, ptr_S->letters, ptr_S->nlttr, 
          &buf, &bufsz, &buflen, lttr_selected_idxs);
      cBYE(status); 
      if ( nPminus == 3 ) { 
        printf("hello world\n");
      }
      if ( strlen(buf) != nPminus ) { go_BYE(-1); }
      // Note the <= below 
      for ( uint32_t j = 0; j <= ptr_S->ncurr; j++ ) {
        uint32_t inner_iters = calc_num_iters(j, ptr_S->ncurr);
        nWminus = j;
        for ( uint32_t jj = 0; jj < inner_iters; jj++ ) {
          // Select j words
          uint32_t before_buflen = buflen;
          status = select_strings(&rand_buf, nWminus, ptr_S->curr_words, ptr_S->ncurr, 
              &buf, &bufsz, &buflen, word_selected_idxs);
          cBYE(status); 
          status = make_new_words(L, buf, ptr_S, &new_words, &n_new_words); 
          cBYE(status); 
          if (( new_words == NULL ) && ( n_new_words > 0 )) { go_BYE(-1); }
          if (( new_words != NULL ) && ( n_new_words == 0 )) { go_BYE(-1); }
          uint64_t t_now = get_time_usec(); 
          if ( ( timeout > 0 ) && ( t_now - t_start ) > timeout ) {
            return 1; // caller should watch for this 
          }
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
  *ptr_new_words = new_words;
  *ptr_n_new_words = n_new_words;
BYE:
  return status;
}
