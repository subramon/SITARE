#ifndef _C3I_SERVER_H
#define _C3I_SERVER_H
#include "lualib.h"
//START_FOR_CDEF
typedef struct _c3i_server_config_t {
  uint32_t nP0; // minimum number of letters in pool 
  uint32_t nU;  // number of users
  int port; 
  char *word_list;
} c3i_server_config_t;
//STOP_FOR_CDEF

typedef struct _c3i_server_info_t {
  c3i_server_config_t *C; // READ  ONLY
  lua_State *L;
  // start for debugging 
  uint64_t t_start;
  uint64_t t_stop;
  int status;
  // stop  for debugging 
} c3i_server_info_t;

extern void * c3i_server( void *arg);
#endif // _C3I_SERVER_H
