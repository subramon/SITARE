#include "q_incs.h"
#include "free_2d_array.h"
#include "c3i_server.h"
#include "c3i_server_aux.h"
void
c3i_server_stats(
    c3i_server_config_t *ptr_C,
    c3i_server_info_t *ptr_I
    )
{
  printf("c3i Server Started at     %" PRIu64 "\n", ptr_I->t_start);
  printf("c3i Server Stopped at     %" PRIu64 "\n", ptr_I->t_stop);
}
void
c3i_server_free_configs(
    c3i_server_config_t *ptr_C
    )
{
  return NULL; 
}