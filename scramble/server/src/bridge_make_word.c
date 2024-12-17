#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "q_macros.h"
#include "lualib.h"
#include "bridge_make_word.h"
int
bridge_make_word(
    lua_State *L,
    const char * const json_str
    )
{
  int status = 0;

  if ( ( json_str == NULL ) || ( *json_str == '\0' ) ) { go_BYE(-1); }
  const char *lua_fn = "make_word";
  lua_getglobal(L, lua_fn);
  int chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isfunction(L, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(L, 1);
    go_BYE(-1);
  }
  lua_pushstring(L, json_str);
  chk = lua_gettop(L); if ( chk != 2 ) { go_BYE(-1); }
  // call lua function and check status 
  status = lua_pcall(L, 1, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "lua_fn %s failed: %s\n", lua_fn, lua_tostring(L, -1));
    lua_pop(L, 1);
    go_BYE(-1); 
  }
  chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isboolean(L, 1) ) { go_BYE(-1); }
  bool rslt = lua_toboolean(L, -1);
  if ( !rslt ) { 
    chk = lua_gettop(L); lua_pop(L, chk); 
    go_BYE(-1); 
  }
  lua_pop(L, 1);
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

BYE:
  return status;
}
