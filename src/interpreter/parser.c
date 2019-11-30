#include <stdlib.h>
#include <stdio.h>

#include "interpreter.h"

token_t * parser(size_t const n, char const expr[static restrict n]) {
    token_t * const output = malloc(n * sizeof *output);
    size_t tail = 0;
    token_t operators[n];
    size_t top = 0;

    for (token_t curr = lexer(n, expr); curr.type; curr = lexer(n, expr)) {
        switch (curr.type) {
            case NUMBER:
            case VARIABLE:
            case DERIVATIVE: {
                output[tail++] = curr;
                break;
            }

            case FUNCTION: {
                operators[top++] = curr;
                break;
            }

            case OPERATOR: {
                token_t t = operators[top];
                while ((t.type == FUNCTION
                        || (t.type == OPERATOR && t.oper.precedence >= curr.oper.precedence))
                        && !(t.type == BRACE && t.open)) {
                    output[tail++] = operators[top--];
                    t = operators[top];
                }
                operators[top++] = curr;
                break;
            }

            case BRACE:
                if (curr.open) {
                    operators[top++] = curr;
                } else {
                    while (operators[top].type != BRACE && !operators[top].open) {
                        output[tail++] = operators[top--];
                    }
                    if (operators[top].type == BRACE && operators[top].open) {
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

    return output;
}
