#include "util.h"

char* concat(const char *s1, const char *s2) {

  int n = strlen(s1) + strlen(s2) + 1;
  char *result = malloc(n * sizeof(char));

  strcpy(result, s1);
  strcat(result, s2);

  for(int i = 0; i < strlen(result); i++) {
  	for(int j = i + 1; result[j] != '\0'; j++) {
  		if(result[j] == result[i]) {
  			for(int k = j; result[k] != '\0'; k++) {
  				result[k] = result[k + 1];
  			}
  		}
  	}
  }

  return result;

}

int index_of(char* str, char c) {

  for (int i = 0; *(str + i) != '\0'; i = i + 1) {
    if (str[i] == c) {
      return i;
    }
  }
  return 0;

}
