#ifndef __GAME_STATE_H
#define __GAME_STATE_H
#include <stdint.h>
#include "consts.h"

//START_FOR_CDEF
typedef struct _game_state_t {
  char **used_words;
  uint32_t nused; 
  char **curr_words;
  uint32_t ncurr; 
  char **letters; 
  uint32_t nlttr;
  // letters[i] is a 1 char string and could have been kept as char *letters
  // but this way, it is consistent treatment with words
  int clock; 
} game_state_t; 
//STOP_FOR_CDEF

#endif // __GAME_STATE_H
