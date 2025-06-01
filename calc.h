#ifndef _CALC_H_
#define _CALC_H_

#include "lexer.h"

typedef struct ASTNode {
    int val;
    TokenType type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

ASTNode *parse(TokenStream *tokens, int bp);

int eval_ast(ASTNode *root);

void free_ast(ASTNode *root);

int calc(TokenStream *tokens);

#endif
