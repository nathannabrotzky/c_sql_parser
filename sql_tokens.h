#ifndef SQL_TOKENS
#define SQL_TOKENS

typedef struct {
  char type[32];
  char value[128];
} sqlToken;

void toUppercase(char *str);

bool is_stop(char c);

bool is_punctuation(char c);

bool is_keyword(char buffer[100]);

sqlToken* sql_lexer(const char *filepath, int *count);

#endif