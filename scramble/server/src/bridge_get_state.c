#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include "q_macros.h"
#include "lualib.h"
#include "bridge_get_state.h"
int
bridge_get_state(
    lua_State *L,
    char **out_str
    )
{
  int status = 0;

  const char *lua_fn = "get_state";
  lua_getglobal(L, lua_fn);
  int chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isfunction(L, -1)) {
    fprintf(stderr, "Lua Function %s undefined\n", lua_fn);
    lua_pop(L, 1);
    go_BYE(-1);
  }
  // call lua function and check status 
  status = lua_pcall(L, 0, 1, 0);
  if ( status != 0 ) {
    fprintf(stderr, "lua_fn %s failed: %s\n", lua_fn, lua_tostring(L, -1));
    lua_pop(L, 1);
    go_BYE(-1); 
  }
  chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
  if ( !lua_isstring(L, 1) ) { go_BYE(-1); }
  const char *cptr = lua_tostring(L, -1);
  if ( cptr == NULL ) { go_BYE(-1); }
  *out_str = strdup(cptr); 
  lua_pop(L, 1);
  chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

BYE:
  return status;
}
