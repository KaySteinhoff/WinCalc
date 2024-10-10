#ifndef CALC_PARSER_H_
#define CALC_PARSER_H_

#include "lexer.h"

typedef enum
{
	Unresolved,
	Resolved
}NodeStatus;

typedef struct Node
{
	NodeStatus status;
	char type;
	double result;
	struct Node *left;
	struct Node *right;
}Node;

Node* ParseTokens(TokenList list);
double ResolveNode(Node *node);
char* PrintNode(Node *node);
void freeNode(Node *node);

#endif