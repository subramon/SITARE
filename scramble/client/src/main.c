#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdbool.h>
#include <unistd.h>
#include "q_macros.h"
#include "free_2d_array.h"
#include "game_state.h"
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
#define CMD_LEN 1024-1
  char cmd[CMD_LEN+1];  memset(cmd, 0, CMD_LEN+1);
  game_state_t S; memset(&S, 0, sizeof(game_state_t));
  char *server_response = NULL;

  if ( argc != 4 ) { go_BYE(-1); }
  int user = atoi(argv[1]);
  const char * const server = argv[2]; 
  int port = atoi(argv[3]);

  for ( int i = 0; ; i++ ) { 
    // Get state from server in server_response
    sprintf(cmd, "http://%s:%d/GetState", server, port);
    // Load state 
    free_state(&S); 
    status = load_state(&S, server_response); cBYE(status);

    free_if_non_null(server_response);
    sleep(1);
    printf("Attempt %d \n", i);
  }

BYE:
  free_if_non_null(server_response);
  free_state(&S); 
  return status;
}
