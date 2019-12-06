#include <stdio.h>
#include <string.h>

#include "interpreter.h"

void token_pretty_print(token_t t) {
    switch (t.type) {
        case NUMBER:
            printf("%.2f ", t.value);
            break;
        case VARIABLE:
            switch (t.ident) {
                case X: {printf("x "); break;}
                case Y: {printf("y "); break;}
                case Z: {printf("z "); break;}
                case T: {printf("t "); break;}
                default: ;
            }
            break;
        case OPERATOR:
            switch (t.ident) {
                case ADD: {printf("+ "); break;}
                case SUB: {printf("- "); break;}
                case MUL: {printf("* "); break;}
                case DIV: {printf("/ "); break;}
                default: ;
            }
            break;
        case FUNCTION:
            switch (t.ident) {
                case SIN: {printf("sin "); break;}
                case COS: {printf("cos "); break;}
                case EXP: {printf("exp "); break;}
                case LOG: {printf("log "); break;}
                default: ;
            }
            break;
        case DERIVATIVE:
            if (t.order > 0) {
                printf("d");
                if (t.order == 2) {printf("2");}
                switch (t.ident) {
                    case DX: {printf("x"); break;}
                    case DY: {printf("y"); break;}
                    case DZ: {printf("z"); break;}
                    case DT: {printf("t"); break;}
                    default: ;
                }
            }
            printf("f ");
            break;
        case LBRACE:
            printf("( ");
            break;
        case RBRACE:
            printf(") ");
            break;
        case EMPTY:
        default:
            ;
    }
}

int main(int argc, char ** argv) {
    char * input = "2*(2+3)";
    token_t * const output = parser(strlen(input), input);
    size_t n = 0;
    for (size_t i = 0; ; ++i) {
        if (output[i].type == EMPTY) break;
        ++n;
    }
    for (size_t i = 0; i < n; ++i) {
        token_pretty_print(output[i]);
    }
    printf("\n");
    float const result = evaluate(n, output);
    printf("%f\n", result);
    return 0;
}
