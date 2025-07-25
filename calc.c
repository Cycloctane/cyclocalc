#include "calc.h"
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

long long eval_ast(ASTNode *root) {
    if (!root)
        return 0;
    switch (root->type) {
    case Num:
        return root->val;
    case OpAdd:
        return eval_ast(root->left) + eval_ast(root->right);
    case OpSub:
        return eval_ast(root->left) - eval_ast(root->right);
    case OpMul:
        return eval_ast(root->left) * eval_ast(root->right);
    case OpDiv:
        return eval_ast(root->left) / eval_ast(root->right);
    case OpMod:
        return eval_ast(root->left) % eval_ast(root->right);
    case OpShiftLeft:
        return eval_ast(root->left) << eval_ast(root->right);
    case OpShiftRight:
        return eval_ast(root->left) >> eval_ast(root->right);
    case OpAnd:
        return eval_ast(root->left) & eval_ast(root->right);
    case OpXor:
        return eval_ast(root->left) ^ eval_ast(root->right);
    case OpOr:
        return eval_ast(root->left) | eval_ast(root->right);
    default:
        return 0;
    }
}

void free_ast(ASTNode *root) {
    if (!root)
        return;
    free_ast(root->left);
    free_ast(root->right);
    free(root);
}

long long calc(TokenStream *tokens) {
    ASTNode *ast = parse(tokens, 0);
    if (!ast)
        return 0;
    long long ret = eval_ast(ast);
    free_ast(ast);
    return ret;
}
