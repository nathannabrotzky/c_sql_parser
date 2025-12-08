#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>

void toUppercase(char *str) {
    int i = 0;
    for (int i = 0; i <= sizeof(str)/sizeof(str[0]); i++) {
      if (str[i] == '\0') {
        break;
      }
      str[i] = toupper((unsigned char)str[i]);
    }
}

bool is_stop(char c) {
  const char stops[5] = " ;,";
  for (int i = 0; i < 5; i++) {
    if (stops[i] == c) {
      return true;
    }
  }
  return false;
}

bool is_punctuation(char c) {
  const char stops[5] = "()";
  for (int i = 0; i < 5; i++) {
    if (stops[i] == c) {
      return true;
    }
  }
  return false;
}

bool is_keyword(char buffer[100]) {
  const char *keywords[] = {
    "SELECT", "FROM", "GROUP", "BY", "AS", "SUM"
  };
  toUppercase(buffer);
  for (int i = 0; i < sizeof(keywords)/sizeof(keywords[0]); i++) {
    if (strcmp(buffer, keywords[i]) == 0) {
      return true;
    }
  }
  return false;
};

typedef struct {
  char type[32];
  char value[128];
} sqlToken;

int main(void) {
    FILE *filePointer = fopen("test.sql", "r");
    fseek(filePointer, 0, SEEK_END);
    long int size = ftell(filePointer);
    fseek(filePointer, 0, SEEK_SET);
    char buffer[100];
    int offset = 0;
    int layer = 0;
    for (long int i = 0; i < size; i++) {
      char c = fgetc(filePointer);
      if (c == '(') {
        layer++;
      } else if (c == ')') {
        layer--;
      }
      bool stopFlag = is_stop(c);
      bool puncFlag = is_punctuation(c);
      if ((stopFlag || puncFlag) && ((c == '(' && layer == 1)|| layer == 0)) {
        if (buffer[0] != '\0') {
          sqlToken newToken;
          strcpy(newToken.type,(is_keyword(buffer)? "KEYWORD" : "IDENTIFIER"));
          strcpy(newToken.value,buffer);
          printf("%s %s\n",newToken.type, newToken.value);
        }
        buffer[0] = '\0';
        offset = i + 1;
        continue;
      }
      buffer[i - offset] = c;
      buffer[i - offset + 1] = '\0';
    }
    fclose(filePointer);
    return 0;
}
