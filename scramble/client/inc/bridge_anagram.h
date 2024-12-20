#include <lualib.h>
extern int
bridge_anagram(
    lua_State *L,
    const char * const can_str,
    char *anagrams[MAX_NUM_ANAGRAMS],
    int *ptr_num_anagrams
    );
