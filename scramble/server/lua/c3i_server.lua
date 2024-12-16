 return [[ 

typedef struct _c3i_server_config_t {
  uint32_t nP0;
  uint32_t nU;
  int port;
} c3i_server_config_t;

typedef struct _c3i_server_info_t {
  c3i_server_config_t *C;

  uint64_t t_start;
  uint64_t t_stop;
  int status;

} c3i_server_info_t;

 ]] 
