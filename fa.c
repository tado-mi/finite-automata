#include "fa.h"

struct FA {

  // <Q, Σ, δ, F, q0 >
  int*  Q;
  char* Sigma;
  int** delta;
  int** F;
  int q0;

};

FA new_FA() {
  FA new = malloc(sizeof(FA));
  return new;
}
