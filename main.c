#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "fa.h"
#include "construct.h"

int main() {

  FA fa = get_FA("a.b|c");
  execute(fa, "ab");

}
