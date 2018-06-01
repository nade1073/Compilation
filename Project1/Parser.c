#include <stdio.h>
#include "Parser.h"
#include "Token.h"
#include "Stack.h"
#include <string.h>

FILE *yyoutSyntax;
Token *currentToken;
StackOfHashTables* stack;
DataItem* tempDataItem;

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
	push(stack,hashTableToPushToStack);

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
	if (tempDataItem != NULL)
	{
		free(tempDataItem);
	}
	tempDataItem = (DataItem*)malloc(sizeof(DataItem));
	switch ((*currentToken).kind)
	{
		case ID:
		{
			tempDataItem->m_Data = (Data*)malloc(sizeof(DataItem));
			tempDataItem->m_Data->name = currentToken->lexeme;
			tempDataItem->m_Data->categoryOfType = Basic;
			tempDataItem->m_Data->role = Variable;

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
			fprintf(yyoutSyntax, "{TYPE_DEFINITION -> type type_name is TYPE_INDICATOR}\n");
			if (match(ID) == 0)
			{
				HandleMatchError(followArray, folowArraySize);
				break;
			}
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
			HashTable* m_CurrentHashTableOfBlock = top(stack);
			insert(m_CurrentHashTableOfBlock, tempDataItem);
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
		fprintf(yyoutSyntax, "{TYPE_INDICATOR -> BASIC_TYPE -> INTEGER}\n");
		break;
	}

	case KEYWORD_REAL:
	{
		fprintf(yyoutSyntax, "{TYPE_INDICATOR -> BASIC_TYPE -> REAL}\n");
		break;
	}
	case KEYWORD_ARRAY:
	{
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
			fprintf(yyoutSyntax, "{TYPE_INDICATOR -> ARRAY_TYPE -> array [INT_NUM] of INTEGER }\n");
			break;
		}
		case KEYWORD_REAL:
		{
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
		fprintf(yyoutSyntax, "{POINTER_TYPE_TAG -> ^INTEGER}\n");
		break;
	}
	case KEYWORD_REAL:
	{
		fprintf(yyoutSyntax, "{POINTER_TYPE_TAG -> ^REAL}\n");
		break;
	}
	case ID:
	{
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
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
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
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
		if (match(UNARY_OP_INCREMENT) == 0)
		{
			HandleMatchError(followArray, folowArraySize);
			return;
		}
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
		if (match(SEPERATION_SIGN_PARENT_CLOSE))
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
		eTOKENS *expectedTokens[] = { ID,KEYWORD_WHEN, KEYWORD_FOR,KEYWORD_FREE,KEYWORD_BLOCK };
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
		fprintf(yyoutSyntax, "{EXPRESSION -> INT_NUM}\n");
		break;
	}
	case REL_NUM:
	{
		fprintf(yyoutSyntax, "{EXPRESSION -> REL_NUM}\n");
		break;
	}
	case ID:
	{
		fprintf(yyoutSyntax, "{EXPRESSION -> id EXPRESSION_TAG}\n");
		parser_EXPRESSION_TAG();
		break;
	}
	case UNARY_OP_AMP:
	{
		fprintf(yyoutSyntax, "{EXPRESSION -> &id}\n");
		if (match(ID) == 0)
		{
			HandleMatchError(followArray, followArraySize);
			return;
		}
		break;
	}
	case KEYWORD_SIZE_OF:
	{
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
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> [EXPRESSION]}\n");
		parser_EXPRESSION();
		if (match(SEPERATION_SIGN_BRACKET_CLOSE) == 0)
		{
			HandleMatchError(followArray, followArraySize);
			return;
		}
		break;
	}
	case POINTER:
	{
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
	{
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> EPSILON}\n");
		back_token();
		break;
	}
	default:
	{
		eTOKENS expectedTokens[] = { SEPERATION_SIGN_BRACKET_OPEN,POINTER,AR_OP_ADDITION,AR_OP_SUBTRACTION,AR_OP_MULTIPLICATION,AR_OP_DIVISION,UNARY_OP_INCREMENT,AR_OP_POWER,REL_OP_EQUAL,REL_OP_NOT_EQUAL,REL_OP_EQUAL_OR_GREATER,REL_OP_EQUAL_OR_LESS,REL_OP_GREATER,REL_OP_LESS,SEPERATION_SIGN_BRACKET_CLOSE };
		int expectedTokenSize = 15;
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
		fprintf(yyoutSyntax, "{RECIVER_TAG -> [EXPRESSION]}\n");
		parser_EXPRESSION();
		if (match(SEPERATION_SIGN_BRACKET_CLOSE) == 0)
		{
			HandleMatchError(followArray, sizeOfFollowArray);
			return;
		}
		break;
	}
	case POINTER:
	{
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

