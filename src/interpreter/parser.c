#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interpreter.h"

token_t * parser(size_t const n, char const expr[static restrict n]) {
    token_t * const output = calloc(n, sizeof *output);
    size_t tail = 0;
    token_t operators[n];
    memset(operators, 0, n * sizeof *operators);
    ptrdiff_t top = -1;
    lexin_t lexin = {(char *)expr, n, 0};

    for (token_t curr = lexer(&lexin); curr.type; curr = lexer(&lexin)) {
        switch (curr.type) {
            case NUMBER:
            case VARIABLE:
            case DERIVATIVE: {
                output[tail++] = curr;
                break;
            }

            case FUNCTION: {
                operators[++top] = curr;
                break;
            }

            case OPERATOR: {
                if (top > -1) {
                    token_t t = operators[top];
                    while ((t.type == FUNCTION || (t.type == OPERATOR && t.precedence >= curr.precedence)) && t.type != LBRACE) {
                        output[tail++] = operators[top--];
                        if (top < 0) break;
                        t = operators[top];
                    }
                }
                operators[++top] = curr;
                break;
            }

            case LBRACE: {
                operators[++top] = curr;
                break;
            }
                
            case RBRACE: {
                while (top > -1 && operators[top].type != LBRACE) {
                    output[tail++] = operators[top--];
                }
                if (top > -1 && operators[top].type == LBRACE) {
                    --top;
                } else {
                    fprintf(stderr, "Mismatched Parenthesis\n");
                    return 0;
                }
                break;
            }

            case EMPTY:
            default:
                ;
        }
    }

    while (top > -1) {
        output[tail++] = operators[top--];
    }

    return output;
}
