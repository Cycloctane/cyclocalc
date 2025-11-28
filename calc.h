#ifndef _CALC_H_
#define _CALC_H_

#include "lexer.h"

typedef struct ASTNode {
    int val;
    TokenType type;
    struct ASTNode *left;
    struct ASTNode *right;
} ASTNode;

typedef enum CalcError {
    NoError = 0,
    ErrorInvalidOp,
    ErrorZeroDivision,
    ErrorNegativeShift,
} CalcError;

typedef struct CalcResult {
    CalcError err;
    int val;
} CalcResult;

ASTNode *parse(TokenStream *tokens, int bp);

CalcResult eval_ast(ASTNode *root);

void free_ast(ASTNode *root);

CalcResult calc(TokenStream *tokens);

#endif
