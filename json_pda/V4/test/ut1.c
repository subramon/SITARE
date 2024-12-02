#include <stdio.h>
#include "bloom.h"
int
main(
    int argc,
    char **argv
    )
{
  int status = 0;

  BloomFilter bf;
  /*  elements = 10;
      false positive rate = 5% */
  bloom_filter_init(&bf, 10, 0.05);
  bloom_filter_add_string(&bf, "test");
  if (bloom_filter_check_string(&bf, "test") == BLOOM_FAILURE) {
    printf("'test' is not in the Bloom Filter\n");
  } else {
    printf("'test' is in the Bloom Filter\n");
  }
  if (bloom_filter_check_string(&bf, "blah") == BLOOM_FAILURE) {
    printf("'blah' is not in the Bloom Filter!\n");
  } else {
    printf("'blah' is in th Bloom Filter\n");
  }
  bloom_filter_stats(&bf);
  bloom_filter_destroy(&bf);

BYE: 
  return status;
}
