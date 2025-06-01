#ifndef _LEXER_H_
#define _LEXER_H_

typedef enum TokenType {
    Num,
    ParenLeft,
    ParenRight,
    OpAdd,
    OpSub,
    OpMul,
    OpDiv,
    OpMod,
    OpShiftLeft,
    OpShiftRight,
    OpAnd,
    OpXor,
    OpOr,
} TokenType;

typedef struct TokenNode {
    int val;
    TokenType type;
    struct TokenNode *next;
} TokenNode;

typedef struct TokenStream {
    TokenNode *head;
    TokenNode *pos;
} TokenStream;

TokenStream *tokenize(char *s);

void free_tokenstream(TokenStream *tokens);

TokenNode *next_token(TokenStream *tokens);

TokenNode *peek_token(TokenStream *tokens);

#endif
