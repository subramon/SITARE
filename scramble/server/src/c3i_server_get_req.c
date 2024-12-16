#include <string.h>
#include <stdio.h>
#include "c3i_server_get_req.h"
int 
c3i_server_get_req(
    const char *api
    )
{
  if (strcasecmp(api, "Ignore") == 0) {
    return Ignore;
  }
  //-----------------------------
  else if (strcasecmp(api, "Halt") == 0) {
    return Halt;
  }
  //-----------------------------
  else if (strcasecmp(api, "favicon.ico") == 0) {
    return Favicon;
  }
  //--------------------------------------------------
  else if (strcasecmp(api, "Pass") == 0) {
    return Pass;
  }
  //-----------------------------
  else if (strcasecmp(api, "AddLetter") == 0) {
    return AddLetter;
  }
  //-----------------------------
  else if (strcasecmp(api, "MakeWord") == 0) {
    return MakeWord;
  }
  //-----------------------------
  else if (strcasecmp(api, "GetState") == 0) {
    return GetState;
  }
  //-----------------------------
  else {
    fprintf(stderr,  "Unknown API = %s \n", api);
    return WebUndefined;
  }
}
