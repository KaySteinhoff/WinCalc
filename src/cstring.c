#include <cstring.h>
#include <stdlib.h>
#include <string.h>

string_t newString(char *str, size_t length)
{
	string_t result = {
		.length = length,
		.capacity = length <= 0 ? 1 : length << 1,
	};

	result.c_str = malloc(result.capacity);
	if(!result.c_str)
		return (string_t){ 0 };

	if(length > 0)
		memcpy(result.c_str, str, length);

	return result;
}

int stringEquals(string_t str1, char *str2, size_t length)
{
	if(str1.length != length || (str1.length > 0 && length > 0 && str1.c_str[0] != str2[0]))
		return 0;
	for(size_t i = 1; i < length; ++i)
		if(str1.c_str[i] != str2[i])
			return 0;
	return 1;
}

int stringContains(string_t str1, char *str2, size_t length)
{
	string_t tmp = {
		.c_str = str2,
		.length = length,
		.capacity = 0
	};

	if(str1.length < length)
		return 0;
	for(size_t i = 0; i < str1.length-length+1; ++i)//Once we reach the point of equal length anything further to the end of str1 can't be equal since it would be shorter. Also string bounds prevent us from checking that deep(not literally but logically)
	{
		if(str1.c_str[i] != str2[0])
			continue;
		if(stringEquals(tmp, str1.c_str + i, length))
			return 1;
	}

	return 0;
}

string_t stringSubstring(string_t str, size_t start, size_t length)
{
	if(length <= 0 || start < 0 || start > str.length-1 || start + length > str.length || str.length - start < length)
		return (string_t) { 0 };

	string_t substr = {
		.length = length,
		.capacity = length << 1,
		.c_str = malloc(length << 1)
	};

	memcpy(substr.c_str, str.c_str + start, length);

	return substr;
}

int stringAppend(string_t *str1, char *str2, size_t length)
{
	if(str1->length+length >= str1->capacity)
	{
		char *tmp = realloc(str1->c_str, str1->capacity + (length << 1));
		if(!tmp)
			return 0;
		str1->c_str = tmp;
		str1->capacity += length << 1;
	}

	memcpy(str1->c_str+str1->length, str2, length);
	str1->length += length;
	return 1;
}

int stringPushBack(string_t *str, char c)
{
	if(str->length == str->capacity)
	{
		char *tmp = realloc(str->c_str, str->capacity << 1);
		if(!tmp)
			return 0;
		str->c_str = tmp;
		str->capacity <<= 1;
	}

	str->c_str[str->length++] = c;
	return 1;
}

int stringPopBack(string_t *str)
{
	if(str->length <= 0)
		return 0;
	str->c_str[--str->length] = '\0';
	return 1;
}

void clearString(string_t *str)
{
	memset(str->c_str, 0, str->length);
	str->length = 0;
}

void freeString(string_t str)
{
	free(str.c_str);
	str.length = -1;
	str.capacity = -1;
}
