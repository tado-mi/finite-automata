#include "util.h"
#include "state.h"
#include "../fa.h"

struct FA {

  state* Q;
  char* Sigma;

  state q0;

  // miscelaneous
  // number of states
  int n;
  // length of the alphabet
  int m;

};

FA new_FA(int n, char* sigma) {

  FA new = malloc(sizeof(struct FA));

  // m = size of the sigma
  int m = (int) strlen(sigma);

  // initialise the set of states
  (*new).Q = malloc(n * sizeof(state));
  for (int i = 0; i < n; i = i + 1) {
    (*new).Q[i] = new_state(i, m, n);
  }

  // assign the alphabet
  (*new).Sigma = sigma;

  /*
  // assign the number of states as the number of rows
  (*new).ROW = n;
  // assign the number of terminals as the number of columns; add one for possible ε - transitions
  (*new).COL = (int) strlen(sigma) + 1;

  // initialize the transition function
  (*new).delta = malloc((*new).ROW * sizeof(int**));
  for (int i = 0; i < (*new).ROW; i = i + 1) {
    (*new).delta[i] = malloc((*new).COL * sizeof(int*));
    for (int j = 0; j < (*new).COL; j = j + 1) {
      (*new).delta[i][j] = malloc((*new).ROW * sizeof(int));
      for (int k = 0; k < n; k = k + 1) {
        (*new).delta[i][j][k] = -1;
      }
    }
  }

  // initialise the set of accepting states F[i] = 1 if Q[i] is an accepting state, 0 otherwise
  (*new).F = malloc(n * sizeof(int));
  for (int i = 0; i < n; i = i + 1) {
    (*new).F[i] = 0;
  }
  */

  // initialise the start state
  (*new).q0 = (*new).Q[0];

  //
  (*new).n = n;
  (*new).m = m;

  // return the machine
  return new;
}

void print_FA(FA fa) {

  printf("\t<Q, Σ, δ, F, q0 >\n");

  printf("Q = {q0, ... , q%d}\n", (*fa).n - 1);

  printf("Σ = %s\n", (*fa).Sigma);

  printf("δ = \n");
  for (int i = 0; i < (*fa).n; i = i + 1) {

    state q = (*fa).Q[i];
    printf("q%d\n", get_ID(q));
    print_transitions(q, (*fa).Sigma);

  }

  printf("F = { ");
  for (int i = 0; i < (*fa).n; i = i + 1) {
    state q = (*fa).Q[i];
    if (is_accepting(q)) {
      printf("q%d ", get_ID(q));
    }
  }
  printf("}\n");
  // print_set(fa, (*fa).F);

  printf("q0 = %d\n", get_ID((*fa).q0));

}

int execute(FA fa, char* input) {

  state* curr = malloc((*fa).n * sizeof(state));
  // fill up with -1
  for (int i = 0; i < (*fa).n; i = i + 1) {
    curr[i] = NULL;
  }
  // assign the start state
  curr[0] = (*fa).q0;

  printf("\nstarting execution on the input \"%s\":\n", input);

  for (int c = 0; input[c] != '\0'; c = c + 1) {

    printf("terminal: %c; current set of states { ", input[c]);
    for (int i = 0; curr[i] != NULL; i = i + 1) {
        printf("q%d ", get_ID(curr[i]));
    }
    printf("}\n");

    state* new_curr = malloc((*fa).n * sizeof(state));
    for (int i = 0; i < (*fa).n; i = i + 1) {
      new_curr[i] = NULL;
    }

    for (int i = 0; i < (*fa).n && curr[i] != NULL; i = i + 1) {
      state* temp = get_transitions(curr[i], c);
      if (temp == NULL)
        continue;
      join_states(new_curr, temp, (*fa).n);
    }

    if (new_curr[0] == NULL) {

      // no transition
      printf("result:\t rejected");
      return 0;

    }

    // change the state
    curr = new_curr;

  }

  printf("result:\t");

  int ans = 0;
  for (int i = 0; curr[i] != NULL; i = i + 1) {
    if (is_accepting(curr[i])) {
      ans = 1;
      break;
    }
  }
  printf("%s\n", ans? "accepted" : "rejected");
  return ans;

}


FA atomic_FA(char* Sigma) {

  FA this = new_FA(2, Sigma);

  // char c = Sigma[0]
  int index = 0;

  // 0 -- c --> 1
  state STATE_0 = (*this).Q[0];
  state STATE_1 = (*this).Q[1];

  set_transition(STATE_0, index, STATE_1);
  set_accepting(STATE_1, 1);

  return this;

}

state* get_accepting_states(FA fa) {

  int n = (*fa).n + 1;
  state* accepting = malloc(n * sizeof(state));
  for (int i = 0; i < n; i = i + 1) {

    accepting[i] = NULL;

  }

  int count = 0;
  for (int i = 0; i < (*fa).n; i = i + 1) {

    state q = (*fa).Q[i];
    if (is_accepting(q)) {

      accepting[count] = q;
      count++;

    }

  }
  return accepting;
}

void copy_FA(FA target, FA source, int offset) {

  int source_0 = get_ID((*source).q0);
  int target_0 = get_ID((*target).q0);

  for (int i = 0; i < (*source).n; i = i + 1) {

    state q = (*source).Q[i];
    int from = (i == source_0)? target_0: i + offset;

    for (int j = 0; j < (*source).m; j = j + 1) {

      char c = (*source).Sigma[j];
      int on = index_of((*target).Sigma, c);

      state* transitions = get_transitions(q, j);
      for (int x = 0; x < (*source).n; x = x + 1) {

        state t = transitions[x];
        if (t == NULL) {
          break;
        }

        int to = get_ID(t);
        to = (to == source_0)? target_0: to + offset;

        set_transition((*target).Q[i], on, (*target).Q[to]);
      }

    }

    if (is_accepting(q)) {
      set_accepting((*target).Q[from], 1);
    }

  }

}

FA union_FA(FA FA_1, FA FA_2) {

  // -1 since starting states will be merged
  int n = (*FA_1).n + (*FA_2).n - 1;
  int offset = (*FA_1).n - 1;

  char* Sigma = concat((*FA_1).Sigma, (*FA_2).Sigma);

  FA this = new_FA(n, Sigma);

  // copy FA #1 merging the starting states, with no offset
  copy_FA(this, FA_1, 0);

  // copy FA #2 merging the starting states, with an offset
  copy_FA(this, FA_2, offset);

  return this;

}

state* offset_states(FA this, state* source, int offset) {

  state* answer = malloc( (*this).n * sizeof(state) );

  for (int i = 0; source[i] != NULL; i = i + 1) {

    state temp = source[i];
    int index = get_ID(temp) + offset;
    answer[i] = (*this).Q[index];
  }

  return answer;

}

FA concat_FA(FA FA_1, FA FA_2) {

  int n = (*FA_1).n + (*FA_2).n - 1;
  int offset = (*FA_1).n - 1;

  char* Sigma = concat((*FA_1).Sigma, (*FA_2).Sigma);

  FA this = new_FA(n, Sigma);

  // copy FA #1 merging the starting states, with no offset
  copy_FA(this, FA_1, 0);
  state* accepting = get_accepting_states(this);
  for (int k = 0; accepting[k] != NULL; k = k + 1) {

    // p is an accepting state of the FA this
    state p = accepting[k];
    // undo the accepting status of p
    set_accepting(p, 0);

  }

  // note: all the accepting states of FA_1 should act as a starting state for FA_2
  state q = (*FA_2).q0;
  for (int j = 0; j < (*FA_2).m; j = j + 1) {

    char c = (*FA_2).Sigma[j];
    // transitions from FA #2's starting state on char c
    state* transitions_0 = get_transitions(q, j);

    for (int k = 0; accepting[k] != NULL; k = k + 1) {

      // p is an accepting state of the FA this
      state p = accepting[k];
      // undo the accepting status of p
      set_accepting(p, 0);

      // offset them to align with the fa this
      state* transitions = offset_states(this, transitions_0, offset);
      // if there is a transition q to q, ensure it translates to a transition from p to p, not to (0 + offset)
      for (int r = 0; r < (*this).n; r = r + 1) {

        state temp = transitions[r];
        if (temp == NULL) {
          break;
        }

        if (get_ID(temp) == offset) {
          transitions[r] = p;
          break;
        }

      }

      // transfer all of q's transitions on c into p
      int index = index_of((*this).Sigma, c);
      set_transitions(p, index, transitions);

    }

  }

  // copy the rest of the transitions of FA #2
  for (int i = 1; i < (*FA_2).n; i = i + 1) {

    q = (*FA_2).Q[i];
    state from = (*this).Q[get_ID(q) + offset];

    for (int j = 0; j < (*FA_2).m; j = j + 1) {

      char c = (*FA_2).Sigma[j];
      // transitions from FA #2's starting state on char c
      state* transitions = get_transitions(q, j);

      // offset the transitions
      transitions = offset_states(this, transitions, offset);

      // transfer all of q's transitions on c into p
      int index = index_of((*this).Sigma, c);
      set_transitions(from, index, transitions);

    }

    if (is_accepting(q)) {

        set_accepting(from, 1);

    }

  }

  return this;

}
