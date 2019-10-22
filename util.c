#include "util.h"

int index_of(FA fa, char c) {
  char* sigma = alphabet(fa);
  if (c == -50) {
    return (int) strlen(sigma);
  }
  for (int i = 0; sigma[i] != '\0'; i = i + 1) {
    if (sigma[i] == c)
      return i;
  }
  return -1;
}

int size(FA fa, int* arr) {
  int len = 0;
  for (int i = 0; i < row(fa); i = i + 1) {
    if (arr[i] == -1) {
      break;
    }
    len = i + 1;
  }
  return len;
}

char* get_char(FA fa, int j) {
  char* sigma = alphabet(fa);
  char* str = "ε";
  if (j == strlen(sigma)) {
    return str;
  }
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

  if (from < 0 || from >= row(fa) || to < 0 || to >= row(fa))
    return 0;

  int on_index = index_of(fa, on);
  if (on_index < 0 || on_index >= col(fa))
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
  for (int i = size(fa, arr); i < row(fa); i = i + 1) {
    int count = 0;
    while (contains(fa, arr, brr[count])) {
      count = count + 1;
    }
    arr[i] = brr[count];
  }

  return 1;

}
