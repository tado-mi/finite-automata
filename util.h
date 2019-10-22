#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fa.h"

// 1 if the fa is not null, and from, to are within bounds, and on is a char in the alphabet or epsilon
// 0 otherwise
extern int is_valid(FA fa, int from, char on, int to);

extern int index_of(FA fa, char inp);

extern char* get_char(FA fa, int j);

// size of the arr: # of non-negative elements in arr
extern int size(FA fa, int* arr);

// small utility to print an arr
extern void print_set(FA fa, int* arr);

// join two sets
extern int join(FA fa, int* arr, int* brr);
