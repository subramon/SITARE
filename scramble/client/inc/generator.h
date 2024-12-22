extern int
generator(
    const game_state_t * const ptr_S,
    lua_State *L,
    uint64_t timeout,
    int16_t which_lttrs_selected[MAX_NUM_SELECTIONS],
    uint16_t *ptr_nPminus, // number of letters selected
    int16_t which_words_selected[MAX_NUM_SELECTIONS],
    uint16_t *ptr_nWminus, // number of words selected
    char ***ptr_new_words,
    uint32_t *ptr_n_new_words
    );
