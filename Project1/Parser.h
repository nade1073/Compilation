#ifndef PARSER
#define PARSER

#include <stdio.h>
#include "Token.h"
extern FILE *yyoutSyntax;
void match(eTOKENS);
void REL_OP_CASES(Token *currentToken);
int CheckIfTokenInFollowArr(Token* currentToken, eTOKENS *followArr, int followArrsize);
void parser_PROGRAM();
void parser_BLOCK();
void parser_DEFINITIONS();
void parser_DEFINITION();
void parser_DEFINITIONS_TAG();
void parser_VAR_DEFINITION_TAG();
void parser_TYPE_INDICATOR();
void parser_POINTER_TYPE_TAG();
void parser_COMMANDS();
void parser_COMMANDS_TAG();
void parser_COMMAND();
void parser_EXPRESSION();
void parser_EXPRESSION_TAG();
void parser_RECIVER();
void parser_RECIVER_TAG();
void HandlingErrors(Token* currentToken, eTOKENS *followArr, int followArrSize, eTOKENS *expectedTokens, int expectedTokensSize);
#endif
