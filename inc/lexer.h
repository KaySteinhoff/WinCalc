#ifndef CALC_LEXER_H_
#define CALC_LEXER_H_

#include <cstring.h>
#include <stddef.h>

typedef enum
{
	NUMBER = 0,
	PLUS = 1,
	MINUS = 2,
	ASTERIC = 4,
	SLASH = 8,
	LBRACKET = 16,
	RBRACKET = 32
}LEXTokenType;

typedef struct
{
	LEXTokenType type;
	string_t value;
}Token;

typedef struct
{
	Token *tokens;
	size_t length;
	size_t capacity;
}TokenList;

TokenList LexEquation(string_t equation);

#endif