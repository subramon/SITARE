 return [[ 

typedef struct _game_state_t {
  char **prev_words;
  uint32_t nprev;
  char **curr_words;
  uint32_t ncurr;
  char **letters;
  uint32_t nlttr;


  int clock;
} game_state_t;

 ]] 
