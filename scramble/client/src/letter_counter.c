#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>
#include "q_macros.h"
#include "consts.h"
#include "letter_counter.h"
int
letter_counter(
    const char * const in_str, // input 
    uint8_t count[NUM_ALPHABET]
    )
{
  int status = 0;
  memset(count, 0, NUM_ALPHABET * sizeof(uint8_t));
  for ( const char * cptr = in_str; *cptr != '\0'; cptr++ ) {
    int idx  = *cptr - 'a';
#ifdef DEBUG
    if ( ( idx < 0 ) || ( idx >= NUM_ALPHABET ) ) { go_BYE(-1); } 
    uint8_t curr_count  = count[idx];
    if ( curr_count >= UCHAR_MAX ) { 
      go_BYE(-1); }
#endif
    count[idx]++;
  }
BYE:
  return status;
}
int
pr_letter_counter(
    uint8_t count[NUM_ALPHABET]
    )
{
  int status = 0;
  bool first = true; 
  for ( uint32_t i = 0; i < NUM_ALPHABET; i++ ) { 
    char letter = 'a' + i;
    if ( count[i] > 0 ) { 
      if ( !first ) { fprintf(stdout, ","); }
      fprintf(stdout, "%c:%1u", letter, count[i]);
    }
  }
  fprintf(stdout, "\n"); 
BYE:
  return status;
}
