#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdint.h>
#include <pthread.h>

#include "q_macros.h"
#include "bridge_read_configs.h"
#include "mk_lua_state.h"
#include "bridge_init.h"
// for webserver
#include "web_struct.h"
#include "webserver.h"
#include "c3i_server.h"
#include "c3i_server_aux.h"
#include "c3i_server_get_req.h"
#include "c3i_server_proc_req.h"

int g_halt_called; 

int
main(
    int argc,
    char **argv
    )
{
  int status = 0;
  // for c3i server
  c3i_server_config_t Cc3i; memset(&Cc3i, 0, sizeof(c3i_server_config_t));
  pthread_t l_c3i_server; memset(&l_c3i_server, 0, sizeof(pthread_t));
  c3i_server_info_t Ic3i; memset(&Ic3i, 0, sizeof(c3i_server_info_t));
  web_info_t winfo; memset(&winfo, 0, sizeof(web_info_t));
  g_halt_called = 0;
  //-----------------------------
  if ( argc != 2 ) {  go_BYE(-1); }
  const char * const c3i_cfg_file  = argv[1];
  status = bridge_read_configs(c3i_cfg_file, &Cc3i, "read_c3i_server"); 
  cBYE(status);
  // Initialize game 
  Ic3i.L = mk_lua_state(); if ( Ic3i.L == NULL ) { go_BYE(-1); } 
  status = luaL_dostring(Ic3i.L, "require 'game'");
  status = bridge_init(Ic3i.L, Cc3i.nP0, Cc3i.nU, Cc3i.word_list); 
  cBYE(status);

  Ic3i.C = &Cc3i;
  winfo.port        = Cc3i.port;
  winfo.get_req_fn  = c3i_server_get_req;
  winfo.proc_req_fn = c3i_server_proc_req;
  winfo.W = &Ic3i;
  
  status = pthread_create(&l_c3i_server, NULL, &webserver, &winfo);
  cBYE(status);

  // Wait for threads to finish 
  pthread_join(l_c3i_server, NULL);
  if ( Ic3i.status != 0 ) { go_BYE(-1); }
  // Check all queues should be empty at end
  c3i_server_stats(&Cc3i, &Ic3i); cBYE(status);  

  fprintf(stdout, "%s completed successfully\n", argv[0]);
BYE:
  // cleanup 
  c3i_server_free_configs(&Cc3i);
  c3i_server_free_info(&Ic3i);
  return status;
}
