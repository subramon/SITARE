#include <stdio.h>
#include <stdbool.h>
#include "q_macros.h"
#include "lualib.h"
#include "game_state.h"
#include "bridge_read_state.h"
int
bridge_read_state(
    lua_State *L,
    const char * const server_response,
    game_state_t *game_state
    )
{
  int status = 0;

  const char *lua_fn = "read_state";
  lua_getglobal(L, lua_fn);
  int chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isfunction(L, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(L, 1);
    go_BYE(-1);
  }
  lua_pushstring(L, server_response); 
  chk = lua_gettop(L); if ( chk != 2 ) { go_BYE(-1); }
  lua_pushlightuserdata(L, game_state); 
  chk = lua_gettop(L); if ( chk != 3 ) { go_BYE(-1); }
  // call lua function and check status 
  status = lua_pcall(L, 2, 1, 0);
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
