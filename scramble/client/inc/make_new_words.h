extern int
make_new_words(
    lua_State *L,
    const char * const buf, 
    const game_state_t * const ptr_S,
    char ***ptr_new_words,
    uint32_t *ptr_n_new_words
    );
