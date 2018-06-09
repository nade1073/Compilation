#include <stdio.h>
#include "Parser.h"
#include "Token.h"
#include "Stack.h"
#include <string.h>

FILE *yyoutSyntax;
FILE *yyoutSemantic;
Token *currentToken;
StackOfHashTables* stack;
DataItem* tempDataItem;
Token *currentCommandLine;
int sizeOfTokensInCommandLine = 0;
void parser_PROGRAM()
{
	stack = createStack();
	fprintf(yyoutSyntax, "{PROGRAM -> BLOCK}\n");
	parser_BLOCK();
	match(END_OF_FILE);
}

void parser_BLOCK()
{
	eTOKENS followArray[] = { END_OF_FILE, SEPERATION_SIGN_SEMICOLON };
	int folowArraySize = 2;

	fprintf(yyoutSyntax, "{BLOCK -> block DEFINITIONS; begin COMMANDS; end}\n");
	if (match(KEYWORD_BLOCK) == 0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}


	HashTable* hashTableToPushToStack = createHashTable();
	push(stack, hashTableToPushToStack);

	parser_DEFINITIONS();
	if (match(SEPERATION_SIGN_SEMICOLON) == 0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}

	if (match(KEYWORD_BEGIN) == 0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}
	parser_COMMANDS();

	if (match(SEPERATION_SIGN_SEMICOLON) == 0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}

	if (match(KEYWORD_END) == 0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}
	pop(stack);
}

void parser_DEFINITIONS()
{
	fprintf(yyoutSyntax, "{DEFINITIONS -> DEFINITION DEFINITIONS_TAG }\n");
	parser_DEFINITION();
	parser_DEFINITIONS_TAG();
}

void parser_DEFINITION()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON };
	int folowArraySize = 1;
	eTOKENS expectedTokens[] = { ID , KEYWORD_TYPE };
	int expectedTokensArraySize = 2;
	currentToken = next_token();
	fprintf(yyoutSyntax, "{DEFINITION -> VAR_DEFINITION | TYPE_DEFINITION}\n");
	tempDataItem = calloc(1, sizeof(DataItem));
	tempDataItem->m_Data = (Data*)calloc(1, sizeof(Data));
	switch ((*currentToken).kind)
	{
	case ID:
	{
		tempDataItem->m_Data->name = currentToken->lexeme;
		tempDataItem->m_Data->role = Variable;
		tempDataItem->m_Key = StringToIntHash(tempDataItem->m_Data->name);
		fprintf(yyoutSyntax, "{VAR_DEFINITION -> id: VAR_DEFINITION_TAG}\n");
		if (match(SEPERATION_SIGN_COLON) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			break;
		}
		parser_VAR_DEFINITION_TAG();
		break;
	}
	case KEYWORD_TYPE:
	{
		tempDataItem->m_Data->role = UserDefinedType;
		fprintf(yyoutSyntax, "{TYPE_DEFINITION -> type type_name is TYPE_INDICATOR}\n");
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			break;
		}
		tempDataItem->m_Data->name = currentToken->lexeme;
		tempDataItem->m_Key = StringToIntHash(tempDataItem->m_Data->name);
		if (match(KEYWORD_IS) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			break;
		}
		parser_TYPE_INDICATOR();
		break;
	}
	default:
	{
		HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, expectedTokensArraySize);
	}
	}
}

void parser_DEFINITIONS_TAG()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON };
	int followArraySize = 1;
	eTOKENS expectedTokens[] = { SEPERATION_SIGN_SEMICOLON };
	int expectedTokensArraySize = 1;
	fprintf(yyoutSyntax, "{DEFINITIONS_TAG -> ; DEFINITION DEFINITION_TAG | EPSILON}\n");
	if (match(SEPERATION_SIGN_SEMICOLON) == 0)
	{
		HandleMatchError(followArray, followArraySize);
		return;
	}
	if (tempDataItem != NULL) 
	{
		insterToHashTableIfIdIsValid();
	}


	currentToken = next_token();
	switch ((*currentToken).kind)
	{
	case KEYWORD_BEGIN:
	{
		fprintf(yyoutSyntax, "{DEFINITIONS_TAG -> EPSILON}\n");
		currentToken = back_token();
		currentToken = back_token();
		break;
	}
	case ID:
	case KEYWORD_TYPE:
	{
		currentToken = back_token();
		parser_DEFINITIONS();
		break;
	}
	default:
	{
		eTOKENS expectedTokens[] = { KEYWORD_BEGIN,ID,KEYWORD_TYPE };
		int expectedTokensArraySize = 3;
		HandlingErrors(currentToken, followArray, followArraySize, expectedTokens, expectedTokensArraySize);
	}
	}
}

void parser_VAR_DEFINITION_TAG()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON };
	int folowArraySize = 1;
	eTOKENS expectedTokens[] = { KEYWORD_INTEGER , KEYWORD_REAL , ID };
	int sizeOfExpectedTokensArray = 3;
	currentToken = next_token();
	fprintf(yyoutSyntax, "{VAR_DEFINITION_TAG -> BASIC TYPE | type_name}\n");
	switch ((*currentToken).kind)
	{
	case KEYWORD_INTEGER:
	{
		tempDataItem->m_Data->typeOfVariable = stringFromeTOKENS(KEYWORD_INTEGER);
		fprintf(yyoutSyntax, "{VAR_DEFINITION_TAG -> BASIC_TYPE -> INTEGER}\n");
		break;
	}
	case KEYWORD_REAL:
	{
		tempDataItem->m_Data->typeOfVariable = stringFromeTOKENS(KEYWORD_REAL);
		fprintf(yyoutSyntax, "{VAR_DEFINITION_TAG -> BASIC_TYPE -> REAL}\n");
		break;
	}
	case ID:
	{
		tempDataItem->m_Data->typeOfVariable = ((*currentToken).lexeme);
		fprintf(yyoutSyntax, "{VAR_DEFINITION_TAG -> type_name}\n");
		break;
	}
	default:
	{
		HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokensArray);
	}
	}
}

void parser_TYPE_INDICATOR()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON };
	int folowArraySize = 1;
	eTOKENS expectedTokens[] = { KEYWORD_INTEGER , KEYWORD_REAL , KEYWORD_ARRAY , POINTER };
	int sizeOfExpectedTokens = 4;
	currentToken = next_token();
	fprintf(yyoutSyntax, "{TYPE_INDICATOR -> BASIC_TYPE | ARRAY_TYPE | POINTER_TYPE}\n");

	switch ((*currentToken).kind)
	{
	case KEYWORD_INTEGER:
	{
		tempDataItem->m_Data->categoryOfType = Basic;
		tempDataItem->m_Data->basicSubTypeName = stringFromeTOKENS(KEYWORD_INTEGER);
		fprintf(yyoutSyntax, "{TYPE_INDICATOR -> BASIC_TYPE -> INTEGER}\n");
		break;
	}

	case KEYWORD_REAL:
	{
		tempDataItem->m_Data->categoryOfType = Basic;
		tempDataItem->m_Data->basicSubTypeName = stringFromeTOKENS(KEYWORD_REAL);
		fprintf(yyoutSyntax, "{TYPE_INDICATOR -> BASIC_TYPE -> REAL}\n");
		break;
	}
	case KEYWORD_ARRAY:
	{
		tempDataItem->m_Data->categoryOfType = Array;
		fprintf(yyoutSyntax, "{TYPE_INDICATOR -> ARRAY_TYPE}\n");
		if (match(SEPERATION_SIGN_BRACKET_OPEN) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			break;
		}
		if (match(INT_NUM) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			break;
		}
		tempDataItem->m_Data->sizeOfArray = currentToken->lexeme;
		if (match(SEPERATION_SIGN_BRACKET_CLOSE) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			break;
		}
		if (match(KEYWORD_OF) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			break;
		}

		currentToken = next_token();
		switch ((*currentToken).kind)
		{
		case KEYWORD_INTEGER:
		{
			tempDataItem->m_Data->basicSubTypeName = stringFromeTOKENS(KEYWORD_INTEGER);
			fprintf(yyoutSyntax, "{TYPE_INDICATOR -> ARRAY_TYPE -> array [INT_NUM] of INTEGER }\n");
			break;
		}
		case KEYWORD_REAL:
		{
			tempDataItem->m_Data->basicSubTypeName = stringFromeTOKENS(KEYWORD_REAL);
			fprintf(yyoutSyntax, "{TYPE_INDICATOR -> ARRAY_TYPE -> array [INT_NUM] of REAL }\n");
			break;
		}
		default:
		{
			eTOKENS expectedTokensInside[] = { KEYWORD_INTEGER , KEYWORD_REAL };
			int sizeOfExpectedTokensInside = 2;
			HandlingErrors(currentToken, followArray, folowArraySize, expectedTokensInside, sizeOfExpectedTokensInside);
		}
		}
		break;
	}
	case POINTER:
	{
		tempDataItem->m_Data->categoryOfType = Pointer;

		fprintf(yyoutSyntax, "{TYPE_INDICATOR -> POINTER_TYPE -> ^POINTER_TYPE_TAG}\n");
		parser_POINTER_TYPE_TAG();
		break;
	}
	default:
	{
		HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokens);
	}
	}
}

void parser_POINTER_TYPE_TAG()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS expectedTokens[] = { KEYWORD_INTEGER , KEYWORD_REAL , ID };
	int folowArraySize = 1;
	int sizeOfExpectedTokens = 3;
	currentToken = next_token();

	switch ((*currentToken).kind)
	{
	case KEYWORD_INTEGER:
	{
		tempDataItem->m_Data->basicSubTypeName = stringFromeTOKENS(KEYWORD_INTEGER);
		fprintf(yyoutSyntax, "{POINTER_TYPE_TAG -> ^INTEGER}\n");
		break;
	}
	case KEYWORD_REAL:
	{
		tempDataItem->m_Data->basicSubTypeName = stringFromeTOKENS(KEYWORD_REAL);
		fprintf(yyoutSyntax, "{POINTER_TYPE_TAG -> ^REAL}\n");
		break;
	}
	case ID:
	{
		tempDataItem->m_Data->basicSubTypeName = currentToken->lexeme;
		fprintf(yyoutSyntax, "{POINTER_TYPE_TAG -> ^type_name}\n");
		break;
	}
	default:
	{
		HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokens);
	}
	}
}

void parser_COMMANDS()
{
	fprintf(yyoutSyntax, "{COMMANDS -> COMMAND COMMANDS_TAG}\n");
	parser_COMMAND();
	parser_COMMANDS_TAG();
}

void parser_COMMAND()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON };
	int folowArraySize = 1;
	fprintf(yyoutSyntax, "{COMMAND -> RECEIVER = EXPRESSION | when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when | for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for | id = malloc(size_of(type_name)) | free(id) | BLOCK}\n");
	currentToken = next_token();

	switch ((*currentToken).kind)
	{
	case ID:
	{
		fprintf(yyoutSyntax, "{COMMAND -> id = EXPRESSION | id = malloc(size_of(type_name))}\n");
		assignCurrentTokenToCommands();
		currentToken = next_token();
		switch ((*currentToken).kind)
		{
		case ASSIGNMENT:
		{
			currentToken = next_token();
			switch ((*currentToken).kind)
			{
			case KEYWORD_MALLOC:
			{
				assignCurrentTokenToCommands();
				fprintf(yyoutSyntax, "{COMMAND -> id=malloc(sizeof(type_name))}\n");
				if (match(SEPERATION_SIGN_PARENT_OPEN) == 0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(KEYWORD_SIZE_OF) == 0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				assignCurrentTokenToCommands();
				if (match(SEPERATION_SIGN_PARENT_OPEN) == 0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(ID) == 0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				assignCurrentTokenToCommands();
				if (match(SEPERATION_SIGN_PARENT_CLOSE) == 0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(SEPERATION_SIGN_PARENT_CLOSE) == 0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				break;
			}
			case REL_NUM:
			case ID:
			case INT_NUM:
			case UNARY_OP_AMP:
			case KEYWORD_SIZE_OF:
			{
			
				fprintf(yyoutSyntax, "{COMMAND -> id = EXPRESSION}\n");
				currentToken = back_token();
				parser_EXPRESSION();
				break;
			}
			default:
			{
				eTOKENS expectedTokens[] = { KEYWORD_MALLOC,REL_NUM,ID,INT_NUM,UNARY_OP_AMP,KEYWORD_SIZE_OF };
				int sizeOfExpectedTokens = 6;
				HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokens);
				break;
			}
			}
			break;
		}
		case POINTER:
		case SEPERATION_SIGN_BRACKET_OPEN:
		{

			currentToken = back_token();
			parser_RECIVER_TAG();
			if (match(ASSIGNMENT) == 0)
			{
				HandleMatchError(followArray, folowArraySize);
				return;
			}
			parser_EXPRESSION();
			break;
		}
		default:
		{
			eTOKENS expectedTokens[] = { ASSIGNMENT,POINTER,SEPERATION_SIGN_BRACKET_OPEN };
			int sizeOfExpectedTokens = 3;
			HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokens);
			break;
		}
		}
		break;
	}
	case KEYWORD_WHEN:
	{
		eTOKENS followARRRelOP[9] = { SEPERATION_SIGN_SEMICOLON, REL_OP_LESS ,REL_OP_EQUAL_OR_LESS ,REL_OP_EQUAL ,REL_OP_NOT_EQUAL ,REL_OP_GREATER ,REL_OP_EQUAL_OR_GREATER ,SEPERATION_SIGN_PARENT_CLOSE ,SEPERATION_SIGN_BRACKET_CLOSE };
		eTOKENS expectedTokensRelOp[6] = { REL_OP_LESS, REL_OP_EQUAL_OR_LESS , REL_OP_EQUAL , REL_OP_NOT_EQUAL , REL_OP_GREATER, REL_OP_EQUAL_OR_GREATER };
		fprintf(yyoutSyntax, "{COMMAND -> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when}\n");
		if (match(SEPERATION_SIGN_PARENT_OPEN) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		parser_EXPRESSION();
		currentToken = next_token();
		switch ((*currentToken).kind)
		{
		case REL_OP_LESS:
		case REL_OP_EQUAL_OR_LESS:
		case REL_OP_EQUAL:
		case REL_OP_NOT_EQUAL:
		case REL_OP_GREATER:
		case REL_OP_EQUAL_OR_GREATER:
		{
			parser_EXPRESSION();
			break;
		}
		default:
		{
			eTOKENS expectedTokens[] = { REL_OP_LESS , REL_OP_EQUAL_OR_LESS , REL_OP_EQUAL , REL_OP_NOT_EQUAL , REL_OP_GREATER , REL_OP_EQUAL_OR_GREATER };
			int sizeOfExpectedTokens = 6;
			HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokens);
		}
		}
		if (match(SEPERATION_SIGN_PARENT_CLOSE) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		assignInWhenSemiColon();
		commandsHandlerType();
		sizeOfTokensInCommandLine = 0;
		if (match(KEYWORD_DO) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		parser_COMMANDS();
		if (match(SEPERATION_SIGN_SEMICOLON) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		if (match(KEYWORD_DEFAULT) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		parser_COMMANDS();
		if (match(SEPERATION_SIGN_SEMICOLON) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		if (match(KEYWORD_END_WHEN) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		break;
	}
	case KEYWORD_FOR:
	{
		fprintf(yyoutSyntax, "{COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for}\n");
		if (match(SEPERATION_SIGN_PARENT_OPEN) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		if (match(ASSIGNMENT) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		parser_EXPRESSION();
		if (match(SEPERATION_SIGN_SEMICOLON) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		commandsHandlerType();
		sizeOfTokensInCommandLine = 0;
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		currentToken = next_token();
		if (((*currentToken).kind == REL_OP_LESS) || (*currentToken).kind == REL_OP_EQUAL_OR_LESS || (*currentToken).kind == REL_OP_EQUAL || (*currentToken).kind == REL_OP_NOT_EQUAL || (*currentToken).kind == REL_OP_GREATER || (*currentToken).kind == REL_OP_EQUAL_OR_GREATER)
		{
			parser_EXPRESSION();
		}
		else
		{
			eTOKENS expectedArray[] = { REL_OP_EQUAL, REL_OP_LESS, REL_OP_NOT_EQUAL,REL_OP_EQUAL_OR_GREATER,REL_OP_GREATER,REL_OP_EQUAL_OR_LESS };
			int expectedArraySize = 6;
			char expectedTokensArray[256] = "";
			for (int i = 0; i < expectedArraySize; i++)
			{
				strcat(expectedTokensArray, "'");
				strcat(expectedTokensArray, stringFromeTOKENS(expectedArray[i]));
				strcat(expectedTokensArray, "' ");
			}
			printErrorToTheFile(expectedTokensArray, (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		if (match(SEPERATION_SIGN_SEMICOLON) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		commandsHandlerType();
		sizeOfTokensInCommandLine = 0;
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		if (match(UNARY_OP_INCREMENT) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		commandsHandlerTypeOfUnrayOpIncrement();
		sizeOfTokensInCommandLine = 0;
		if (match(SEPERATION_SIGN_PARENT_CLOSE) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		parser_COMMANDS();
		if (match(SEPERATION_SIGN_SEMICOLON) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		if (match(KEYWORD_END_FOR) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		break;
	}
	case KEYWORD_FREE:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{COMMAND -> free(id)}\n");
		if (match(SEPERATION_SIGN_PARENT_OPEN) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		if (match(SEPERATION_SIGN_PARENT_CLOSE)==0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		break;
	}
	case KEYWORD_BLOCK:
	{
		fprintf(yyoutSyntax, "{COMMAND -> BLOCK}\n");
		back_token();
		parser_BLOCK();
		break;
	}
	default:
	{
		eTOKENS expectedTokens[] = { ID,KEYWORD_WHEN, KEYWORD_FOR,KEYWORD_FREE,KEYWORD_BLOCK };
		int sizeOfExpectedTokens = 5;
		HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokens);
	}
	}
}

void parser_COMMANDS_TAG()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON };
	int folowArraySize = 1;
	if (match(SEPERATION_SIGN_SEMICOLON) == 0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}
	assignCurrentTokenToCommands();
	if (sizeOfTokensInCommandLine == 1)
	{
		sizeOfTokensInCommandLine = 0;
	}
	else
	{
		commandsHandlerType();
		sizeOfTokensInCommandLine = 0;
	}

	
	fprintf(yyoutSyntax, "{COMMANDS_TAG -> ; COMMAND COMMANDS_TAG | EPSILON}\n");
	currentToken = next_token();
	switch ((*currentToken).kind)
	{
	case KEYWORD_END:
	case KEYWORD_END_FOR:
	case KEYWORD_END_WHEN:
	case KEYWORD_DEFAULT:
	{
		fprintf(yyoutSyntax, "{COMMAND_TAG -> Epsilon}\n");
		back_token();
		back_token();
		break;
	}
	case ID:
	case KEYWORD_WHEN:
	case KEYWORD_FOR:
	case KEYWORD_FREE:
	case KEYWORD_BLOCK:
	{
		back_token();
		parser_COMMAND();
		parser_COMMANDS_TAG();
		break;
	}
	case ASSIGNMENT:
	{
		parser_EXPRESSION();
		break;
	}
	default:
	{
		eTOKENS *expectedTokens[] = { ID,KEYWORD_WHEN, KEYWORD_FOR,KEYWORD_FREE,KEYWORD_BLOCK,KEYWORD_END,KEYWORD_END_FOR,KEYWORD_END_WHEN,KEYWORD_DEFAULT };
		int sizeOfExpectedTokens = 9;
		HandlingErrors(currentToken, followArray, folowArraySize, expectedTokens, sizeOfExpectedTokens);
		break;
	}
	}
}

void parser_EXPRESSION()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON, AR_OP_ADDITION ,AR_OP_DIVISION ,AR_OP_MULTIPLICATION ,AR_OP_MULTIPLICATION ,AR_OP_POWER ,AR_OP_SUBTRACTION ,SEPERATION_SIGN_PARENT_CLOSE ,SEPERATION_SIGN_BRACKET_CLOSE };
	int followArraySize = 9;
	fprintf(yyoutSyntax, "{EXPRESSION -> int_num | real_num | id EXPRESSION_TAG | &id | size_of(type_name)}\n");
	currentToken = next_token();
	switch ((*currentToken).kind)
	{
	case INT_NUM:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{EXPRESSION -> INT_NUM}\n");
		break;
	}
	case REL_NUM:
	{

		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{EXPRESSION -> REL_NUM}\n");
		break;
	}
	case ID:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{EXPRESSION -> id EXPRESSION_TAG}\n");
		parser_EXPRESSION_TAG();
		break;
	}
	case UNARY_OP_AMP:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{EXPRESSION -> &id}\n");
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, followArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		break;
	}
	case KEYWORD_SIZE_OF:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{EXPRESSION -> size_of(type_name)}\n");
		if (match(SEPERATION_SIGN_PARENT_OPEN) == 0)
		{
			HandleMatchError(followArray, followArraySize);
			return;
		}
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, followArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		if (match(SEPERATION_SIGN_PARENT_CLOSE) == 0)
		{
			HandleMatchError(followArray, followArraySize);
			return;
		}
		break;
	}
	default:
	{
		eTOKENS expectedTokens[] = { INT_NUM, REL_NUM , ID , UNARY_OP_AMP , KEYWORD_SIZE_OF };
		int sizeOfExpectedTokens = 5;
		HandlingErrors(currentToken, followArray, followArraySize, expectedTokens, sizeOfExpectedTokens);
	}
	}
}

void parser_EXPRESSION_TAG()
{
	eTOKENS followArray[] = { SEPERATION_SIGN_SEMICOLON,UNARY_OP_INCREMENT,AR_OP_SUBTRACTION,AR_OP_ADDITION,AR_OP_DIVISION,AR_OP_MULTIPLICATION,AR_OP_POWER,SEPERATION_SIGN_BRACKET_CLOSE,SEPERATION_SIGN_PARENT_CLOSE };
	int followArraySize = 9;
	currentToken = next_token();
	switch ((*currentToken).kind)
	{
	case SEPERATION_SIGN_BRACKET_OPEN:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> [EXPRESSION]}\n");
		parser_EXPRESSION();
		if (match(SEPERATION_SIGN_BRACKET_CLOSE) == 0)
		{
			HandleMatchError(followArray, followArraySize);
			return;
		}
		assignCurrentTokenToCommands();
		break;
	}
	case POINTER:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> ^}\n");
		break;
	}
	case AR_OP_ADDITION:
	case AR_OP_SUBTRACTION:
	case AR_OP_MULTIPLICATION:
	case AR_OP_DIVISION:
	case UNARY_OP_INCREMENT:
	case AR_OP_POWER:
	{
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> ar_op EXPRESSION}\n");
		parser_EXPRESSION();
		break;
	}
	case REL_OP_EQUAL:
	case REL_OP_NOT_EQUAL:
	case REL_OP_EQUAL_OR_GREATER:
	case REL_OP_EQUAL_OR_LESS:
	case REL_OP_GREATER:
	case REL_OP_LESS:
	case SEPERATION_SIGN_BRACKET_CLOSE:
	case SEPERATION_SIGN_SEMICOLON:
	{
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> EPSILON}\n");
		back_token();
		break;
	}
	default:
	{
		eTOKENS expectedTokens[] = { SEPERATION_SIGN_SEMICOLON, SEPERATION_SIGN_BRACKET_OPEN,POINTER,AR_OP_ADDITION,AR_OP_SUBTRACTION,AR_OP_MULTIPLICATION,AR_OP_DIVISION,UNARY_OP_INCREMENT,AR_OP_POWER,REL_OP_EQUAL,REL_OP_NOT_EQUAL,REL_OP_EQUAL_OR_GREATER,REL_OP_EQUAL_OR_LESS,REL_OP_GREATER,REL_OP_LESS,SEPERATION_SIGN_BRACKET_CLOSE };
		int expectedTokenSize = 16;
		HandlingErrors(currentToken, followArray, followArraySize, expectedTokens, expectedTokenSize);
	}
	}
}

void parser_RECIVER_TAG()
{
	eTOKENS followArray[] = { ASSIGNMENT };
	int sizeOfFollowArray = 1;
	fprintf(yyoutSyntax, "{RECEIVER_TAG -> EPSILON | [EXPRESSION] | ^}\n");
	currentToken = next_token();
	switch ((*currentToken).kind)
	{
	case SEPERATION_SIGN_BRACKET_OPEN:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{RECIVER_TAG -> [EXPRESSION]}\n");
		parser_EXPRESSION();
		if (match(SEPERATION_SIGN_BRACKET_CLOSE) == 0)
		{
			HandleMatchError(followArray, sizeOfFollowArray);
			return;
		}
		assignCurrentTokenToCommands();
		break;
	}
	case POINTER:
	{
		assignCurrentTokenToCommands();
		fprintf(yyoutSyntax, "{RECIVER_TAG -> ^}\n");
		break;
	}
	case ASSIGNMENT:
	{
		fprintf(yyoutSyntax, "{RECEIVER_TAG -> EPSILON}\n");
		currentToken = back_token();
		break;
	}
	default:
	{
		eTOKENS expectedTokens[] = { SEPERATION_SIGN_BRACKET_OPEN , POINTER,ASSIGNMENT };
		int sizeOfExpectedTokens = 3;
		HandlingErrors(currentToken, followArray, 1, expectedTokens, sizeOfExpectedTokens);
	}
	}
}

int match(eTOKENS i_ActualKind)
{
	int boolToReturn = 0;
	currentToken = next_token();
	if ((*currentToken).kind != i_ActualKind)
	{
		printErrorToTheFile(stringFromeTOKENS(i_ActualKind), (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
		if ((*currentToken).kind == END_OF_FILE)
		{
			boolToReturn = 0;
		}
		boolToReturn = 0;
		return boolToReturn;
	}
	else if (i_ActualKind == END_OF_FILE)
	{
		fprintf(yyoutSyntax, "EOF\n");
		boolToReturn = 1;
	}

	boolToReturn = 1;
	return boolToReturn;
}

void printErrorToTheFile(char* i_ExcepectedToken, int i_LineNumber, char* i_ActualToken, char* i_Lexeme)
{
	fprintf(yyoutSyntax, "Expected token %s at line: %d,\nActual token '%s', lexeme: '%s'.\n",
		i_ExcepectedToken, i_LineNumber, i_ActualToken, i_Lexeme);
}

int CheckIfTokenInFollowArr(Token* currentToken, eTOKENS *followArr, int followArrsize)
{
	int TokenFound = 0;
	int i;

	for (i = 0; i < followArrsize; i++)
	{
		if (followArr[i] == (*currentToken).kind)
		{
			TokenFound = 1;
		}
	}
	return TokenFound;
}

void HandleMatchError(eTOKENS *i_FollowArray, int i_SizeOfFollowArray)
{
	currentCommandLine = NULL;
	sizeOfTokensInCommandLine = 0;
	tempDataItem = NULL;
	int tokenFound = 0;
	tokenFound = CheckIfTokenInFollowArr(currentToken, i_FollowArray, i_SizeOfFollowArray);
	while (tokenFound == 0 && (*currentToken).kind != END_OF_FILE)
	{
		currentToken = next_token();
		tokenFound = CheckIfTokenInFollowArr(currentToken, i_FollowArray, i_SizeOfFollowArray);
	}
	currentToken = back_token();
}

void HandlingErrors(Token* currentToken, eTOKENS *followArr, int followArrSize, eTOKENS *expectedTokens, int expectedTokensSize)
{
	sizeOfTokensInCommandLine = 0;
	currentCommandLine = NULL;
	tempDataItem = NULL;
	int i;
	char expectedTokensString[250] = "";
	int tokenFound;
	tokenFound = CheckIfTokenInFollowArr(currentToken, followArr, followArrSize);
	for (i = 0; i < expectedTokensSize; i++)
	{
		strcat(expectedTokensString, "'");
		strcat(expectedTokensString, stringFromeTOKENS(expectedTokens[i]));
		strcat(expectedTokensString, "' ");
	}
	if (tokenFound == 1)
	{
		printErrorToTheFile(expectedTokensString, (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
	}
	while (tokenFound == 0 && (*currentToken).kind != END_OF_FILE)
	{

		if (expectedTokensSize == 1)
		{
			printErrorToTheFile(expectedTokensString, (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
		}
		else
		{
			printErrorToTheFile(expectedTokensString, (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
		}
		currentToken = next_token();
		tokenFound = CheckIfTokenInFollowArr(currentToken, followArr, followArrSize);
	}
	currentToken = back_token();
}

void insterToHashTableIfIdIsValid()
{
	char* currentTypeOfVariable = NULL;
	if (tempDataItem->m_Data->role == Variable)
	{
		handleWithVariableType();

	}
	else
	{
		handleWithUserDefinedType();
	}
}

void handleWithVariableType()
{
	char* currentTypeOfVariable = tempDataItem->m_Data->typeOfVariable;
	char* integerType = stringFromeTOKENS(KEYWORD_INTEGER);
	char* realType = stringFromeTOKENS(KEYWORD_REAL);
	DataItem* dataItemThatReturnFromTheHashtable;
	HashTable* tempHashTable = top(stack);
	DataItem* currentDataItem = searchInsideHashTableAndReturnItem(tempHashTable, tempDataItem->m_Key);
	if (currentDataItem != NULL)
	{
		duplicateIdInTheSameScope(currentDataItem->m_Data->name, currentToken->lineNumber);
		return;
	}
	if (((strcmp((currentTypeOfVariable), integerType) != 0) && (strcmp((currentTypeOfVariable), realType) != 0)))
	{
		dataItemThatReturnFromTheHashtable = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(tempDataItem->m_Data->typeOfVariable));
		if (dataItemThatReturnFromTheHashtable == NULL )
		{
			printTypeNotDefined(currentToken->lineNumber, tempDataItem->m_Data->typeOfVariable);
		}
		else if (dataItemThatReturnFromTheHashtable->m_Data->role == Variable)
		{
			fprintf(yyoutSemantic, "(Line %d) Unexpected Variable, Lexeme : %s\n", currentToken->lineNumber, dataItemThatReturnFromTheHashtable->m_Data->name);
		}
		else
		{
			tempDataItem->m_Data->subType = dataItemThatReturnFromTheHashtable;
			insert(top(stack), tempDataItem);
		}
	}
	else
	{
		insert(top(stack), tempDataItem);
	}

}

void handleWithUserDefinedType()
{
	char* currentTypeOfVariable = tempDataItem->m_Data->basicSubTypeName;
	char* integerType = stringFromeTOKENS(KEYWORD_INTEGER);
	char* realType = stringFromeTOKENS(KEYWORD_REAL);
	DataItem* dataItemThatReturnFromTheHashtable;
	HashTable* tempHashTable = top(stack);
	DataItem* currentDataItem = searchInsideHashTableAndReturnItem(tempHashTable, tempDataItem->m_Key);
	if (currentDataItem != NULL)
	{	
		duplicateIdInTheSameScope(currentDataItem->m_Data->name, currentToken->lineNumber);
		return;
	}
	if (((strcmp((currentTypeOfVariable), integerType) != 0) && (strcmp((currentTypeOfVariable), realType) != 0)))
	{
		dataItemThatReturnFromTheHashtable = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(tempDataItem->m_Data->basicSubTypeName));
	
		if (dataItemThatReturnFromTheHashtable == NULL)
		{
			printTypeNotDefined(currentToken->lineNumber, tempDataItem->m_Data->basicSubTypeName);
		}
		else if (dataItemThatReturnFromTheHashtable->m_Data->role == Variable)
		{
			fprintf(yyoutSemantic, "(Line %d) Unexpected Variable, Lexeme : %s\n", currentToken->lineNumber, dataItemThatReturnFromTheHashtable->m_Data->name);
		}
		else
		{
			tempDataItem->m_Data->subType = dataItemThatReturnFromTheHashtable;
			DataItem* temp = tempDataItem->m_Data->subType;
			insert(top(stack), tempDataItem);
		}
	}
	else
	{
		insert(top(stack), tempDataItem);
	}
}

DataItem* searchInsideHashTableIfTheSubTypeExist(int i_Key)
{
	int isIdExsitsInAllHashTables = 0;
	DataItem* currentDataItem = NULL;
	StackOfHashTables* tempStack = createStack();
	while (!isEmpty(stack) && isIdExsitsInAllHashTables == 0)
	{
		HashTable* tempHashTable = pop(stack);
		currentDataItem = searchInsideHashTableAndReturnItem(tempHashTable, i_Key);
		if (currentDataItem != NULL)
		{
			isIdExsitsInAllHashTables = 1;
		}
		push(tempStack, tempHashTable);
	}
	pushAllItemFromFirstStackToSecondStack(tempStack, stack);
	return currentDataItem;
}

DataItem* checkIfIdExists(int i_Key)
{
	HashTable* tempHashTable = top(stack);
	DataItem* isFound;
	isFound = searchInsideHashTableIfTheSubTypeExist(i_Key);
	if (isFound == NULL)
	{
		printTypeNotDefined(currentToken->lineNumber, currentToken->lexeme);
	}
	return isFound;
}

void assignCurrentTokenToCommands()
{
	currentCommandLine = realloc(currentCommandLine, (++sizeOfTokensInCommandLine)*sizeof(Token));
	currentCommandLine[sizeOfTokensInCommandLine - 1].lexeme = currentToken->lexeme;
	currentCommandLine[sizeOfTokensInCommandLine - 1].kind = currentToken->kind;
	currentCommandLine[sizeOfTokensInCommandLine - 1].lineNumber = currentToken->lineNumber;
}
void assignInWhenSemiColon()
{
	currentCommandLine = realloc(currentCommandLine, (++sizeOfTokensInCommandLine) * sizeof(Token));
	currentCommandLine[sizeOfTokensInCommandLine - 1].lexeme = ';';
	currentCommandLine[sizeOfTokensInCommandLine - 1].kind = SEPERATION_SIGN_SEMICOLON;
	currentCommandLine[sizeOfTokensInCommandLine - 1].lineNumber = currentToken->lineNumber;
}
void commandsHandlerType()
{
	DataItem* currentItem=NULL;
	int indexArray = 0;
	char* typeVariable = NULL;
	char* currentKindTypeOfAllTheCommands = NULL;
	char* integerType = stringFromeTOKENS(KEYWORD_INTEGER);
	char* realType = stringFromeTOKENS(KEYWORD_REAL);
	eCategoryOfType currentCategoryType=Basic;
	int isLeftCommandSideWithRoof = 0;
	DataItem* subTypeItem = NULL;
	eTOKENS currentKindOfToken;
	if (currentCommandLine[0].kind == KEYWORD_FREE)
	{
		currentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[1].lexeme));
		if (currentItem == NULL)
		{
			printVarNotDefined(currentCommandLine[1].lineNumber, currentCommandLine[1].lexeme);
			return;
		}
		else if (currentItem->m_Data->role == UserDefinedType)
		{
			printExpectedTypeOrVar(currentCommandLine[1], UserDefinedType);
			return;
		}
		if (currentItem->m_Data->categoryOfType != POINTER)
		{
			printErrorAssignVaribleToPointer(currentCommandLine[1]);
			return;
		}
		currentCommandLine=NULL;
	}
	if (currentCommandLine != NULL)
	{
		currentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[indexArray].lexeme));
		if (currentItem == NULL)
		{
			printVarNotDefined(currentCommandLine[indexArray].lineNumber, currentCommandLine[indexArray].lexeme);
			return;
		}
		else if (currentItem->m_Data->role == UserDefinedType)
		{
			printExpectedTypeOrVar(currentCommandLine[indexArray], UserDefinedType);
			return;
		}
		typeVariable = currentItem->m_Data->typeOfVariable;
		currentKindTypeOfAllTheCommands = typeVariable;
		indexArray++;
		if (currentItem->m_Data->subType != NULL)
		{
			subTypeItem = currentItem->m_Data->subType;
			currentKindTypeOfAllTheCommands = subTypeItem->m_Data->basicSubTypeName;
			switch (subTypeItem->m_Data->categoryOfType)
			{
				case Array:
				{
					if (currentCommandLine[indexArray].kind != SEPERATION_SIGN_BRACKET_OPEN)
					{
						printErrorOfArrayAfterVariable(currentCommandLine[indexArray]);
						return;
					}
					indexArray++;
					indexArray = handleWithIndexesInsideArray(indexArray);
					if (indexArray == -1)
					{
						return;
					}
					break;
				}
				case Pointer:
				{
					currentCategoryType = Pointer;
					if (currentCommandLine[indexArray].kind == POINTER)
					{
						indexArray++;
						isLeftCommandSideWithRoof = 1;
					}
					break;
				}
				case Basic:
				{
					if (currentCommandLine[indexArray].kind == POINTER)
					{
						printErrorVaiableWrongType(currentCommandLine[indexArray], Pointer);
						return;
					}
					else if (currentCommandLine[indexArray].kind == SEPERATION_SIGN_BRACKET_OPEN)
					{
						printErrorVaiableWrongType(currentCommandLine[indexArray], Array);
						return;
					}
					break;
				}
			}

		}
		else
		{
			if (currentCommandLine[indexArray].kind == POINTER)
			{
				printErrorVaiableWrongType(currentCommandLine[indexArray], Pointer);
				return;
			}
			else if (currentCommandLine[indexArray].kind == SEPERATION_SIGN_BRACKET_OPEN)
			{
				printErrorVaiableWrongType(currentCommandLine[indexArray], Array);
				return;
			}
		}
		switch (currentCommandLine[indexArray].kind)
		{
			case KEYWORD_SIZE_OF:
			{
				indexArray = handleWithSizeOfKeyword(indexArray);
				if (indexArray == -1)
				{
					return;
				}
				if (isLeftCommandSideWithRoof == 0 && currentCategoryType==Pointer)
				{
					printErrorOfPointerAfterVariable(currentCommandLine[indexArray - 2]);	
					return;
				}
				if (strcmp(currentKindTypeOfAllTheCommands, integerType) != 0)
				{
					printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, integerType);
					return;
				}				
				break;
			}
			case KEYWORD_MALLOC:
			{
				if (currentCategoryType == Pointer)
				{
					if (isLeftCommandSideWithRoof == 1)
					{
						fprintf(yyoutSemantic, "(Line %d)  Trying to allocate memory to %s that is not pointer .\n", currentCommandLine[indexArray].lineNumber, currentCommandLine[indexArray].lexeme);
						return;
					}
					indexArray++;
					indexArray = handleWithSizeOfKeywordInsideTheKeywordMalloc(indexArray, currentKindTypeOfAllTheCommands);
					if (indexArray == -1)
					{
						return;
					}				
				}
				else
				{
					fprintf(yyoutSemantic, "(Line %d)  Trying to allocate memory to %s that is not pointer .\n", currentCommandLine[indexArray].lineNumber, currentCommandLine[indexArray].lexeme);
					return;
				}
				break;
			}			
			default :
			{
				while (currentCommandLine[indexArray].kind != SEPERATION_SIGN_SEMICOLON)
				{
					currentKindOfToken = currentCommandLine[indexArray].kind;
					if (currentCategoryType == Pointer && isLeftCommandSideWithRoof == 0)
					{
						if (currentKindOfToken != UNARY_OP_AMP && currentKindOfToken != ID)
						{
							fprintf(yyoutSemantic, "(Line %d)  cannot assign %s to type of pointer .\n", currentCommandLine[indexArray].lineNumber, currentCommandLine[indexArray].lexeme);
							return;
						}
					}
					switch (currentKindOfToken)
					{
						case REL_NUM:
						{
							if (strcmp(currentKindTypeOfAllTheCommands, realType) != 0)
							{
								printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, realType);
								return;
							}
							break;
						}
						case INT_NUM:
						{
							if (strcmp(currentKindTypeOfAllTheCommands, integerType) != 0)
							{
								printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, integerType);
								return;
							}
							break;
						}
						case ID:
						{
							currentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[indexArray].lexeme));
							if (currentItem == NULL )
							{
								printVarNotDefined(currentCommandLine[indexArray].lineNumber, currentCommandLine[indexArray].lexeme);
								return;
							}
							else if (currentItem->m_Data->role == UserDefinedType)
							{
								printExpectedTypeOrVar(currentCommandLine[1], UserDefinedType);
								return;
							}
							if (currentItem->m_Data->subType != NULL)
							{
								subTypeItem = currentItem->m_Data->subType;
								if (currentCategoryType == Pointer)
								{
									if (isLeftCommandSideWithRoof == 0)
									{
										if ((subTypeItem->m_Data->categoryOfType != Pointer))
										{
											printErrorAssignVaribleToPointer(currentCommandLine[indexArray]);
											return;
										}
										if (strcmp(currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName) != 0)
										{
											printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName);
											return;
										}
										/*
										if (strcmp(currentItem->m_Data->typeOfVariable, typeVariable) != 0)
										{
											printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentItem->m_Data->typeOfVariable, typeVariable);
											return;
										}
										*/
									}
									else
									{
										switch (subTypeItem->m_Data->categoryOfType)
										{
											case Basic:
											{
												if (strcmp(currentKindTypeOfAllTheCommands, currentItem->m_Data->typeOfVariable) != 0)
												{
													printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, currentItem->m_Data->typeOfVariable);
													return;
												}
												break;
											}

											case Pointer:
											{
												indexArray++;
												if (currentCommandLine[indexArray].kind != POINTER)
												{
													printErrorOfPointerAfterVariable(currentCommandLine[indexArray - 1]);
													return;
												}
												indexArray++;
											}
											default:
											{
												if (strcmp(currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName) != 0)
												{
													printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName);
													return;
												}
											}
										}
									}
								}
								else if (subTypeItem->m_Data->categoryOfType == Pointer)
								{
									indexArray++;
									if ((currentCommandLine[indexArray].kind != POINTER))
									{
										printErrorOfPointerAfterVariable(currentCommandLine[indexArray]);
										return;
									}
									if (strcmp(currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName) != 0)
									{
										printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName);
										return;
									}
								}
								else if (subTypeItem->m_Data->categoryOfType == Array)
								{
									indexArray++;
									if (currentCommandLine[indexArray].kind != SEPERATION_SIGN_BRACKET_OPEN)
									{
										printErrorOfArrayAfterVariable(currentCommandLine[indexArray]);
										return;
									}
									indexArray++;
									indexArray = handleWithIndexesInsideArray(indexArray);
									if (indexArray == -1)
									{
										return;
									}
									if (currentCommandLine[indexArray].kind == SEPERATION_SIGN_SEMICOLON)
									{
										indexArray--;
									}
								}
								else if (subTypeItem->m_Data->categoryOfType == Basic)
								{
									if (strcmp(currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName) != 0)
									{
										printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName);
										return;
									}
									if (currentCommandLine[indexArray + 1].kind == POINTER)
									{
										printErrorVaiableWrongType(currentCommandLine[indexArray], Pointer);
										return;
									}
									else if (currentCommandLine[indexArray + 1].kind == SEPERATION_SIGN_BRACKET_OPEN)
									{
										printErrorVaiableWrongType(currentCommandLine[indexArray], Array);
										return;
									}
								}
							}
							else
							{
								if (strcmp(currentKindTypeOfAllTheCommands, realType) == 0)
								{
									if (strcmp(currentItem->m_Data->typeOfVariable, realType) != 0)
									{
										printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentItem->m_Data->typeOfVariable, realType);
										return;
									}
								}
								else
								{
									if (strcmp(currentItem->m_Data->typeOfVariable, integerType) != 0)
									{
										printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentItem->m_Data->typeOfVariable, integerType);
										return;
									}
								}
							}
							break;
						}
						case UNARY_OP_AMP:
						{
							indexArray++;
							if (isLeftCommandSideWithRoof == 1)
							{
								fprintf(yyoutSemantic, "(Line %d) Cannot assign adress to pointer that point to variable .\n", currentCommandLine[indexArray].lineNumber);
								return;
							}
							currentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[indexArray].lexeme));
							if (currentItem == NULL)
							{
								printVarNotDefined(currentCommandLine[indexArray].lineNumber, currentCommandLine[indexArray].lexeme);
								return;
							}
							else if (currentItem->m_Data->role == UserDefinedType)
							{
								printExpectedTypeOrVar(currentCommandLine[indexArray],currentItem->m_Data->role);
								return;
							}
							if (strcmp(currentItem->m_Data->typeOfVariable, currentKindTypeOfAllTheCommands) != 0)
							{
								printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, currentItem->m_Data->typeOfVariable);
								return;
							}
							currentKindOfToken = currentCommandLine[indexArray].kind;
							if (currentItem->m_Data->subType != NULL)
							{
								subTypeItem = currentItem->m_Data->subType;
								if (strcmp(currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName) != 0)
								{
									printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentKindTypeOfAllTheCommands, subTypeItem->m_Data->basicSubTypeName);
									return;
								}
							}
							else
							{
								if (strcmp(currentKindTypeOfAllTheCommands, realType) == 0)
								{
									if (strcmp(currentItem->m_Data->typeOfVariable, realType) != 0)
									{
										printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentItem->m_Data->typeOfVariable, realType);
										return;
									}
								}
								else
								{
									if (strcmp(currentItem->m_Data->typeOfVariable, integerType) != 0)
									{
										printDiffrentTypes(currentCommandLine[indexArray].lineNumber, currentItem->m_Data->typeOfVariable, integerType);
										return;
									}
								}
							}
							break;
						}
					}
					indexArray++;
				}
			}
		}
	}
}

int handleWithIndexesInsideArray(int i_IndexInsideArray)
{	
	DataItem* CurrentItem;
	eTOKENS currentKind;
	while (currentCommandLine[i_IndexInsideArray].kind!= SEPERATION_SIGN_BRACKET_CLOSE)
	{
		currentKind = currentCommandLine[i_IndexInsideArray].kind;
		switch (currentKind)
		{
			case INT_NUM:
			{
				i_IndexInsideArray++;
				break;
			}
			case ID:
			{
				CurrentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[i_IndexInsideArray].lexeme));
				if (CurrentItem == NULL)
				{
					printVarNotDefined(currentCommandLine[i_IndexInsideArray].lineNumber, currentCommandLine[i_IndexInsideArray].lexeme);
					return -1;
				}
				else if (CurrentItem->m_Data->role == UserDefinedType)
				{
					printExpectedTypeOrVar(currentCommandLine[i_IndexInsideArray], CurrentItem->m_Data->role);
					return -1;
				}
				if (strcmp(CurrentItem->m_Data->typeOfVariable, stringFromeTOKENS(KEYWORD_INTEGER))==0)
				{
					i_IndexInsideArray++;
				}
				else if (strcmp(CurrentItem->m_Data->typeOfVariable, stringFromeTOKENS(KEYWORD_REAL))==0)
				{
					indexArayIsNotIntger(currentCommandLine[i_IndexInsideArray]);
					//printDiffrentTypes(currentCommandLine[i_IndexInsideArray].lineNumber, CurrentItem->m_Data->typeOfVariable, stringFromeTOKENS(KEYWORD_REAL));
					return -1;
				}
				else
				{					
					int keyOfSubItem = StringToIntHash(CurrentItem->m_Data->typeOfVariable);
					DataItem* subTypeItem = CurrentItem->m_Data->subType;
					switch (CurrentItem->m_Data->categoryOfType)
					{
						case Pointer:
						{
							if (currentCommandLine[i_IndexInsideArray + 1].kind != POINTER)
							{
								printErrorOfPointerAfterVariable(currentCommandLine[i_IndexInsideArray]);
								return -1;
							}
							i_IndexInsideArray++;
						}
						case Basic:
						{
							if (strcmp(subTypeItem->m_Data->basicSubTypeName, stringFromeTOKENS(KEYWORD_INTEGER))==0)
							{
								i_IndexInsideArray++;
							}
							else if (strcmp(subTypeItem->m_Data->basicSubTypeName, stringFromeTOKENS(KEYWORD_REAL))==0)
							{
								indexArayIsNotIntger(currentCommandLine[i_IndexInsideArray]);
								//printDiffrentTypes(currentCommandLine[i_IndexInsideArray].lineNumber, subTypeItem->m_Data->typeOfVariable, stringFromeTOKENS(KEYWORD_REAL));
								return -1;
							}
							break;
						}
						case Array:
						{
							i_IndexInsideArray = handleWithIndexesInsideArray(i_IndexInsideArray + 2);
							if (i_IndexInsideArray == -1)
							{
								return -1;
							}
							break;
						}
					}
				}
				break;
			}
			case KEYWORD_SIZE_OF:
			{
				i_IndexInsideArray= handleWithSizeOfKeyword(i_IndexInsideArray);
				if (i_IndexInsideArray == -1)
				{
					return -1;
				}
				i_IndexInsideArray++;
				break;
			}
			default:
			{
				indexArayIsNotIntger(currentCommandLine[i_IndexInsideArray]);
				return -1;
			}
		}
	}
	return ++i_IndexInsideArray;
}

int handleWithSizeOfKeyword(int i_IndexInsideSizeofKeyword)
{
	i_IndexInsideSizeofKeyword++;
	Token currentToken = currentCommandLine[i_IndexInsideSizeofKeyword];
	char* integerType = stringFromeTOKENS(KEYWORD_INTEGER);
	DataItem* currentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[i_IndexInsideSizeofKeyword].lexeme));
	if (currentItem == NULL)
		{
		printTypeNotDefined(currentToken.lineNumber, currentToken.lexeme);
			return -1;
		}
	else if (currentItem->m_Data->role == Variable)
		{
		  fprintf(yyoutSemantic, "(Line %d) %s is not a type name\n", currentToken.lineNumber, currentToken.lexeme);
			return -1;
		}
	return i_IndexInsideSizeofKeyword;
}

int handleWithSizeOfKeywordInsideTheKeywordMalloc(int i_IndexInsideSizeofKeyword,char* i_CurrentType)
{
	i_IndexInsideSizeofKeyword++;
	char* integerType = stringFromeTOKENS(KEYWORD_INTEGER);
	DataItem* currentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[i_IndexInsideSizeofKeyword].lexeme));
	if (currentItem == NULL)
	{
		printTypeNotDefined(currentCommandLine[i_IndexInsideSizeofKeyword].lineNumber, currentCommandLine[i_IndexInsideSizeofKeyword].lexeme);
		return -1;
	}
	else if (currentItem->m_Data->role == Variable)
	{
		printExpectedTypeOrVar(currentCommandLine[i_IndexInsideSizeofKeyword], Variable);
		return -1;
	}
	if (strcmp(currentItem->m_Data->name, i_CurrentType) != 0)
	{
		printDiffrentTypes(currentCommandLine[i_IndexInsideSizeofKeyword].lineNumber, currentItem->m_Data->basicSubTypeName, i_CurrentType);
		return -1;
	}
	return i_IndexInsideSizeofKeyword;
}

void printExpectedTypeOrVar(Token i_CurrentToken,eRoleTypes i_UnexpectedCurrentRole)
{

	if (i_UnexpectedCurrentRole == Variable)
	{
		fprintf(yyoutSemantic, "(Line %d) Unexpected Variable, Lexeme : %s\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
	}
	else
	{
		fprintf(yyoutSemantic, "(Line %d) Unexpected User defined type, Lexeme : %s\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
	}

}

void printErrorAssignVaribleToPointer(Token i_CurrentToken)
{
	fprintf(yyoutSemantic, "(Line %d) Trying to assign Variable:%s into Pointer,Need to write '&' before the variable\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
}

void printErrorOfPointerAfterVariable(Token i_CurrentToken)
{
	fprintf(yyoutSemantic, "(Line %d) Need to write '^' after the variable : %s\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
}

void printErrorOfArrayAfterVariable(Token i_CurrentToken)
{
	fprintf(yyoutSemantic, "(Line %d) Need to write '[X]' after the variable : %s\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
}

void printErrorVaiableWrongType(Token i_CurrentToken, eCategoryOfType i_UnexpectedCategory)
{
	

	switch (i_UnexpectedCategory)
	{
		case Basic:
		{
			fprintf(yyoutSemantic, "(Line %d) The variable:%s is not of Basic type\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
			break;
		}
		case Pointer:
		{
			fprintf(yyoutSemantic, "(Line %d) The variable:%s is not of Pointer type\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
			break;
		}
		case Array:
		{
			fprintf(yyoutSemantic, "(Line %d) The variable:%s is not of Array type\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
			break;
		}
	}
}

void printTypeNotDefined(int i_Line, char* i_Lexeme)
{
	sizeOfTokensInCommandLine = 0;
	fprintf(yyoutSemantic, "(Line %d) Type %s not Defined .\n", i_Line, i_Lexeme);
}

void printVarNotDefined(int i_Line, char* i_Lexeme)
{
	sizeOfTokensInCommandLine = 0;
	fprintf(yyoutSemantic, "(Line %d) Var %s not Defined .\n", i_Line, i_Lexeme);
}

void printDiffrentTypes(int i_Line, char* i_TypeFirst, char* i_TypeSecond)
{
	sizeOfTokensInCommandLine = 0;
	fprintf(yyoutSemantic, "(Line %d) Inconsistency of types in assignment: left side is of type %s, while right side is of type %s.\n", i_Line, i_TypeFirst, i_TypeSecond);
}

void indexArayIsNotIntger(Token i_CurrentToken)
{
	fprintf(yyoutSemantic, "(Line %d)  %s : is not integer .\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
}

void printErrorOfPtrInsideFree(Token i_CurrentToken)
{
	fprintf(yyoutSemantic, "(Line %d)  Tyring to Free %s That's is not a type of pointer.\n", i_CurrentToken.lineNumber, i_CurrentToken.lexeme);
}

void duplicateIdInTheSameScope(char* i_NameOfDuplicatedDecleration, int i_LineNumber)
{
	fprintf(yyoutSemantic, "(Line %d) duplicated declaration of %s\n", i_LineNumber, i_NameOfDuplicatedDecleration);
}

void commandsHandlerTypeOfUnrayOpIncrement()
{
	DataItem* currentItem = NULL;
	DataItem* subTypeItem = NULL;
	int indexArray = 0;
	char* typeVariable = NULL;
	char* integerType = stringFromeTOKENS(KEYWORD_INTEGER);
	char* realType = stringFromeTOKENS(KEYWORD_REAL);
	currentItem = searchInsideHashTableIfTheSubTypeExist(StringToIntHash(currentCommandLine[indexArray].lexeme));
	if (currentItem == NULL)
	{
		printVarNotDefined(currentCommandLine[indexArray].lineNumber, currentCommandLine[indexArray].lexeme);
		return;
	}
	else if (currentItem->m_Data->role == UserDefinedType)
	{
		printExpectedTypeOrVar(currentCommandLine[indexArray], UserDefinedType);
		return;
	}
	typeVariable = currentItem->m_Data->typeOfVariable;
	if (currentItem->m_Data->subType != NULL)
	{
		subTypeItem = currentItem->m_Data->subType;
		typeVariable = subTypeItem->m_Data->basicSubTypeName;
	}
	if ((strcmp(typeVariable, integerType) != 0))
	{
		printDiffrentTypes(currentCommandLine[indexArray].lineNumber, typeVariable, integerType);
		return;
	}

}
