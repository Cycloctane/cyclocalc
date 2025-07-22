#include <stdio.h>
#include <string.h>
#include "calc.h"

#define BUF_SIZE 1024

static int eval(char *s) {
    TokenStream *tokens = tokenize(s);
    if (!tokens)
        return 1;
    printf("%d\n", calc(tokens));
    free_tokenstream(tokens);
    return 0;
}

int main(int argc, char **argv) {
    if (argc < 2) { // repl
        char buffer[BUF_SIZE];
        while (1) {
            fputs("(calc)> ", stdout);
            if (!fgets(buffer, BUF_SIZE, stdin))
                break;
            int len = strlen(buffer);
            if (!len || buffer[len - 1] != '\n'){
                fputs("Input line too long\n", stderr);
                return 1;
            }
            if (len == 1)
                continue;
            buffer[len - 1] = 0;
            eval(buffer);
        }
        return 0;
    } else if (argc == 2)
        return eval(argv[1]);
    else
        return 2;
}
