#include <stdio.h>
#include <stdbool.h>
#include <lualib.h>
#include "q_macros.h"
#include "consts.h"
#include "game_state.h"
#include "bridge_anagram.h"
#include "get_array_of_strings.h"
int
bridge_anagram(
    lua_State *L,
    const char * const can_str,
    char ***ptr_anagrams, // [n_anagrams]
    uint32_t *ptr_n_anagrams
    )
{
  int status = 0;

  const char *lua_fn = "anagram";
  lua_getglobal(L, lua_fn);
  int chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isfunction(L, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(L, 1);
    go_BYE(-1);
  }
  lua_pushstring(L, can_str); 
  chk = lua_gettop(L); if ( chk != 2 ) { go_BYE(-1); }
  // call lua function and check status 
  status = lua_pcall(L, 1, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "lua_fn %s failed: %s\n", lua_fn, lua_tostring(L, -1));
    lua_pop(L, 1);
    go_BYE(-1); 
  }
  chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_istable(L, 1) ) { go_BYE(-1); }
  status = get_array_of_strings(L, 1, ptr_anagrams, ptr_n_anagrams);

  // cleanup
  chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  lua_pop(L, 1);
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }
BYE:
  return status;
}
