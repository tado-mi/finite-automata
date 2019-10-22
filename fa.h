#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct FA *FA;

extern FA new_FA(int n, char* sigma);
extern void print_FA(FA fa);
extern int execute(FA fa, char* input);
extern int set_transition(FA fa, int from, char on, int to);
extern int set_accepting(FA fa, int state);

extern int row(FA fa);
extern int col(FA fa);
extern char* alphabet(FA fa);
