#include <stdio.h>
#include "calc.h"

int main(int argc, char **argv) {
    if (argc < 2)
        return 2;
    TokenStream *tokens = tokenize(argv[1]);
    if (!tokens)
        return 1;
    printf("%d\n", calc(tokens));
    free_tokenstream(tokens);
    return 0;
}
