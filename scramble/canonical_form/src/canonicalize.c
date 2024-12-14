#include <stdio.h>
#include <string.h>
#include "macros.h"
#include "letter_counter.h"
#include "canonicalize.h"
// We assume that the counts cannot exceed 16
int
canonicalize_1(
    const char * const in_str, // input 
    char cstr[2*NUM_ALPHABET+1]
    )
{
  int status = 0;
  memset(cstr, 0, 2*NUM_ALPHABET+1);
  static char lc[16] = { '0', '1', '2', '3', '4', '5', '6', '7', '8', '9',
    'A', 'B', 'C', 'D', 'E', 'F' };
  uint8_t counts[NUM_ALPHABET];
  memset(counts, 0, sizeof(counts));
  status = letter_counter(in_str, counts); cBYE(status);
  int idx = 0;
  for ( int i = 0; i < NUM_ALPHABET; i++ ) { 
    if ( counts[i] > 0 ) { 
      cstr[idx++] = 'a' + i;
      cstr[idx++] = lc[counts[i]];
    }
  }
BYE:
  return status;
}
