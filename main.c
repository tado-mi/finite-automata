#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fa.h"

#define epsilon "ε"[0]

int main() {

  FA new = new_FA(2, "01");
  set_transition(new, 0, '0', 0);
  set_transition(new, 0, '0', 1);
  set_transition(new, 0, '1', 0);
  set_transition(new, 1, '0', 0);
  set_transition(new, 1, '1', 1);
  set_accepting(new, 0);


  execute(new, "1111");

}
