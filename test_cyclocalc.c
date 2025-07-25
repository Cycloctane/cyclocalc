#include "calc.h"
#include <assert.h>

static void test_tokenize_fail(char *s) {
    TokenStream *tokens = tokenize(s);
    assert(!tokens);
}

static void test_eval(char *s, long long val) {
    TokenStream *tokens = tokenize(s);
    assert(tokens);
    long long ret = calc(tokens);
    free_tokenstream(tokens);
    assert(ret == val);
}

int main(){
    test_eval("", 0);
    test_eval("9223372036854775807", 9223372036854775807);
    test_eval("-9223372036854775808", -9223372036854775807 - 1);
    test_eval("1", 1);
    test_eval(" -42 ", -42);
    test_eval("+42", 42);
    test_eval("(100)", 100);

    test_eval("1 << 4", 16);
    test_eval("32 >> 5", 1);
    test_eval("1 & 2", 0);
    test_eval("2 | 2", 2);
    test_eval("2 ^ 3", 1);

    test_eval("1 - 2 * 3", -5);
    test_eval("3 * (2 + 4)", 18);
    test_eval("3 | 2 & 1", 3);
    test_eval("6 & 1 << 2", 4);
    test_eval("1 + 2 ^ 5", 6);
    test_eval("( 1 + (4 + 5 + 2) - 3) + (6 + 8)", 23);
    test_eval("2 * (3-2 - (1 - 2))+ 1", 5);
    test_eval("1 + 3 % 2 - -4 * 5 + 7 / 6", 23);
    test_eval("2 + 2 * 3 << 1 & 14 | 8 ^ 2", 10);

    test_tokenize_fail("3 * (2 + 4 ");
    test_tokenize_fail("3 * 2 + 4)");
    test_tokenize_fail("3 * )2 + 4");
    test_tokenize_fail("100 --");
    test_tokenize_fail("1 < 2");
    test_tokenize_fail("@");
}
