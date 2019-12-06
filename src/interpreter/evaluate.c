#include <string.h>
#include <stdio.h>
#include <stdlib.h>

#include "interpreter.h"

float evaluate(size_t const n, token_t const rpn[static restrict n]) {
    float stack[n];
    memset(stack, 0, sizeof *stack * n);
    ptrdiff_t top = -1;

    for (size_t i = 0; i < n; ++i) {
        switch (rpn[i].type) {
            case NUMBER: {
                stack[++top] = rpn[i].value;
                break;
            }

            case OPERATOR: {
                float a = stack[top--];
                float b = stack[top--];
                switch (rpn[i].ident) {
                    case ADD: {stack[++top] = a + b; break;}
                    case SUB: {stack[++top] = a - b; break;}
                    case MUL: {stack[++top] = a * b; break;}
                    case DIV: {stack[++top] = a / b; break;}
                    default: ;
                }
                break;
            }
                
            default:
                ;
        }
    }

    if (top != 0) {
        fprintf(stderr, "Invalid rpn expression.\n");
        exit(-1);
    }

    return stack[top];
}

