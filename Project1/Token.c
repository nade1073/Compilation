#include "Token.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


Node *headNode = NULL;
int nextTokenCount = 0;
int maxNextToken=0;

int currentIndex = 0;
int HowManyTimesNextTokenHappend = 0;
Node* currentNode = NULL;
int backCounter = 0; 
#define TOKEN_ARRAY_SIZE 100
extern FILE *yyin, *yyout;
/*
* This function creates a token and stores it in the storage.
*/
void create_and_store_token(eTOKENS kind, int numOfLine, char* lexeme)
{

	// case 1: there is still no tokens in the storage.
	if (currentNode == NULL)
	{
		printf("Create NEW LIST\n");
		currentNode = (Node*)malloc(sizeof(Node));

		if (currentNode == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);
		if (currentNode->tokensArray == NULL)
		{
			fprintf(yyout, "\nUnable to allocate memory! \n");
			exit(0);
		}
		currentNode->prev = NULL;
		currentNode->next = NULL;
	}

	// case 2: at least one token exsits in the storage.
	else
	{
		// the array (the current node) is full, need to allocate a new node
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentIndex = 0;
			currentNode->next = (Node*)malloc(sizeof(Node));

			if (currentNode == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next->prev = currentNode;
			currentNode = currentNode->next;
			currentNode->tokensArray = (Token*)malloc(sizeof(Token)*TOKEN_ARRAY_SIZE);

			if (currentNode->tokensArray == NULL)
			{
				fprintf(yyout, "\nUnable to allocate memory! \n");
				exit(0);
			}
			currentNode->next = NULL;
		}

		// the array (the current node) is not full
		else
		{
			currentIndex++;
		}
	}
	currentNode->tokensArray[currentIndex].kind = kind;
	currentNode->tokensArray[currentIndex].lexeme = (char*)malloc(sizeof(char)*(strlen(lexeme) + 1));
	strcpy(currentNode->tokensArray[currentIndex].lexeme, lexeme);
	currentNode->tokensArray[currentIndex].lineNumber = numOfLine;
	

}

/*
* This function returns the token in the storage that is stored immediately before the current token (if exsits).
*/
Token *back_token()
{
	nextTokenCount--;
	backCounter++;
	if (currentIndex == 0)
	{
		currentNode = currentNode->prev;
		currentIndex = TOKEN_ARRAY_SIZE - 1;
	}
	else
	{
		currentIndex--;
	}
	return &(currentNode->tokensArray)[currentIndex];
}

/*
* If the next token already exists in the storage (this happens when back_token was called before this call to next_token):
*  this function returns the next stored token.
* Else: continues to read the input file in order to identify, create and store a new token (using yylex function);
*  returns the token that was created.
*/


Token* next_token()
{
	nextTokenCount++;
	if (backCounter > 0)
	{
		if (currentIndex == TOKEN_ARRAY_SIZE - 1)
		{
			currentNode = currentNode->next;
			currentIndex = 0;
		}
		else
		{
			currentIndex++;
		}
	backCounter--;
	}
	else
	{
		yylex();
	}
return &(currentNode->tokensArray[currentIndex]);
}









/*
Token *next_token()
{
	nextTokenCount++;

	if (nextTokenCount > maxNextToken)
	{
		maxNextToken++;
		yylex();
		if (currentNode == NULL)
		{
			Node *currentNode = (Node*)calloc(1, sizeof(Node));
		}
		else if (currentIndex == TOKEN_ARRAY_SIZE)
		{
			addToken();
			currentIndex = 0;
		}

		return &(currentNode->tokensArray)[currentIndex++];
	}
	else
	{
		if (currentIndex == TOKEN_ARRAY_SIZE)
		{
			currentNode = currentNode->next;
			currentIndex = 0;
			return &(currentNode->tokensArray)[currentIndex];

		}
		return &(currentNode->tokensArray)[currentIndex++];
	}
}

void addToken()
{
	if (currentNode == NULL)
	{
		currentNode = (Node*)calloc(1, sizeof(Node));
		headNode = currentNode;
	}
	else
	{
		Node *currentNodeTemp = currentNode;
		Node *newNode = (Node*)calloc(1, sizeof(Node));
		currentNode->next = newNode;
		currentNode = currentNode->next;
		currentNode->prev = currentNodeTemp;
		currentIndex = 0;
	}
	currentNode->next = NULL;
}
*/
void backTokenAllNextTokenHappend()
{
	int i;
	for (i = 0; i < HowManyTimesNextTokenHappend; i++)
	{
		back_token();
	}
}
char *stringFromeTOKENS(enum eTOKENS f)
{
	static const char *strings[] =
	{
		"AR_OP_ADDITION",
		"AR_OP_SUBTRACTION",
		"AR_OP_MULTIPLICATION",
		"AR_OP_DIVISION",
		"AR_OP_POWER",
		"UNARY_OP_INCREMENT",
		"UNARY_OP_AMP",
		"REL_OP_LESS",
		"REL_OP_EQUAL_OR_LESS",
		"REL_OP_EQUAL",
		"REL_OP_NOT_EQUAL",
		"REL_OP_GREATER",
		"REL_OP_EQUAL_OR_GREATER",
		"ASSIGNMENT",
		"POINTER",
		"ID",
		"INT_NUM",
		"REL_NUM",
		"KEYWORD_BLOCK",
		"KEYWORD_BEGIN",
		"KEYWORD_END",
		"KEYWORD_TYPE",
		"KEYWORD_IS",
		"KEYWORD_INTEGER",
		"KEYWORD_REAL",
		"KEYWORD_ARRAY",
		"KEYWORD_OF",
		"KEYWORD_WHEN",
		"KEYWORD_DO",
		"KEYWORD_DEFAULT",
		"KEYWORD_END_WHEN",
		"KEYWORD_FOR",
		"KEYWORD_END_FOR",
		"KEYWORD_FREE",
		"KEYWORD_SIZE_OF",
		"KEYWORD_MALLOC",
		"SEPERATION_SIGN_COLON",
		"SEPERATION_SIGN_SEMICOLON",
		"SEPERATION_SIGN_PARENT_OPEN",
		"SEPERATION_SIGN_PARENT_CLOSE",
		"SEPERATION_SIGN_BRACKET_OPEN",
		"SEPERATION_SIGN_BRACKET_CLOSE",
		"END_OF_FILE"
	};
	return strings[f];
}

void resetVarsAndList()
{
	printf("Reset Values And List\n");
	yyrestart();
	Node* saveNextNode;
	backCounter = 0;
	currentIndex = 0;
	while (currentNode->prev != NULL)
	{
		currentNode = currentNode->prev;
	}
	while (currentNode != NULL)
	{
		free(currentNode->tokensArray);
		saveNextNode = currentNode->next;
		free(currentNode);
		currentNode = saveNextNode;
	}
	printf("Sucsses Reset Value and List\n");
	
}
