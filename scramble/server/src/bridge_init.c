#include <stdio.h>
#include <stdbool.h>
#include "q_macros.h"
#include "lualib.h"
#include "bridge_init.h"
int
bridge_init(
    lua_State *L,
    uint32_t nP0,
    uint32_t nU,
    const char * const word_list
    )
{
  int status = 0;

  const char *lua_fn = "init";
  lua_getglobal(L, lua_fn);
  int chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isfunction(L, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(L, 1);
    go_BYE(-1);
  }
  lua_pushnumber(L, nP0); 
  lua_pushnumber(L, nU); 
  lua_pushstring(L, word_list); 
  chk = lua_gettop(L); if ( chk != 4 ) { go_BYE(-1); }
  // call lua function and check status 
  status = lua_pcall(L, 3, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "lua_fn %s failed: %s\n", lua_fn, lua_tostring(L, -1));
    lua_pop(L, 1);
    go_BYE(-1); 
  }
  chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isboolean(L, 1) ) { go_BYE(-1); }
  bool bval = lua_toboolean(L, -1);
  if ( !bval ) { go_BYE(-1); }
  lua_pop(L, 1);
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

BYE:
  return status;
}
