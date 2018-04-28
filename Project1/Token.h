#ifndef TOKEN_H
#define TOKEN_H



typedef enum eTOKENS
{
	AR_OP_ADDITION,
	AR_OP_SUBTRACTION,
	AR_OP_MULTIPLICATION,
	AR_OP_DIVISION,
	AR_OP_POWER,
	UNARY_OP_INCREMENT,
	UNARY_OP_AMP,
	REL_OP_LESS,
	REL_OP_EQUAL_OR_LESS,
	REL_OP_EQUAL,
	REL_OP_NOT_EQUAL,
	REL_OP_GREATER,
	REL_OP_EQUAL_OR_GREATER,
	ASSIGNMENT,
	POINTER,
	ID,
	INT_NUM,
	REL_NUM,
	KEYWORD_BLOCK,
	KEYWORD_BEGIN,
	KEYWORD_END,
	KEYWORD_TYPE,
	KEYWORD_IS,
	KEYWORD_INTEGER,
	KEYWORD_REAL,
	KEYWORD_ARRAY,
	KEYWORD_OF,
	KEYWORD_WHEN,
	KEYWORD_DO,
	KEYWORD_DEFAULT,
	KEYWORD_END_WHEN,
	KEYWORD_FOR,
	KEYWORD_END_FOR,
	KEYWORD_FREE,
	KEYWORD_SIZE_OF,
	KEYWORD_MALLOC,
	SEPERATION_SIGN_COLON,
	SEPERATION_SIGN_SEMICOLON,
	SEPERATION_SIGN_PARENT_OPEN,
	SEPERATION_SIGN_PARENT_CLOSE,
	SEPERATION_SIGN_BRACKET_OPEN,
	SEPERATION_SIGN_BRACKET_CLOSE,
	END_OF_FILE

}eTOKENS;

typedef struct Token
{
	eTOKENS kind;
	char *lexeme;
	int lineNumber;
}Token;

typedef struct Node
{
	Token *tokensArray;
	struct Node *prev;
	struct Node *next;
} Node;

void create_and_store_token(eTOKENS kind, int numOfLine, char* lexeme);
Token *next_token();
Token *back_token();

char* stringFromeTOKENS(enum eTOKENS f);
void resetVarsAndList();



void addTokenNode();
#endif
