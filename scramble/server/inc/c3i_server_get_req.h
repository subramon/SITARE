#ifndef __C3I_GET_REQ_TYPE_H
#define __C3I_GET_REQ_TYPE_H
// Request type of 0 => undefined. See handler.c 
#define  WebUndefined 0 // By convention, this must always be there 

#define Halt          1 
#define Ignore        2
#define Favicon       3
#define Pass          4
#define AddLetter     5
#define MakeWord      6
#define GetState      7
extern int 
c3i_server_get_req(
    const char *api
    );
#endif //  __C3I_GET_REQ_TYPE_H
