#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <lexer.h>

static string_t plus = { .c_str = "+" };
static string_t minus = { .c_str = "-" };
static string_t asteric = { .c_str = "*" };
static string_t slash = { .c_str = "/" };
static string_t lbracket = { .c_str = "(" };
static string_t rbracket = { .c_str = ")" };

int AppendToken(TokenList *list, string_t value, LEXTokenType type)
{
	if(list->length == list->capacity)
	{
		Token *tmp = realloc(list->tokens, (list->capacity << 1)*sizeof(Token));
		if(!tmp)
			return 0;
		list->tokens = tmp;
		list->capacity <<= 1;
	}

	list->tokens[list->length++] = (Token){
		.value = value,
		.type = type
	};

	return 1;
}

int ReadNumber(string_t input, int startIndex, string_t *output)
{
	memset(output->c_str, 0, output->length);
	output->length = 0;
	int i = startIndex;

	for(; i < input.length; ++i)
	{
		if((input.c_str[i] >= '0' && input.c_str[i] <= '9') || input.c_str[i] == ' ' || (input.c_str[i] == '-' && i == startIndex))
			continue;

		if(input.c_str[i] == '.')
		{
			if(input.c_str[i-1] != '.')
				continue;
		}
		break;
	}

	string_t subStr = stringSubstring(input, startIndex, i-startIndex);
	output->c_str = subStr.c_str;
	output->length = subStr.length;

	return output->length;
}

TokenList LexEquation(string_t equation)
{
	int errorOccured = 0;
	string_t number;

	TokenList list = {
		.length = 0,
		.capacity = 1,
		.tokens = malloc(sizeof(Token))
	};
	if(!list.tokens)
		return (TokenList){ 0 };

	for(int i = 0; i < equation.length; ++i)
	{
		switch(equation.c_str[i])
		{
			case ' ':
				break;
			case '+':
				if(AppendToken(&list, plus, PLUS))
					break;
				errorOccured = 1;
				break;
			case '-':
				if(AppendToken(&list, minus, MINUS))
					break;
				errorOccured = 1;
				break;
			case '*':
				if(AppendToken(&list, asteric, ASTERIC))
					break;
				errorOccured = 1;
				break;
			case '/':
				if(AppendToken(&list, slash, SLASH))
					break;
				errorOccured = 1;
				break;
			case '(':
			case '{':
			case '[':
				if(AppendToken(&list, lbracket, LBRACKET))
					break;
				errorOccured = 1;
				break;
			case ')':
			case '}':
			case ']':
				if(AppendToken(&list, rbracket, RBRACKET))
					break;
				errorOccured = 1;
				break;
			case '0':
			case '1':
			case '2':
			case '3':
			case '4':
			case '5':
			case '6':
			case '7':
			case '8':
			case '9':
				number = newString("", 0);
				i += ReadNumber(equation, i, &number)-1;
				if(AppendToken(&list, number, NUMBER))
					break;
				errorOccured = 1;
				break;
			default:
				errorOccured = 1;
				break;
		}
	}

	if(errorOccured)
	{
		free(list.tokens);
		return (TokenList) { 0 };
	}

	return list;
}