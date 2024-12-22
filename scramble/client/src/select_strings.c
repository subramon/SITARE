#include <stdio.h>
#include <stdbool.h>
#include <string.h>
#include <stdint.h>
#include "q_macros.h"

#include "game_state.h"
// utilities 
#include "cat_to_buf.h"
#include "shuffle.h"

#include "select_strings.h"

int
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
  if ( nstrings > 327267 ) { go_BYE(-1); }

  // cannot pick more than you have 
  if ( n_to_pick > nstrings ) { go_BYE(-1); }
  uint32_t n_picked = 0; 
  for ( uint32_t i = 0; i < n_to_pick; i++ ) {
    // pick random number until you get one that hasn't been picked before 
    bool unique;
    int pick = -1;
    do { 
      // let j = random index selected
      long int result; 
      lrand48_r(ptr_rand_buf, &result); 
      pick = (int)(result % (long int)nstrings); 
      unique = true; 
      for ( uint32_t j = 0; j < n_picked; j++ ) { 
        if ( pick == selected_idxs[j] ) {
          unique = false; 
          break;
        }
      }
    } while ( !unique ); 
    selected_idxs[n_picked++] = (int16_t)pick;
  }
  for ( uint32_t i = 0; i < n_to_pick; i++ ) { 
    int16_t idx = selected_idxs[i]; 
    if ( ( idx < 0 ) || ( idx >= nstrings ) ) { go_BYE(-1); }
    status = cat_to_buf(ptr_buf, ptr_bufsz, ptr_buflen, strings[idx], 0);
    cBYE(status);
  }
BYE:
  return status;
}
