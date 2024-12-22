#include "game_state.h"
extern int
select_strings(
    struct drand48_data *ptr_rand_buf,
    uint32_t n_to_pick,
    char **strings, 
    uint32_t nstrings, 
    char **ptr_buf,
    uint32_t *ptr_bufsz, 
    uint32_t *ptr_buflen,
    int16_t selected_idxs[MAX_NUM_SELECTIONS]
    );
