#include "lexer.h"
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static inline bool is_num(char *s) { return s && *s <= '9' && *s >= '0'; }

static int read_num(char **s_addr) {
    char *s = *s_addr, *base = *s_addr;
    while (is_num(s))
        s++;
    short n_digit = s - base;
    char n_str[n_digit + 1];
    memcpy(n_str, base, n_digit);
    n_str[n_digit] = 0;
    *s_addr = s;
    return atoi(n_str);
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
        while (*s_head == 0x20)
            s_head++;
        if (!*s_head)
            break;

        TokenType type;
        int val;
        if (expect_num) {
            if (*s_head == '(') {
                type = ParenLeft;
                paren_count++;
                s_head++;
            } else {
                type = Num;
                if (is_num(s_head))
                    val = read_num(&s_head);
                else if (*s_head == '+' && is_num(s_head + 1)) {
                    s_head++;
                    val = read_num(&s_head);
                } else if (*s_head == '-' && is_num(s_head + 1)) {
                    s_head++;
                    val = 0 - read_num(&s_head);
                } else {
                    fprintf(stderr, "Unexpected char '%c' at %ld\n", *s_head, s_head - s);
                    free_tokens(dummy.next);
                    return NULL;
                }
                expect_num = !expect_num;
            }
        } else {
            switch (*(s_head++)) {
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
            case ')':
                type = ParenRight;
                paren_count--;
                expect_num = !expect_num;
                break;
            default:
                fprintf(stderr, "Unexpected char '%c' at %ld\n", *(s_head - 1), s_head - 1 - s);
                free_tokens(dummy.next);
                return NULL;
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
