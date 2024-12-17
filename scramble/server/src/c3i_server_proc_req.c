#include "q_incs.h"
#include "web_struct.h" 
#include "c3i_server.h" 
#include "c3i_server_get_req.h" 
#include "bridge_get_state.h" 
#include "bridge_add_letter.h" 
#include "bridge_make_word.h" 
#include "c3i_server_proc_req.h" 

extern int g_halt_called; 
int
c3i_server_proc_req(
    int req_type,
    const char *const api,
    const char *args,
    const char *body,
    void *W,
    char *outbuf, // [sz_outbuf] size_t sz_outbuf,
    size_t sz_outbuf,
    char *errbuf, // [sz_outbuf] 
    size_t sz_errbuf,
    web_response_t *ptr_web_response
    )
{
  int status = 0;
  c3i_server_info_t *Ic3i = NULL; 
  c3i_server_config_t *Cc3i = NULL;
  char *out_str = NULL;

  if ( W == NULL ) { go_BYE(-1); }
  Ic3i = (c3i_server_info_t *)W; if ( Ic3i == NULL ) { go_BYE(-1); }
  Cc3i = Ic3i->C;                if ( Cc3i == NULL ) { go_BYE(-1); }
  lua_State *L = Ic3i->L;        if ( L == NULL ) { go_BYE(-1); }
  //-----------------------------------------
  switch ( req_type ) {
    case WebUndefined :
      go_BYE(-1);
      break;
      //--------------------------------------------------------
    case Ignore :  
      sprintf(outbuf, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case Favicon :  
      sprintf(outbuf, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case Halt :  
      {
        int itmp = 1; __atomic_store(&g_halt_called, &itmp, 0);
        sprintf(outbuf, "{ \"%s\" : \"OK\" }", api);
      }
      break;
      //--------------------------------------------------------
    case Pass :  
      sprintf(outbuf, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case AddLetter :  
      {
        int nP;
        status = bridge_add_letter(L, &nP); cBYE(status); 
        sprintf(outbuf, "{ \"NumInPool\" : \"%d\" }", nP);
      }
      break;
      //--------------------------------------------------------
    case MakeWord :  
      status = bridge_make_word(L, args); cBYE(status);
      sprintf(outbuf, "{ \"%s\" : \"OK\" }", api);
      break;
      //--------------------------------------------------------
    case GetState :  
      status = bridge_get_state(L, &out_str); cBYE(status); 
      strncpy(outbuf, out_str, sz_outbuf);
      break;
      //--------------------------------------------------------
    default : 
      go_BYE(-1);
      break;
  }
BYE:
  free_if_non_null(out_str);
  return status ;
}
