extern int 
http_make_word(
    game_state_t *ptr_S,
    int user,
    int16_t lidx_selection[MAX_NUM_SELECTIONS], // index in S.pool of letter chosen 
    uint32_t nPminus,
    int16_t widx_selection[MAX_NUM_SELECTIONS],// index in W.pool of letter chosen 
    uint32_t nWminus,
    char **Wplus,
    uint32_t nWplus,
    char **ptr_out
);
