#include <stdio.h>
#include "Parser.h"
#include "Token.h"
#include <string.h>

FILE *yyoutSyntax;
Token *currentToken;


void parser_PROGRAM()
{


	fprintf(yyoutSyntax, "{PROGRAM -> BLOCK}\n");
	parser_BLOCK();
	match(END_OF_FILE);

}

void parser_BLOCK()
{
	eTOKENS followArray[] = { END_OF_FILE, SEPERATION_SIGN_SEMICOLON };
	int folowArraySize = 2;

	fprintf(yyoutSyntax, "{BLOCK -> block DEFINITIONS; begin COMMANDS; end}\n");
	if (match(KEYWORD_BLOCK)==0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}

	parser_DEFINITIONS();
	if (match(SEPERATION_SIGN_SEMICOLON)==0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}

	if (match(KEYWORD_BEGIN)==0)
	{
		HandleMatchError(followArray, folowArraySize);
		return;
	}
	parser_COMMANDS();

	if (match(SEPERATION_SIGN_SEMICOLON)==0)
	{
		HandleMatchError(folowArray, folowArraySize);
		return;
	}

	if (match(KEYWORD_END)==0)
	{
		HandleMatchError(folowArray, folowArraySize);
		return;
	}
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
	int expectedTokensArraySize=2;
	currentToken = next_token();
	fprintf(yyoutSyntax, "{DEFINITION -> VAR_DEFINITION | TYPE_DEFINITION}\n");

	switch ((*currentToken).kind)
	{
		case ID:
		{
			fprintf(yyoutSyntax, "{VAR_DEFINITION -> id: VAR_DEFINITION_TAG}\n");
			if (match(SEPERATION_SIGN_COLON)==0)
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
			if (match(ID)==0)
			{
				HandleMatchError(followArray, folowArraySize);
				break;
			}
			if (match(KEYWORD_IS)==0)
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
	switch ((*currentToken).kind == KEYWORD_BEGIN)
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
			fprintf(yyoutSyntax, "{VAR_DEFINITION_TAG -> BASIC_TYPE -> INTEGER}\n");
			break;
		}
		case KEYWORD_REAL:
		{
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
			if (match(SEPERATION_SIGN_BRACKET_OPEN)==0)
			{
				HandleMatchError(followArray, folowArraySize);
				break;
			}
			if (match(INT_NUM)==0)
			{
				HandleMatchError(followArray, folowArraySize);
				break;
			}
			if (match(SEPERATION_SIGN_BRACKET_CLOSE)==0)
			{
				HandleMatchError(followArray, folowArraySize);
				break;
			}
			if (match(KEYWORD_OF)==0)
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
			if (match(ASSIGNMENT) == 0)
			{
				HandleMatchError(followArray, folowArraySize);
				return;
			}
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
					eTOKENS *expectedTokens[] = { KEYWORD_MALLOC,REL_NUM,ID,INT_NUM,UNARY_OP_AMP,KEYWORD_SIZE_OF };
					int sizeOfExpectedTokens = 6;
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
				if (match(SEPERATION_SIGN_PARENT_OPEN)==0)
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
				if (match(SEPERATION_SIGN_PARENT_CLOSE)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(KEYWORD_DO)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				parser_COMMANDS();
				if (match(SEPERATION_SIGN_SEMICOLON)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(KEYWORD_DEFAULT)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				parser_COMMANDS();
				if (match(SEPERATION_SIGN_SEMICOLON)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(KEYWORD_END_WHEN)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
			break;
			}
		case KEYWORD_FOR:
		{
				fprintf(yyoutSyntax, "{COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for}\n");
				if (match(SEPERATION_SIGN_PARENT_OPEN)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(ID)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(ASSIGNMENT)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				parser_EXPRESSION();
				if (match(SEPERATION_SIGN_SEMICOLON)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(ID)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
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
				if (match(SEPERATION_SIGN_SEMICOLON)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(ID)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(UNARY_OP_INCREMENT)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(SEPERATION_SIGN_PARENT_CLOSE)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				parser_COMMANDS();
				if (match(SEPERATION_SIGN_SEMICOLON)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(KEYWORD_END_FOR)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
			break;
		}
		case KEYWORD_FREE:
		{
				fprintf(yyoutSyntax, "{COMMAND -> free(id)}\n");
				if (match(SEPERATION_SIGN_PARENT_OPEN)==0)
				{
					HandleMatchError(followArray, folowArraySize);
					return;
				}
				if (match(ID)==0)
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


int match(eTOKENS i_ActualKind)
{
	int boolToReturn = 0;
	currentToken = next_token();
	if ((*currentToken).kind != i_ActualKind)
	{
		printErrorToTheFile(stringFromeTOKENS(i_ActualKind), (*currentToken).lineNumber ,stringFromeTOKENS((*currentToken).kind) ,(*currentToken).lexeme);
		if ((*currentToken).kind == END_OF_FILE)
		{
			boolToReturn = 0;
		}

		boolToReturn = 0;
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

void REL_OP_CASES(Token *currentToken)
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS expectedTokens[] = { REL_OP_LESS , REL_OP_EQUAL_OR_LESS , REL_OP_EQUAL , REL_OP_NOT_EQUAL , REL_OP_GREATER , REL_OP_EQUAL_OR_GREATER };
	switch ((*currentToken).kind)
	{
	case REL_OP_LESS:
	{
		parser_EXPRESSION();
		break;
	}
	case REL_OP_EQUAL_OR_LESS:
	{
		parser_EXPRESSION();
		break;
	}
	case REL_OP_EQUAL:
	{
		parser_EXPRESSION();
		break;
	}
	case REL_OP_NOT_EQUAL:
	{
		parser_EXPRESSION();
		break;
	}
	case REL_OP_GREATER:
	{
		parser_EXPRESSION();
		break;
	}
	case REL_OP_EQUAL_OR_GREATER:
	{
		parser_EXPRESSION();
		break;
	}
	default:
	{
		HandlingErrors(currentToken, followARR, 1, expectedTokens, 6);
	}
	}
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

void HandleMatchError(eTOKENS i_FollowArray, int i_SizeOfFollowArray)
{
	int tokenFound = 0;
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
		printErrorToTheFile(expectedTokens, (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
	}
	while (tokenFound == 0 && (*currentToken).kind != END_OF_FILE)
	{

		if (expectedTokensSize == 1)
		{
			fprintf(yyoutSyntax, "Expected token %s at line: %d,\nActual token '%s', lexeme: '%s'.\n",
				expectedTokensString, (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
		}
		else
		{
			fprintf(yyoutSyntax, "Expected one of tokens %s at line: %d,\nActual token '%s', lexeme: '%s'.\n",
				expectedTokensString, (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
		}
		currentToken = next_token();
	}
	currentToken = back_token();
}
















void parser_EXPRESSION()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON, REL_OP_LESS ,REL_OP_EQUAL_OR_LESS ,REL_OP_EQUAL ,REL_OP_NOT_EQUAL ,REL_OP_GREATER ,REL_OP_EQUAL_OR_GREATER ,SEPERATION_SIGN_PARENT_CLOSE ,SEPERATION_SIGN_BRACKET_CLOSE };
	eTOKENS expectedTokens[] = { SEPERATION_SIGN_BRACKET_OPEN, REL_NUM , ID , UNARY_OP_AMP , KEYWORD_SIZE_OF };
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
		if (!match(ID))
		{
			HandlingErrors(currentToken, followARR, 9, expectedTokens, 5);
			break;
		}
		break;
	}
	case KEYWORD_SIZE_OF:
	{
		fprintf(yyoutSyntax, "{EXPRESSION -> size_of(type_name)}\n");
		if (!match(SEPERATION_SIGN_PARENT_OPEN))
		{
			HandlingErrors(currentToken, followARR, 9, expectedTokens, 5);
			break;
		}
		if (!match(ID))
		{
			HandlingErrors(currentToken, followARR, 9, expectedTokens, 5);
			break;
		}
		if (!match(SEPERATION_SIGN_PARENT_CLOSE))
		{
			HandlingErrors(currentToken, followARR, 9, expectedTokens, 5);
			break;
		}
		break;
	}
	default:
	{
		HandlingErrors(currentToken, followARR, 9, expectedTokens, 5);
	}
	}
}

void parser_EXPRESSION_TAG()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON, REL_OP_LESS ,REL_OP_EQUAL_OR_LESS ,REL_OP_EQUAL ,REL_OP_NOT_EQUAL ,REL_OP_GREATER ,REL_OP_EQUAL_OR_GREATER ,SEPERATION_SIGN_PARENT_CLOSE ,SEPERATION_SIGN_BRACKET_CLOSE };
	eTOKENS expectedTokens[] = { SEPERATION_SIGN_BRACKET_OPEN , POINTER , AR_OP_ADDITION , AR_OP_SUBTRACTION , AR_OP_MULTIPLICATION , AR_OP_DIVISION , AR_OP_POWER,UNARY_OP_INCREMENT};
	currentToken = next_token();
	switch ((*currentToken).kind)
	{
	case SEPERATION_SIGN_BRACKET_OPEN:
	{
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> [EXPRESSION]}\n");
		parser_EXPRESSION();
		if (!match(SEPERATION_SIGN_BRACKET_CLOSE))
		{
			HandlingErrors(currentToken, followARR, 9, expectedTokens, 8);
			break;
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
	{
		fprintf(yyoutSyntax, "{EXPRESSION_TAG -> EPSILON}\n");
		back_token();
		break;
	}

	default:
	{
		HandlingErrors(currentToken, followARR, 9, expectedTokens, 8);
	}
	}
}

void parser_RECIVER()
{
	if (!match(ID))
	{
		return;
	}
	parser_RECIVER_TAG();
}

void parser_RECIVER_TAG()
{
	eTOKENS followARR[] = { ASSIGNMENT };
	eTOKENS expectedTokens[] = { SEPERATION_SIGN_BRACKET_OPEN , POINTER };
	currentToken = next_token();
	switch ((*currentToken).kind)
	{
		case SEPERATION_SIGN_BRACKET_OPEN:
		{
			fprintf(yyoutSyntax, "{RECIVER_TAG -> [EXPRESSION]}\n");
			parser_EXPRESSION();
			if (!match(SEPERATION_SIGN_BRACKET_CLOSE))
			{
				HandlingErrors(currentToken, followARR, 1, expectedTokens, 2);
				break;
			}
			break;
		}
		case POINTER:
		{
			fprintf(yyoutSyntax, "{RECIVER_TAG -> ^}\n");
			break;
		}
		default:
		{
			HandlingErrors(currentToken, followARR, 1, expectedTokens, 2);
		}
	}
}
