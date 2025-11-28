#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

static inline bool is_num(char c) { return c <= '9' && c >= '0'; }

static int read_num(char **s_addr) {
    int num = 0;
    while (is_num(**s_addr))
        num = num * 10 + *(*s_addr)++ - '0';
    return num;
}

static void free_tokens(TokenNode *head) {
    TokenNode *free_ptr;
    while (head) {
        free_ptr = head;
        head = head->next;
        free(free_ptr);
    }
}

TokenStream *tokenize(char *s) {
    char *s_head = s;
    TokenNode dummy = {.next = NULL};
    TokenNode *node_head = &dummy;
    bool expect_num = true;
    int paren_count = 0;
    while (*s_head) {
        while (*s_head == ' ')
            ++s_head;
        if (!*s_head)
            break;

        TokenType type;
        int val = 0;
        if (*s_head == '(') {
            type = ParenLeft;
            ++paren_count;
            ++s_head;
        } else if (*s_head == ')') {
            if (paren_count == 0)
                goto error_char;
            type = ParenRight;
            --paren_count;
            ++s_head;
        } else if (expect_num) {
            type = Num;
            if (is_num(*s_head))
                val = read_num(&s_head);
            else if (*s_head == '+' && is_num(*(s_head + 1))) {
                ++s_head;
                val = read_num(&s_head);
            } else if (*s_head == '-' && is_num(*(s_head + 1))) {
                ++s_head;
                val = 0 - read_num(&s_head);
            } else
                goto error_char;
            expect_num = !expect_num;
        } else {
            switch (*s_head++) {
            case '+':
                type = OpAdd;
                break;
            case '-':
                type = OpSub;
                break;
            case '*':
                type = OpMul;
                break;
            case '/':
                type = OpDiv;
                break;
            case '%':
                type = OpMod;
                break;
            case '&':
                type = OpAnd;
                break;
            case '|':
                type = OpOr;
                break;
            case '^':
                type = OpXor;
                break;
            case '<':
                if (*s_head != '<')
                    goto error_char;
                ++s_head;
                type = OpShiftLeft;
                break;
            case '>':
                if (*s_head != '>')
                    goto error_char;
                ++s_head;
                type = OpShiftRight;
                break;
            default:
                --s_head;
                goto error_char;
            }
            expect_num = !expect_num;
        }
        TokenNode *new_node = malloc(sizeof(TokenNode));
        *new_node = (TokenNode){.type = type, .val = val, .next = NULL};
        node_head->next = new_node;
        node_head = node_head->next;
    }

    if (paren_count) {
        fputs("Unclosed parenthesis\n", stderr);
        free_tokens(dummy.next);
        return NULL;
    }
    TokenStream *tokens = malloc(sizeof(TokenStream));
    *tokens = (TokenStream){.head = dummy.next, .pos = dummy.next};
    return tokens;

error_char:
    fprintf(stderr, "Unexpected char '%c' at %ld\n", *s_head, s_head - s);
    free_tokens(dummy.next);
    return NULL;
}

void free_tokenstream(TokenStream *tokens) {
    free_tokens(tokens->head);
    free(tokens);
}

TokenNode *next_token(TokenStream *tokens) {
    TokenNode *ret = tokens->pos;
    if (tokens->pos)
        tokens->pos = tokens->pos->next;
    return ret;
}

TokenNode *peek_token(TokenStream *tokens) { return tokens->pos; }
