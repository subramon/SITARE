// https://www.geeksforgeeks.org/shuffle-a-given-array-using-fisher-yates-shuffle-algorithm/
// C Program to shuffle a given array

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include "shuffle.h"

// A utility function to swap to integers
void 
swap(
    int *a, 
    int *b
    )
{
  int temp = *a;
  *a = *b;
  *b = temp;
}

void 
swap_I1(
    int8_t *a, 
    int8_t *b
    )
{
  int8_t temp = *a;
  *a = *b;
  *b = temp;
}
// A utility function to print an array
void 
printArray_I1(
    int8_t arr[], 
    int n
    )
{
  for (int i = 0; i < n; i++) {
    printf("%c ", arr[i]);
  }
  printf("\n");
}

void 
printArray(
    int arr[], 
    int n
    )
{
  for (int i = 0; i < n; i++) {
    printf("%d ", arr[i]);
  }
  printf("\n");
}

// A function to generate a random permutation of arr[]
void 
randomize(
    int arr[], 
    int n 
    )
{
  // Use a different seed value so that we don't get same
  // result each time we run this program
  srand((unsigned int)time(NULL));

  // Start from the last element and swap one by one. We don't
  // need to run for the first element that's why i > 0
  for (int i = n-1; i > 0; i--) {
    // Pick a random index from 0 to i
    int j = rand() % (i+1);

    // Swap arr[i] with the element at random index
    swap(&arr[i], &arr[j]);
  }
}
void 
randomize_I1(
    int8_t arr[], 
    int n 
    )
{
  // Use a different seed value so that we don't get same
  // result each time we run this program
  srand((unsigned int)time(NULL)); // TODO P2 Initialize this once outside 

  // Start from the last element and swap one by one. We don't
  // need to run for the first element that's why i > 0
  for (int i = n-1; i > 0; i--) {
    // Pick a random index from 0 to i
    int j = rand() % (i+1);

    // Swap arr[i] with the element at random index
    swap_I1(&arr[i], &arr[j]);
  }
}

#undef UNIT_TEST
#ifdef UNIT_TEST
// Driver program to test above function.
int 
main()
{
  int arr[] = {1, 2, 3, 4, 5, 6, 7, 8};
  int n = sizeof(arr)/ sizeof(arr[0]);
  randomize(arr, n);
  printArray(arr, n);

  char arr_I1[] = {'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H'};
  randomize_I1(arr_I1, n);
  printArray_I1(arr_I1, n);

  return 0;
}
#endif
