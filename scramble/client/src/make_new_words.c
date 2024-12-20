#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include "q_macros.h"
#include "consts.h"
#include "game_state.h"
#include "bridge_anagram.h"
#include "canonicalize.h"
#include "free_2d_array.h"
#include "make_new_words.h"
static int
is_str_in_set(
    const char * const needle,
    char **haystacks,
    uint32_t n_haystacks,
    bool *ptr_exists
    )
{
  int status = 0;
  if ( needle == NULL ) { go_BYE(-1); }
  if ( haystacks == NULL ) { *ptr_exists = false; return status; } 
  if ( n_haystacks == 0 ) { *ptr_exists = false; return status; } 
  // Check if this exists in history
  *ptr_exists = false; 
  for ( uint32_t l = 0; l < n_haystacks; l++ ) {
    if ( strcmp(needle, haystacks[l]) == 0 ) { 
      *ptr_exists = true; break;
    }
  }
BYE:
  return status;
}

int
make_new_words(
    lua_State *L,
    const char * const buf, 
    const game_state_t * const ptr_S,
    char ***ptr_new_words, // output 
    uint32_t *ptr_n_new_words // output 
    )
{
  int status = 0;
  char **new_words = NULL;  uint32_t n_new_words = 0;
  char **anagrams = NULL; uint32_t n_anagrams = 0; 
  char can_str[MAX_LEN_CANONICAL_STR];
  memset(can_str, 0, MAX_LEN_CANONICAL_STR);

  // words should have at least 3 letters
  if ( strlen(buf) < 3 ) { goto BYE; } 

  // For starters, we will consider creating only a single word 
  // In other words, we will not partition the letters into 
  // buf into more than one set, each of which is to become a word
  //
  // convert string to canonical representation 
  status = canonicalize_1(buf, can_str); cBYE(status);
  printf("%s %s\n", buf, can_str);
  status = bridge_anagram(L, can_str, &anagrams, &n_anagrams);
  if ( n_anagrams == 0 ) { goto BYE; }
  // Now, we have some candidates but we need to make sure that 
  // these have not been seen before 
  bool is_new_word = false; int aidx = -1; // pos where found 
  for ( uint32_t a = 0; a < n_anagrams; a++ ) {
    const char *l_anagram = anagrams[a];
    bool exists_in_prev;
    status = is_str_in_set(l_anagram, ptr_S->prev_words, ptr_S->nprev, 
        &exists_in_prev);
    cBYE(status);
    bool exists_in_curr;
    status = is_str_in_set(l_anagram, ptr_S->curr_words, ptr_S->ncurr, 
        &exists_in_curr);
    cBYE(status);
    if ( !exists_in_prev && !exists_in_curr ) {
      is_new_word = true;
      aidx = a; 
      break;
    }
  }
  if ( is_new_word ) { 
    n_new_words = 1;
    new_words = malloc(n_new_words * sizeof(char *));
    if ( ( aidx < 0 ) || ( aidx >= (int)n_anagrams ) ) { go_BYE(-1); }
    new_words[n_new_words-1] = strdup(anagrams[aidx]);
  }
BYE:
  free_2d_array(&anagrams, n_anagrams);
  *ptr_new_words = new_words; 
  *ptr_n_new_words = n_new_words; 
  return status;
}
