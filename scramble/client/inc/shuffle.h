#include <stdlib.h>
extern void 
swap_I1(
    int8_t *a, 
    int8_t *b
    );
extern void 
swap(
    int *a, 
    int *b
    );
extern void 
printArray_I1(
    int8_t arr[], 
    int n
    );
extern void 
printArray(
    int arr[], 
    int n
    );
extern void 
randomize_I1(
    struct drand48_data *ptr_rand_buf,
    int8_t arr[], 
    int n 
    );
extern void 
randomize(
    struct drand48_data *ptr_rand_buf,
    int arr[], 
    int n 
    );
extern void 
swap_I2(
    int16_t *a, 
    int16_t *b
    );
extern void 
printArray_I2(
    int16_t arr[], 
    int n
    );
extern void 
randomize_I2(
    struct drand48_data *ptr_rand_buf,
    int16_t arr[], 
    int n 
    );
