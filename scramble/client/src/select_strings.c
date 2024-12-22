#include <stdio.h>
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
