#ifndef _CALC_H_
#define _CALC_H_

#include "lexer.h"

typedef struct ASTNode {
    long long val;
    TokenType type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode *parse(TokenStream *tokens, int bp);

long long eval_ast(ASTNode *root);

void free_ast(ASTNode *root);

long long calc(TokenStream *tokens);

#endif
