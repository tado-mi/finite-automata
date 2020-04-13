#include "state.h"

struct state {

  char* name;
  int ID;
  // transitions[i]: where one can go from current state on i, where i = sigma 'on'/ε
  state** transitions;

  // current state is accepting
  int accepting;

  int m, n;

};

// constructors

state new_state(int ID, int sizeof_sigma, int n) {

  state new = malloc(sizeof(struct state));
  (*new).ID = ID;
  (*new).name = NULL;
  (*new).accepting = 0;

  (*new).m = sizeof_sigma;
  (*new).n = n;

  (*new).transitions = malloc((*new).m * sizeof(state*));
  for (int j = 0; j < (*new).m; j = j + 1) {
    (*new).transitions[j] = malloc(n * sizeof(state));
    for (int i = 0; i < n; i = i + 1) {
      (*new).transitions[j][i] = NULL;
    }
  }

  return new;

}

state new_state_name(char* name, int ID, int sizeof_sigma, int n) {

  state new = new_state(ID, sizeof_sigma, n);
  (*new).name = name;

  return new;

}

// printing functions

void print_transitions(state this, char* Sigma) {

  for (int j = 0; j < (*this).m; j = j + 1) {

    printf("\ton %c: {", Sigma[j]);

    for (int i = 0; i < (*this).n; i = i + 1) {

      state s = (*this).transitions[j][i];

      if (s == NULL) {

        printf("}\n");
        break;

      }

      printf("q%d ", (*s).ID);

    }

  }

}

void print_state(state this, char* Sigma) {

  printf("state: \t[ID: %d, name: %s]\n", (*this).ID, (*this).name == NULL? "": (*this).name);
  printf("\tis accepting: %d\n", (*this).accepting);
  printf("\tδ:\n");
  print_transitions(this, Sigma);


}

// setting functions

int set_transition(state this, int on, state to) {

  // create a temp pointer for the convenience of iteration
  state* temp = (*this).transitions[on];

  for (int i = 0; i <= (*this).n; i = i + 1) {

    if (temp[i] == NULL) {
      (*this).transitions[on][i] = to;
      return 1;
    }

    if ((*temp[i]).ID == (*to).ID) {
      // duplicate state
      return 0;
    }

  }

  // something went wrong
  return 0;

}

int set_transitions(state this, int on, state* to) {

  int result = 1;

  for (int t = 0; t <= (*this).n; t = t + 1) {

    if (to[t] == NULL) {
      break;
    }

    int success = set_transition(this, on, to[t]);
    if (!success) {
      result = 0;
      printf("unable to add q%d\n", (*to[t]).ID);
    }

  }

  return result;

}

void set_accepting(state this, int val) {

  (*this).accepting = val;

}

// getting functions

state* get_transitions(state this, int on) {

  if (on >= (*this).m) {

    printf("no transition at state q%d on char %d\n", (*this).ID, on);
    return NULL;

  }

  return (*this).transitions[on];

}

int get_ID(state this) {

  return (*this).ID;

}

int is_accepting(state this) {

  return (*this).accepting;

}
