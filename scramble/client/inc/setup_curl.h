// for storing response from curl 
typedef struct _curl_userdata_t { 
  char *base;
  size_t size;
  size_t offset;
} curl_userdata_t;

extern int
setup_curl(
    void *userdata,
    CURL **ptr_ch
    );
