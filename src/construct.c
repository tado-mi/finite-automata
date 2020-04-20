#include "../construct.h"

FA build(TREE tree) {

  if (tree == NULL) {
    return NULL;
  }

  TREE* children = get_children(tree);

  char* focus = get_label(tree);

  if (strcmp(focus, "|") == 0) {

    // first FA
    FA fa_0 = build(children[0]);
    // second FA
    FA fa_1 = build(children[1]);

    return union_FA(fa_0, fa_1);

  }

  else if (strcmp(focus, ".") == 0) {

    // first FA
    FA fa_0 = build(children[0]);
    // second FA
    FA fa_1 = build(children[1]);

    return concat_FA(fa_0, fa_1);

  }

  // else if (strcmp(focus, "*") == 0) {
  //   printf("(closure ");
  // }

  else {
    return atomic_FA(focus);
  }

}

FA get_FA(char* regex) {

  TREE tree = get_tree(regex);
  FA fa = build(tree);

  printf("given regular expression \"%s\", we compute FA: ", regex);
  print_FA(fa);

  return fa;

}
