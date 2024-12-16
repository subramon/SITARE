#include "q_macros.h"
#include "mk_lua_state.h"

#include "q_incs.h"
#include <lua.h>
#include <lauxlib.h>
#include <lualib.h>
#include "mk_lua_state.h"

#define MAX_LEN_DIR_NAME 127

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  lua_State *L = NULL;
  char lcmd[1024];
  const char * const lua_fn = "scramble/server/lua/game";
  char run_fn[32];

  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  sprintf(lcmd, "rdfn = require '%s'", lua_fn); 
  status = luaL_dostring(L, lcmd); 
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  // initialize game 
  status = luaL_dostring(L, "init()");
  if ( status != 0 ) { 
    fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
  }
  for ( ; ; ) { 
    char *lineptr = NULL; size_t n;
    ssize_t nr = getline(&lineptr, &n, stdin);
    if ( ( lineptr == NULL ) || ( n == 0 ) || ( *lineptr == '\n' ) ) { 
      free_if_non_null(lineptr);
      break;
    }
    printf("Command = %s", lineptr);
    int n_in, n_out;
    if ( lineptr[0] == 'p'  ) {
      strcpy(run_fn, "pr"); n_in = 0; n_out = 1; 
    }
    else if ( lineptr[0] == 'a'  ) {
      strcpy(run_fn, "add_to_pool"); n_in = 0; n_out = 1; 
    }
    else if ( lineptr[0] == 'w'  ) {
      strcpy(run_fn, "make_word"); n_in = 1; n_out = 1; 
    }
    else { 
      printf("Unknown option. Skipping....\n");
      continue;
    }
    status = luaL_dostring(L, lcmd); 
    if ( status != 0 ) { 
      fprintf(stderr, "Error luaL_string=%s\n", lua_tostring(L,-1));
    }
    int chk;
    lua_getglobal(L, run_fn);
    chk = lua_gettop(L); if ( chk != 1 ) { go_BYE(-1); }
    chk = lua_type(L, -1); 
    if ( !lua_isfunction(L, -1)) {
      fprintf(stderr, "Lua Function %s undefined\n", run_fn);
      lua_pop(L, 1);
      go_BYE(-1);
    }
    if ( lineptr[0] == 'w'  ) {
      // add string to stack 
      if ( strlen(lineptr) <= 3 ) { go_BYE(-1); }
      lua_pushstring(L, lineptr+2); 
      chk = lua_gettop(L); if ( chk != 2 ) { go_BYE(-1); }
    }
    // call lua function and check status 
    status = lua_pcall(L, n_in, n_out, 0);
    if ( status != 0 ) {
      fprintf(stderr, "run_fn %s failed: %s\n", run_fn, lua_tostring(L, -1));
      lua_pop(L, 1);
      go_BYE(-1); 
    }
    chk = lua_gettop(L); if ( chk != n_out ) { go_BYE(-1); }
    if ( lineptr[0] == 'p'  ) {
      if ( !lua_isstring(L, 1) ) { go_BYE(-1); }
      const char *cptr = lua_tostring(L, -1);
      if ( cptr == NULL ) { go_BYE(-1); }
      fprintf(stdout, "%s\n", cptr);
    } 
    else if ( lineptr[0] == 'a'  ) {
      if ( !lua_isnumber(L, 1) ) { go_BYE(-1); }
      int nP = lua_tonumber(L, -1);
      fprintf(stdout, "#P = %d \n", nP);
    }
    else if ( lineptr[0] == 'w'  ) {
      if ( !lua_isboolean(L, 1) ) { go_BYE(-1); }
      bool bval = lua_toboolean(L, -1);
      if ( !bval ) {
        fprintf(stderr, "Bad input %s \n", lineptr);
      }
    }
    else {
      go_BYE(-1);
    }
    // clean up lua stack 
    lua_pop(L, n_out);
    chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

    free_if_non_null(lineptr);
  }

  printf("C: All done\n");
BYE:
  if ( L != NULL ) { lua_close(L); }
}
