#ifndef GBLOL_STRING_H_
#define GBLOL_STRING_H_

#include <stddef.h>

typedef struct
{
	char *c_str;
	size_t length;
	size_t capacity;
}string_t;

string_t newString(char *str, size_t length);
int stringEquals(string_t str1, char *str2, size_t length);
int stringContains(string_t str1, char *str2, size_t length);
string_t stringSubstring(string_t str, size_t start, size_t length);
int stringAppend(string_t *str1, char *str2, size_t length);
int stringPushBack(string_t *str, char c);
int stringPopBack(string_t *str);
void clearString(string_t *str);
void freeString(string_t str);

#endif