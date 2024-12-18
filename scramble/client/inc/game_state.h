#ifndef __GAME_STATE_H
#define __GAME_STATE_H

//START_FOR_CDEF
typedef struct _game_state_t {
  char **prev_words;
  uint32_t nprev; 
  char **curr_words;
  uint32_t ncurr; 
  char *letters;
  uint32_t nl;
  int clock; 
} game_state_t; 
//STOP_FOR_CDEF

#endif // __GAME_STATE_H
