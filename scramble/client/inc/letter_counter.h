#include <stdint.h>
#include "consts.h"
extern int
letter_counter(
    const char * const in_str, // input 
    uint8_t count[NUM_ALPHABET]
    );
extern int
pr_letter_counter(
    uint8_t count[NUM_ALPHABET]
    );
