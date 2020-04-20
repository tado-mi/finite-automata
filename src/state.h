#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef struct state *state;

extern state new_state(int ID, int sizeof_sigma, int n);
extern state new_state_name(char* name, int ID, int sizeof_sigma, int n);

extern void print_transitions(state this, char* Sigma);
extern void print_state(state this, char* Sigma);

extern int set_transition(state this, int on, state to);
extern int set_transitions(state this, int on, state* to);
extern void set_accepting(state this, int val);

extern state* get_transitions(state this, int on);
extern int get_ID(state this);
extern int is_accepting(state this);

extern void join_states(state* in, state* from, int n);
