#include <lualib.h>
extern int
bridge_anagram(
    lua_State *L,
    const char * const can_str,
    char ***ptr_anagrams, // [n_anagrams]
    uint32_t *ptr_n_anagrams
    );
