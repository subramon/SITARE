extern int
c3i_server_proc_req(
    int req_type,
    const char *const api,
    const char *args,
    const char *body,
    void *W,
    char *outbuf, // [sz_outbuf] 
    size_t sz_outbuf,
    char *errbuf, // [sz_outbuf] 
    size_t sz_errbuf,
    web_response_t *ptr_web_response
    );
