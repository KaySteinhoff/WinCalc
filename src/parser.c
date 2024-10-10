#include <parser.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// factor => number
// grouping => "(" <expr> ")"
// expr => <term> {("+" | "-") <term>}
// term => (<facor>  | grouping) {("*" | "/") (<factor> | grouping)}

Node* createNode(void)
{
	return malloc(sizeof(Node));
}

Node* Factor(TokenList list, int *idx);
Node* Term(TokenList list, int *idx);
Node* Expression(TokenList list, int *idx);

Node* Factor(TokenList list, int *idx)
{
	if(list.tokens[*idx].type == LBRACKET)
	{
		*idx = *idx + 1;
		Node *expr = Expression(list, idx);
		if(list.tokens[*idx].type != RBRACKET)
		{
			freeNode(expr);
			return NULL;
		}
		*idx = *idx + 1;
		return expr;
	}

	Node *node = createNode();
	if(!node)
		return NULL;

	if(list.tokens[*idx].type == NUMBER)
	{
		node->type = '\0';
		node->status = Resolved;
		node->left = NULL;
		node->right = NULL;
		sscanf(list.tokens[*idx].value.c_str, "%lf", &node->result);
		*idx = *idx + 1;
	}else
	{
		freeNode(node);
		return NULL;
	}

	return node;
}

Node* Term(TokenList list, int *idx)
{
	Node *node = Factor(list, idx);
	if(!node)
		return NULL;

	while(list.tokens[*idx].type == ASTERIC || list.tokens[*idx].type == SLASH)
	{
		Node *next = createNode();
		if(!next)
		{
			freeNode(node);
			return NULL;
		}
		next->type = list.tokens[*idx].type == ASTERIC ? '*' : '/';
		*idx = *idx + 1;
		next->status = Unresolved;
		next->left = node;
		next->right = Factor(list, idx);

		node = next;
	}

	return node;
}

Node* Expression(TokenList list, int *idx)
{
	Node *node = Term(list, idx);
	if(!node)
		return NULL;

	while(list.tokens[*idx].type == PLUS || list.tokens[*idx].type == MINUS)
	{
		Node *next = createNode();
		if(!next)
		{
			freeNode(node);
			return NULL;
		}
		next->type = list.tokens[*idx].type == PLUS ? '+' : '-';
		*idx = *idx + 1;
		next->status = Unresolved;
		next->left = node;
		next->right = Term(list, idx);

		node = next;
	}

	return node;
}

Node* ParseTokens(TokenList list)
{
	int idx = 0;
	return Expression(list, &idx);
}

double ResolveNode(Node *node)
{
	if(node->left->status == Unresolved)
		ResolveNode(node->left);
	if(node->right->status == Unresolved)
		ResolveNode(node->right);

	switch(node->type)
	{
		case '-':
			node->result = node->left->result - node->right->result;
			break;
		case '*':
			node->result = node->left->result * node->right->result;
			break;
		case '/':
			node->result = node->left->result / node->right->result;
			break;
		case '+':
			node->result = node->left->result + node->right->result;
			break;
	}

	return node->result;
}

char* PrintNode(Node *node)
{
	if(!node || node->status == Resolved)
		return NULL;

	char *leftStr = PrintNode(node->left);
	char *rightStr = PrintNode(node->right);
	size_t strLength = 10 + (leftStr ? strlen(leftStr) : 32) + (rightStr ? strlen(rightStr) : 32);

	char *str = malloc(strLength);
	if(!str)
		return NULL;

	if(leftStr && rightStr)
		snprintf(str, strLength, "{%c %s %s }", node->type, leftStr, rightStr);
	else if(!leftStr && rightStr)
		snprintf(str, strLength, "{%c '%g' %s }", node->type, node->left->result, rightStr);
	else if(leftStr && !rightStr)
		snprintf(str, strLength, "{%c %s '%g' }", node->type, leftStr, node->right->result);
	else if(!leftStr && !rightStr)
		snprintf(str, strLength, "{%c '%g' '%g' }", node->type, node->left->result, node->right->result);

	if(leftStr)
		free(leftStr);
	if(rightStr)
		free(rightStr);

	return str;
}

void freeNode(Node *node)
{
	if(node->left)
		freeNode(node->left);
	if(node->right)
		freeNode(node->right);
	free(node);
}