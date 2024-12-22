extern int
generator(
    const game_state_t * const ptr_S,
    lua_State *L,
    uint64_t timeout,
    char ***ptr_new_words,
    uint32_t *ptr_n_new_words
    );
