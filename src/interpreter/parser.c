#include <stdlib.h>
#include <stdio.h>
#include <string.h>

#include "interpreter.h"

token_t * parser(size_t const n, char const expr[static restrict n]) {
    token_t * const output = malloc(n * sizeof *output);
    memset(output, 0, n * sizeof *output);
    size_t tail = 0;
    token_t operators[n];
    memset(operators, 0, n * sizeof *operators);
    ptrdiff_t top = -1;

    for (token_t curr = lexer(n, expr); curr.type; curr = lexer(n, expr)) {
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
                    while ((t.type == FUNCTION || (t.type == OPERATOR && t.oper.precedence >= curr.oper.precedence)) && !(t.type == BRACE && t.open)) {
                        output[tail++] = operators[top--];
                        if (top < 0) break;
                        t = operators[top];
                    }
                }
                operators[++top] = curr;
                break;
            }

            case BRACE:
                if (curr.open) {
                    operators[++top] = curr;
                } else {
                    while (top > -1 && operators[top].type != BRACE && !operators[top].open) {
                        output[tail++] = operators[top--];
                    }
                    if (top > -1 && operators[top].type == BRACE && operators[top].open) {
                        --top;
                    } else {
                        fprintf(stderr, "Mismatched Parenthesis\n");
                        return 0;
                    }
                }
                break;

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
