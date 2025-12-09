#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <ctype.h>
#include "sql_tokens.h"

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

sqlToken* sql_lexer(const char *filepath, int *count) {
  sqlToken *result = NULL;
  int current_size = 0;
  int capacity = 0;
  FILE *filePointer = fopen(filepath, "r");
  if (filePointer == NULL) {
    printf("Error opening the file.\n");
    return 1;
  }
  if (fseek(filePointer, 0, SEEK_END) == -1) {
    printf("Error finding end of file.\n");
    return 1;
  }
  long int size = ftell(filePointer);
  if (size == -1) {
    printf("Error getting the size of file.\n");
    return 1;
  }
  if (fseek(filePointer, 0, SEEK_SET) == -1) {
    printf("Error finding beginning of file.\n");
    return 1;
  }
  char buffer[100];
  int offset = 0;
  int layer = 0;
  for (long int i = 0; i < size; i++) {
    char c = fgetc(filePointer);
    if (c == EOF) {
      printf("Error getting next character.\n");
      return 1;
    }
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
        if (current_size >= capacity) {
          capacity = (capacity == 0) ? 2 : capacity * 2; 
          int *temp_arr = (int *)realloc(result, capacity * sizeof(sqlToken));
          if (temp_arr == NULL) {
            fprintf(stderr, "Memory reallocation failed!\n");
            free(result);
            return 1;
          }
          result = temp_arr;
          printf("Reallocated to capacity: %zu\n", capacity);
        }
        result[current_size++] = newToken;
        printf("%s %s\n",newToken.type, newToken.value);
      }
      buffer[0] = '\0';
      offset = i + 1;
      continue;
    }
    buffer[i - offset] = c;
    buffer[i - offset + 1] = '\0';
  }
  *count = current_size;
  if (fclose(filePointer) == EOF) {
    print("Error closing the file.\n");
    return 1;
  }
  return result;
}
