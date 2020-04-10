#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FA *FA;

extern FA new_FA(int n, char* sigma);
extern int set_transition(FA fa, int from, char on, int to);
extern int set_accepting(FA fa, int state);

extern int execute(FA fa, char* input);

// #endif
