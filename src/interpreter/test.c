#include <stdio.h>
#include <string.h>

#include "interpreter.h"

void token_pretty_print(token_t t) {
    switch (t.type) {
        case NUMBER:
            printf("%f ", t.value);
            break;
        case VARIABLE:
            printf("%c ", t.ident);
            break;
        case OPERATOR:
            printf("%c ", t.oper.ident);
            break;
        case FUNCTION:
            printf("%s ", t.name);
            break;
        case DERIVATIVE:
            printf("%c ", t.derivative.ident);
            break;
        case BRACE:
            printf("%c ", t.open ? '(' : ')');
            break;
        case EMPTY:
        default:
            ;
    }
}

int main(int argc, char ** argv) {
    char * input = "1+2-3*4/5";
    token_t * const output = parser(strlen(input), input);
    for (size_t i = 0; i < strlen(input); ++i) {
        token_pretty_print(output[i]);
    }
    return 0;
}
