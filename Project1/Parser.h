#ifndef PARSER
#define PARSER

#include <stdio.h>
#include "Token.h"
#include "HashTable.h"
extern FILE *yyoutSyntax;
extern FILE *yyoutSemantic;
int match(eTOKENS);
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
void parser_RECIVER_TAG();
void HandlingErrors(Token* currentToken, eTOKENS *followArr, int followArrSize, eTOKENS *expectedTokens, int expectedTokensSize);
void HandleMatchError(eTOKENS *i_FollowArray, int i_SizeOfFollowArray);
void printErrorToTheFile(char* i_ExcepectedToken, int i_LineNumber, char* i_ActualToken, char* i_Lexeme);
void insterToHashTableIfIdIsValid();
DataItem* checkIfIdExists(int i_Key);
void printTypeNotDefined(int i_Line, char* i_Lexeme);
void handleWithVariableType();
void handleWithUserDefinedType();
DataItem* searchInsideHashTableIfTheSubTypeExist(int i_Key);
void printDiffrentTypes(int i_Line, char* i_TypeFirst, char* i_TypeSecond);
void assignCurrentTokenToCommands();
void commandsHandlerType();
void printExpectedTypeOrVar(Token i_CurrentToken, eRoleTypes i_UnexpectedCurrentRole);
void printErrorAssignVaribleToPointer(Token i_CurrentToken);
void printErrorVaiableWrongType(Token i_CurrentToken, eCategoryOfType i_UnexpectedCategory);
void printErrorOfPointerAfterVariableInTheRightSide(Token i_CurrentToken);

#endif
