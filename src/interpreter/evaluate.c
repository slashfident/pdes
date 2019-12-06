#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <tgmath.h>

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
                    case ADD: {stack[++top] = b + a; break;}
                    case SUB: {stack[++top] = b - a; break;}
                    case MUL: {stack[++top] = b * a; break;}
                    case DIV: {stack[++top] = b / a; break;}
                    default: ;
                }
                break;
            }

            case FUNCTION: {
                switch (rpn[i].ident) {
                    case SIN: {stack[top] = sin(stack[top]); break;}
                    case COS: {stack[top] = cos(stack[top]); break;}
                    case EXP: {stack[top] = exp(stack[top]); break;}
                    case LOG: {stack[top] = log(stack[top]); break;}
                    default: ;
                }
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

