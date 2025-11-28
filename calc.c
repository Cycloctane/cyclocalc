#include "calc.h"
#include <stdio.h>
#include <stdlib.h>

static void get_bp(TokenType type, int *left, int *right) {
    switch (type) {
    case OpAdd:
    case OpSub:
        *left = 100;
        *right = 101;
        return;
    case OpMul:
    case OpDiv:
    case OpMod:
        *left = 110;
        *right = 111;
        return;
    case OpShiftLeft:
    case OpShiftRight:
        *left = 40;
        *right = 41;
        return;
    case OpAnd:
        *left = 30;
        *right = 31;
        return;
    case OpXor:
        *left = 20;
        *right = 21;
        return;
    case OpOr:
        *left = 10;
        *right = 11;
        return;
    default:
        *left = -1;
        *right = -1;
        return;
    }
}

ASTNode *parse(TokenStream *tokens, int bp) {
    TokenNode *next = next_token(tokens);
    if (!next)
        return NULL;

    ASTNode *root_node;
    if (next->type == Num) {
        root_node = malloc(sizeof(ASTNode));
        *root_node = (ASTNode){.type = Num, .val = next->val, .left = NULL, .right = NULL};
    } else if (next->type == ParenLeft) {
        root_node = parse(tokens, 0);
        next_token(tokens);
    } else
        return NULL;

    while (1) {
        TokenNode *op = peek_token(tokens);
        if (!op || op->type == ParenRight)
            break;

        int lbp, rbp;
        get_bp(op->type, &lbp, &rbp);
        if (lbp <= bp)
            break;

        next_token(tokens);
        ASTNode *op_node = malloc(sizeof(ASTNode));
        *op_node = (ASTNode){.type = op->type, .left = root_node, .right = parse(tokens, rbp)};
        root_node = op_node;
    }
    return root_node;
}

CalcResult eval_ast(ASTNode *root) {
    if (!root)
        return (CalcResult){NoError, 0};
    if (root->type == Num)
        return (CalcResult){NoError, root->val};

    CalcResult left = eval_ast(root->left);
    if (left.err)
        return left;
    CalcResult right = eval_ast(root->right);
    if (right.err)
        return right;

    CalcResult res = {.err = NoError};
    switch (root->type) {
    case OpAdd:
        res.val = left.val + right.val;
        break;
    case OpSub:
        res.val = left.val - right.val;
        break;
    case OpMul:
        res.val = left.val * right.val;
        break;
    case OpDiv:
        if (right.val == 0) {
            res.err = ErrorZeroDivision;
            fprintf(stderr, "Division by zero: %d / 0\n", left.val);
        } else
            res.val = left.val / right.val;
        break;
    case OpMod:
        if (right.val == 0) {
            res.err = ErrorZeroDivision;
            fprintf(stderr, "Division by zero: %d %% 0\n", left.val);
        } else
            res.val = left.val % right.val;
        break;
    case OpShiftLeft:
        if (right.val < 0) {
            res.err = ErrorNegativeShift;
            fprintf(stderr, "Negative shifts: %d << %d\n", left.val, right.val);
        } else
            res.val = left.val << right.val;
        break;
    case OpShiftRight:
        if (right.val < 0) {
            res.err = ErrorNegativeShift;
            fprintf(stderr, "Negative shifts: %d >> %d\n", left.val, right.val);
        } else
            res.val = left.val >> right.val;
        break;
    case OpAnd:
        res.val = left.val & right.val;
        break;
    case OpXor:
        res.val = left.val ^ right.val;
        break;
    case OpOr:
        res.val = left.val | right.val;
        break;
    default:
        res.err = ErrorInvalidOp;
        fputs("Invalid operator\n", stderr);
    }
    return res;
}

void free_ast(ASTNode *root) {
    if (!root)
        return;
    free_ast(root->left);
    free_ast(root->right);
    free(root);
}

CalcResult calc(TokenStream *tokens) {
    ASTNode *ast = parse(tokens, 0);
    if (!ast)
        return (CalcResult){NoError, 0};
    CalcResult ret = eval_ast(ast);
    free_ast(ast);
    return ret;
}
