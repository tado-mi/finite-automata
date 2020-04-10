#include "fa.h"

struct FA {

  // <Q, Σ, δ, F, q0 >
  int*  Q;
  char* Sigma;
  int*** delta;
  int* F;
  int q0;

  // number of states
  int ROW;
  // number of terminals (and ε)
  int COL;

};

FA new_FA(int n, char* sigma) {

  FA new = malloc(sizeof(struct FA));

  // initialise the set of states
  (*new).Q = malloc(n * sizeof(int));
  for (int i = 0; i < n; i = i + 1) {
    (*new).Q[i] = i;
  }

  // assign the alphabet
  (*new).Sigma = sigma;

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

  // initialise the start state
  (*new).q0 = 0;

  // return the machine
  return new;
}

// util function

int index_of(FA fa, char c) {

  if (c == "ε"[0]) {
    return (*fa).COL - 1;
  }

  char* sigma = (*fa).Sigma;
  for (int i = 0; sigma[i] != '\0'; i = i + 1) {
    if (sigma[i] == c)
      return i;
  }
  return -1;
}

int size(FA fa, int* arr) {
  int len = 0;
  for (int i = 0; i < (*fa).ROW && arr != NULL; i = i + 1) {
    if (arr[i] == -1) {
      break;
    }
    len = i + 1;
  }
  return len;
}

// for printing
char* get_char(FA fa, int j) {

  char* str = "ε";
  if (j == (*fa).COL - 1) {
    return str;
  }
  char* sigma = (*fa).Sigma;
  str = malloc(sizeof(char));
  if (j < strlen(sigma)) {
    str[0] = sigma[j];
  } else {
    str[0] = '?';
  }
  return str;
}

void print_set(FA fa, int* arr) {
  int len = size(fa, arr);
  printf("{");
  for (int i = 0; i < len - 1; i++) {
    printf(" %d,", arr[i]);
  }
  if (len > 0) {
    printf(" %d", arr[len - 1]);
  }
  printf(" }\n");
}

int is_valid(FA fa, int from, char on, int to) {

  if (fa == NULL)
    return 0;

  if (from < 0 || from >= (*fa).ROW || to < 0 || to >= (*fa).ROW)
    return 0;

  int on_index = index_of(fa, on);
  if (on_index < 0 || on_index >= (*fa).COL)
    return 0;

  return 1;

}

int contains(FA fa, int* arr, int el) {
  for (int i = 0; i < size(fa, arr); i = i + 1) {
    if (arr[i] == el)
      return 1;
  }
  return 0;
}

int join(FA fa, int* arr, int* brr) {

  // assuming arrays are sorted
  for (int i = size(fa, arr); i < (*fa).ROW; i = i + 1) {
    int count = 0;
    while (contains(fa, arr, brr[count])) {
      count = count + 1;
    }
    arr[i] = brr[count];
  }

  return 1;

}

// end util function

int set_transition(FA fa, int from, char on, int to) {

  if (!is_valid(fa, from, on, to)) {
    printf("set_transition: δ(%d, %c) = %d\n", from, on, to );
    printf("\terror:\n");
    printf("Q = ");
    print_set(fa, (*fa).Q);
    printf("Σ = %s\n", (*fa).Sigma);
    return 0;
  }

  int on_index = index_of(fa, on);

  int* arr = (*fa).delta[from][on_index];
  for (int i = 0; i < (*fa).ROW; i = i + 1) {
    if (arr[i] == to || arr[i] == -1) {
      arr[i] = to;
      return 1;
    }
  }

  return 0;

}

int set_accepting(FA fa, int state) {

  if (!is_valid(fa, state, (*fa).Sigma[0], 0))
    return 0;
  (*fa).F[state] = 1;

}

int* get_transition(FA fa, int from, char on) {

  // printf("get transition: δ(%d, %c) = ", from, on);

  if (!is_valid(fa, from, on, 0))
    return NULL;

  int* arr = (*fa).delta[from][index_of(fa, on)];
  if (size(fa, arr) < 1)
    return NULL;

  // print_set(fa, arr);
  return arr;

}

int get_accepting(FA fa, int* arr) {

  int len = size(fa, arr);
  if (len < 1)
    return 0;
  for (int i = 0; i < len; i = i + 1) {
    int ind = arr[i];
    if ((*fa).F[ind]) {
      return 1;
    }
  }
  return 0;

}

void print_FA(FA fa) {

  printf("\t<Q, Σ, δ, F, q0 >\n");

  printf("Q = ");
  print_set(fa, (*fa).Q);

  printf("Σ = %s\n", (*fa).Sigma);

  printf("δ = \n");
  for (int i = 0; i < (*fa).ROW; i = i + 1) {
    for (int j = 0; j < (*fa).COL; j = j + 1) {
      int* temp = (*fa).delta[i][j];
      // if (size(fa, temp) < 1) {
      //   continue;
      // }
      printf(" δ(%d, %s) = ", i, get_char(fa, j));
      print_set(fa, temp);
    }
  }

  printf("F = ");
  print_set(fa, (*fa).F);

  printf("q0 = %d\n", (*fa).q0);

}

int execute(FA fa, char* input) {

  printf("consider the following Finite Automata");
  print_FA(fa);

  int* curr = malloc((*fa).ROW * sizeof(int));
  // fill up with -1
  for (int i = 0; i < (*fa).ROW; i = i + 1)
    curr[i] = -1;
  // assign the start state
  curr[0] = (*fa).q0;

  printf("\nstarting execution on the input \"%s\":\n", input);

  while (*input != '\0') {

    int n = size(fa, curr);

    printf("terminal: %c; current %s = ", *input, n > 1? "(set of) state(s)": "state");
    print_set(fa, curr);

    int* new_curr = malloc((*fa).ROW * sizeof(int));
    // fill up with -1
    for (int i = 0; i < (*fa).ROW; i = i + 1)
      new_curr[i] = -1;

    for (int i = 0; i < n; i = i + 1) {
      int* temp = get_transition(fa, curr[i], *input);
      if (temp == NULL)
        continue;
      join(fa, new_curr, temp);

      // consider epsilon transitions
      // temp = get_transition(fa, curr[i], "ε"[0]);
      // printf("\t (set of) state(s) on ε: ");
      // print_set(fa, temp);

    }

    if (size(fa, new_curr) < 1) {
      // no transition
      return 0;
    }

    // change the state
    curr = new_curr;

    // consume the terminal
    input++;

  }

  printf("result:\t");

  int ans = get_accepting(fa, curr);
  printf("%s\n", ans? "accepted" : "rejected");
  return ans;

}
