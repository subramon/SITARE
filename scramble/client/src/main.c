#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include <lualib.h>
#include "q_macros.h"
#include "free_2d_array.h"
#include "file_as_str.h"
#include "game_state.h"
#include "bridge_read_state.h"
#include "mk_lua_state.h"


static int
free_state(
    game_state_t *ptr_S
    )
{
  int status = 0;
  if ( ptr_S == NULL ) { go_BYE(-1); }
  free_2d_array(&(ptr_S->prev_words), ptr_S->nprev); 
  free_2d_array(&(ptr_S->curr_words), ptr_S->ncurr); 
  free_if_non_null(ptr_S->letters);
BYE:
  return status;
}
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  lua_State *L = NULL;
#define CMD_LEN 1024-1
  char cmd[CMD_LEN+1];  memset(cmd, 0, CMD_LEN+1);
  game_state_t S; memset(&S, 0, sizeof(game_state_t));
  char *server_response = NULL;

  if ( argc != 4 ) { go_BYE(-1); }
  int user = atoi(argv[1]);
  const char * const server = argv[2]; 
  int port = atoi(argv[3]);

  L = mk_lua_state(); if ( L == NULL ) { go_BYE(-1); }
  status = luaL_dostring(L, "require 'scramble/client/lua/read_state'");
  int chk = lua_gettop(L); if ( chk != 0 ) { go_BYE(-1); }

  for ( int i = 1; i <= 6; i++ ) { 
    char json_file[32];sprintf(json_file, "../test/%d.json", i); 
    server_response = file_as_str(json_file); 
    // Get state from server in server_response
    sprintf(cmd, "http://%s:%d/GetState", server, port);
    // Load state 
    status = bridge_read_state(L, &S); cBYE(status); cBYE(status);
    // cleanup
    free_state(&S); 
    // TODO status = load_state(&S, server_response); cBYE(status);

    free_if_non_null(server_response);
    sleep(1);
    printf("Attempt %d \n", i);
  }

BYE:
  if ( L != NULL ) { lua_close(L); L = NULL; }
  free_if_non_null(server_response);
  free_state(&S); 
  return status;
}
