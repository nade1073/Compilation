#include <stdio.h>
#include "Parser.h"
#include "Token.h"
#include <string.h>
 FILE *yyoutSyntax;



void match(eTOKENS i_ActualKind)
{
	Token *currentToken = next_token();
	
	if ((*currentToken).kind != i_ActualKind)
	{
		fprintf(yyoutSyntax, "Expected token '%s' at line: %d, Actual token '%s', lexeme: '%s'.\n",stringFromeTOKENS(i_ActualKind), (*currentToken).lineNumber, stringFromeTOKENS((*currentToken).kind), (*currentToken).lexeme);
		if ((*currentToken).kind == END_OF_FILE)
		{
			exit(0);
		}
		
	}
	else if (i_ActualKind == END_OF_FILE)
	{
		fprintf(yyoutSyntax, "EOF\n");
		return;
	}
	
}

void REL_OP_CASES(Token *currentToken)
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS expectedTokens[] = { REL_OP_LESS , REL_OP_EQUAL_OR_LESS , REL_OP_EQUAL , REL_OP_NOT_EQUAL , REL_OP_GREATER , REL_OP_EQUAL_OR_GREATER };
	switch ((*currentToken).kind)
	{
		case REL_OP_LESS :
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

int CheckIfTokenInFollowArr(Token* currentToken, eTOKENS *followArr,int followArrsize)
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

void HandlingErrors(Token* currentToken, eTOKENS *followArr,int followArrSize, eTOKENS *expectedTokens,int expectedTokensSize)
{
	int i;
	char expectedTokensString[250] = "";
	int TokenFound;
	TokenFound = CheckIfTokenInFollowArr(currentToken, followArr, followArrSize);
	for (i = 0; i < expectedTokensSize; i++)
	{
		strcat(expectedTokensString, "'");
		strcat(expectedTokensString, stringFromeTOKENS(expectedTokens[i]));
		strcat(expectedTokensString, "' ");
	}
		
	
	while (TokenFound == 0 && (*currentToken).kind != END_OF_FILE)
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
		TokenFound = CheckIfTokenInFollowArr(currentToken, followArr, followArrSize);

	}
		currentToken = back_token();
}

void parser_PROGRAM()
{
	fprintf(yyoutSyntax, "{PROGRAM -> BLOCK}\n");
	parser_BLOCK();
	match(END_OF_FILE);
}

void parser_BLOCK()
{
	fprintf(yyoutSyntax, "{BLOCK -> block DEFINITIONS; begin COMMANDS; end}\n");
	match(KEYWORD_BLOCK);
	parser_DEFINITIONS();
	match(SEPERATION_SIGN_SEMICOLON);
	match(KEYWORD_BEGIN);
	parser_COMMANDS();
	match(SEPERATION_SIGN_SEMICOLON);
	match(KEYWORD_END);
}

void parser_DEFINITIONS()
{
	fprintf(yyoutSyntax, "{DEFINITIONS -> DEFINITION DEFINITIONS_TAG }\n");
	parser_DEFINITION();
	parser_DEFINITIONS_TAG();
}

void parser_DEFINITION()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON};
	eTOKENS expectedTokens[] = { ID , KEYWORD_TYPE };
	Token *currentToken = next_token();
	switch ((*currentToken).kind)
	{
		case ID:
		{
			fprintf(yyoutSyntax, "{VAR_DEFINITION -> id: VAR_DEFINITION_TAG}\n");
			match(SEPERATION_SIGN_COLON);
			parser_VAR_DEFINITION_TAG();
			break;
		}
		case KEYWORD_TYPE:
		{
			fprintf(yyoutSyntax, "{TYPE_DEFINITION -> type type_name is TYPE_INDICATOR}\n");
			match(ID);
			match(KEYWORD_IS);
			parser_TYPE_INDICATOR();
			break;
		}
		default:
		{
			HandlingErrors(currentToken, followARR, 1, expectedTokens, 2);
		}
	}
}

void parser_DEFINITIONS_TAG()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS expectedTokens[] = { SEPERATION_SIGN_SEMICOLON };
	Token *currentToken = next_token();
		switch ((*currentToken).kind)
		{
		case SEPERATION_SIGN_SEMICOLON:
		{
			currentToken = next_token();
			switch ((*currentToken).kind)
			{
				case KEYWORD_BEGIN:
				{
					fprintf(yyoutSyntax, "{DEFINITION_TAG -> Epsilon}\n");
					back_token();
					back_token();
					break;
				}
				default:
				{
					fprintf(yyoutSyntax, "{DEFINITION_TAG -> DEFINITION DEFINITONS_TAG}\n");
					back_token();
					parser_DEFINITION();
					parser_DEFINITIONS_TAG();		
				}
			}
			break;
		}
		case KEYWORD_TYPE:
		{
			fprintf(yyoutSyntax, "{TYPE_DEFINITION -> type type_name is TYPE_INDICATOR}\n");
			match(ID);
			match(KEYWORD_IS);
			parser_TYPE_INDICATOR();
			break;
		}
		default:
		{
			HandlingErrors(currentToken, followARR, 1, expectedTokens, 1);
		}
	}


 }

void parser_VAR_DEFINITION_TAG()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS expectedTokens[] = { KEYWORD_INTEGER , KEYWORD_REAL , ID };
	Token *currentToken = next_token();
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
			HandlingErrors(currentToken, followARR, 1, expectedTokens, 3);
		}
	}
}

void parser_TYPE_INDICATOR()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS expectedTokens[] = { KEYWORD_INTEGER , KEYWORD_REAL , KEYWORD_ARRAY , POINTER };
	eTOKENS expectedTokensInsideIf[] = { KEYWORD_INTEGER , KEYWORD_REAL };
	Token *currentToken = next_token();
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
			fprintf(yyoutSyntax, "{VAR_DEFINITION_TAG -> type_name}\n");
			match(SEPERATION_SIGN_BRACKET_OPEN);
			match(INT_NUM);
			match(SEPERATION_SIGN_BRACKET_CLOSE);
			match(KEYWORD_OF);
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
					HandlingErrors(currentToken, followARR, 1, expectedTokensInsideIf, 2);
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
			HandlingErrors(currentToken, followARR, 1, expectedTokens, 4);
		}
	}
}


void parser_POINTER_TYPE_TAG()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS expectedTokens[] = { KEYWORD_INTEGER , KEYWORD_REAL , ID };
	Token *currentToken = next_token();
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
			HandlingErrors(currentToken, followARR, 1, expectedTokens, 3);
		}
	}
}

void parser_COMMANDS()
{
	 parser_COMMAND();
	 parser_COMMANDS_TAG();
}

void parser_COMMANDS_TAG()
 {
	 eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	 eTOKENS expectedTokens[] = { SEPERATION_SIGN_SEMICOLON };
	 Token *currentToken = next_token();

	 switch ((*currentToken).kind)
	{
		 case SEPERATION_SIGN_SEMICOLON:
		 {
			 Token *currentToken = next_token();
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
				 default :
				 {
					 fprintf(yyoutSyntax, "{COMMANDS_TAG -> COMMAND COMMANDS_TAG}\n");
					 back_token();
					 parser_COMMAND();
					 parser_COMMANDS_TAG();
				 }
			 }
			 break;
		 }
		 default:
		 {
			 HandlingErrors(currentToken, followARR, 1, expectedTokens, 1);
		 }
	 }
 }

void parser_COMMAND()
{
	eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON };
	eTOKENS *expectedTokens[] = { ID,KEYWORD_WHEN, KEYWORD_FOR,KEYWORD_FREE,KEYWORD_BLOCK };
	eTOKENS InsideexpectedTokens[] = { REL_OP_LESS , REL_OP_EQUAL_OR_LESS , REL_OP_EQUAL , REL_OP_NOT_EQUAL , REL_OP_GREATER , REL_OP_EQUAL_OR_GREATER };
	Token *currentToken = next_token();
	switch ((*currentToken).kind)
	{
		case ID:
		{
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
						match(SEPERATION_SIGN_PARENT_OPEN);
						match(KEYWORD_SIZE_OF);
						match(SEPERATION_SIGN_PARENT_OPEN);
						match(ID);
						match(SEPERATION_SIGN_PARENT_CLOSE);
						match(SEPERATION_SIGN_PARENT_CLOSE);
						break;
					}
					default:
					{
						fprintf(yyoutSyntax, "{COMMAND -> ID = EXPRESSION}\n");
						switch ((*currentToken).kind)
						{
						case REL_NUM:
						case ID:
						case INT_NUM:
						case UNARY_OP_AMP:
						case KEYWORD_SIZE_OF:
						{
							back_token();
							parser_EXPRESSION();
							break;
						}
						default:
						{
							HandlingErrors(currentToken, followARR, 1, InsideexpectedTokens, 6);
						}
						}
					}
					}
					break;
				}
				default:
				{
					fprintf(yyoutSyntax, "{COMMAND -> RECEIVER = EXPRESSION}\n");
					back_token();
					parser_RECIVER_TAG();
					match(ASSIGNMENT);
					parser_EXPRESSION();
				}
			}
			break;
		}
		case KEYWORD_WHEN:
		{
			fprintf(yyoutSyntax, "{COMMAND -> when (EXPRESSION rel_op EXPRESSION) do COMMANDS; default COMMANDS; end_when}\n");
			match(SEPERATION_SIGN_PARENT_OPEN);
			parser_EXPRESSION();
			currentToken = next_token();
			switch ((*currentToken).kind)
			{
			case REL_OP_LESS:
			{
				parser_EXPRESSION();
			}
			case REL_OP_EQUAL_OR_LESS:
			{
				parser_EXPRESSION();
			}
			case REL_OP_EQUAL:
			{
				parser_EXPRESSION();
			}
			case REL_OP_NOT_EQUAL:
			{
				parser_EXPRESSION();
			}
			case REL_OP_GREATER:
			{
				parser_EXPRESSION();
			}
			case REL_OP_EQUAL_OR_GREATER:
			{
				parser_EXPRESSION();
			}
			default:
			{
				eTOKENS followARRRelOP[9] = { SEPERATION_SIGN_SEMICOLON, REL_OP_LESS ,REL_OP_EQUAL_OR_LESS ,REL_OP_EQUAL ,REL_OP_NOT_EQUAL ,REL_OP_GREATER ,REL_OP_EQUAL_OR_GREATER ,SEPERATION_SIGN_PARENT_CLOSE ,SEPERATION_SIGN_BRACKET_CLOSE };
				eTOKENS expectedTokensRelOp[6] = { REL_OP_LESS, REL_OP_EQUAL_OR_LESS , REL_OP_EQUAL , REL_OP_NOT_EQUAL , REL_OP_GREATER, REL_OP_EQUAL_OR_GREATER };
				HandlingErrors(currentToken, followARRRelOP, 9, expectedTokensRelOp, 6);
			}
			}
			match(SEPERATION_SIGN_PARENT_CLOSE);
			match(KEYWORD_DO);
			parser_COMMANDS();
			match(SEPERATION_SIGN_SEMICOLON);
			match(KEYWORD_DEFAULT);
			parser_COMMANDS();
			match(SEPERATION_SIGN_SEMICOLON);
			match(KEYWORD_END_WHEN);
			break;
		}
		case KEYWORD_FOR:
		{
			fprintf(yyoutSyntax, "{COMMAND -> for (id = EXPRESSION; id rel_op EXPRESSION; id++) COMMANDS; end_for}");
			match(SEPERATION_SIGN_PARENT_OPEN);
			match(ID);
			match(ASSIGNMENT);
			parser_EXPRESSION();
			match(SEPERATION_SIGN_SEMICOLON);
			match(ID);
			currentToken = next_token();
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
					HandlingErrors(currentToken, followARR, 1, InsideexpectedTokens, 6);
				}
			}
			match(SEPERATION_SIGN_SEMICOLON);
			match(ID);
			match(UNARY_OP_INCREMENT);
			match(SEPERATION_SIGN_PARENT_CLOSE);
			parser_COMMANDS();
			match(SEPERATION_SIGN_SEMICOLON);
			match(KEYWORD_END_FOR);
			break;
		}
		case KEYWORD_FREE:
		{
			fprintf(yyoutSyntax, "{COMMAND -> free(id)}");
			match(SEPERATION_SIGN_PARENT_OPEN);
			match(ID);
			match(SEPERATION_SIGN_PARENT_CLOSE);
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
			HandlingErrors(currentToken, followARR, 1, expectedTokens, 5);
		}
	}
}

void parser_EXPRESSION()
 {
	 eTOKENS followARR[] = { SEPERATION_SIGN_SEMICOLON, REL_OP_LESS ,REL_OP_EQUAL_OR_LESS ,REL_OP_EQUAL ,REL_OP_NOT_EQUAL ,REL_OP_GREATER ,REL_OP_EQUAL_OR_GREATER ,SEPERATION_SIGN_PARENT_CLOSE ,SEPERATION_SIGN_BRACKET_CLOSE };
	 eTOKENS expectedTokens[] = { SEPERATION_SIGN_BRACKET_OPEN, REL_NUM , ID , UNARY_OP_AMP , KEYWORD_SIZE_OF };
	 Token *currentToken = next_token();
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
			 match(ID);
			 break;
		 }
		 case KEYWORD_SIZE_OF:
		 {
			 fprintf(yyoutSyntax, "{EXPRESSION -> size_of(type_name)}\n");
			 match(SEPERATION_SIGN_PARENT_OPEN);
			 match(ID);
			 match(SEPERATION_SIGN_PARENT_CLOSE);
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
	 eTOKENS expectedTokens[] = { SEPERATION_SIGN_BRACKET_OPEN , POINTER , AR_OP_ADDITION , AR_OP_SUBTRACTION , AR_OP_MULTIPLICATION , AR_OP_DIVISION , AR_OP_POWER };
	 Token *currentToken = next_token();
	 switch ((*currentToken).kind)
	 {
		 case SEPERATION_SIGN_BRACKET_OPEN:
			 {
				 fprintf(yyoutSyntax, "{EXPRESSION_TAG -> [EXPRESSION]}\n");
				 parser_EXPRESSION();
				 match(SEPERATION_SIGN_BRACKET_CLOSE);
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
		 case AR_OP_POWER:
		 {
			 fprintf(yyoutSyntax, "{EXPRESSION_TAG -> ar_op EXPRESSION}\n");
			 parser_EXPRESSION();
			 break;
		 }
		 default:
		 {
			 HandlingErrors(currentToken, followARR, 9, expectedTokens, 7);
		 }
	 }
 }
 
 void parser_RECIVER()
 {
	 match(ID);
	 parser_RECIVER_TAG();
 }

 void parser_RECIVER_TAG()
 {
	 eTOKENS followARR[] = { ASSIGNMENT };
	 eTOKENS expectedTokens[] = {SEPERATION_SIGN_BRACKET_OPEN , POINTER};
	 Token *currentToken = next_token();
	 switch ((*currentToken).kind)
	 {
		 case SEPERATION_SIGN_BRACKET_OPEN:
		 {
			 fprintf(yyoutSyntax, "{RECIVER_TAG -> [EXPRESSION]}\n");
			 parser_EXPRESSION();
			 match(SEPERATION_SIGN_BRACKET_CLOSE);
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
 