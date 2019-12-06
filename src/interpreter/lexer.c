#include <ctype.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "interpreter.h"

identifier_e ismathfun(size_t const n, char const str[static restrict n]) {
    if (!strncmp(str, "sin", 3)) {return SIN;}
    if (!strncmp(str, "cos", 3)) {return COS;}
    if (!strncmp(str, "exp", 3)) {return EXP;}
    if (!strncmp(str, "log", 3)) {return LOG;}
    return 0;
}

identifier_e isvar(size_t const n, char const str[static restrict n]) {
    if (str[0] == 'x') {return X;}
    if (str[0] == 'y') {return Y;}
    if (str[0] == 'z') {return Z;}
    if (str[0] == 't') {return T;}
    return 0;
}

size_t isderivative(size_t const n, char const str[static restrict n]) {
    if (str[0] == 'f') {return 1;}
    if (str[0] == 'd' && isvar(n - 1, str + 1)) {
        return 1 + isderivative(n - 2, str + 2);
    }
    return 0;
}

identifier_e derivativeident(size_t const n, char const str[static restrict n]) {
    if (str[0] == 'f') {return F;}
    switch (str[1]) {
        case 'x': {return DX;}
        case 'y': {return DY;}
        case 'z': {return DZ;}
        case 't': {return DT;}
        default:
            ;
    }
    return 0;
}

size_t isoperator(char const c) {
    if (c == '+') {return 5;}
    if (c == '-') {return 5;}
    if (c == '*') {return 10;}
    if (c == '/') {return 10;}
    return 0;
}

identifier_e operatorident(char const c) {
    if (c == '+') {return ADD;}
    if (c == '-') {return SUB;}
    if (c == '*') {return MUL;}
    if (c == '/') {return DIV;}
    return 0;
}

/* expects to always be called with the same arguments */
token_t lexer(size_t const n, char const expr[static restrict n]) {
    size_t static curr = 0;
    char static buf[32] = {0};
    size_t static bufcurr = 0;

    if (curr == n) return (token_t){0};

    if (isspace(expr[curr])) {
        ++curr;
        return lexer(n, expr);
    }

    else if (isalpha(expr[curr])) {
        do {
            buf[bufcurr++] = expr[curr++];
            if (curr == n) break;
        } while (isalpha(expr[curr]));

        if (isvar(32, buf)) {
            token_t token;
            token.type = VARIABLE;
            token.ident = isvar(32, buf);

            memset(buf, 0, 32);
            bufcurr = 0;

            return token;
        }

        if (ismathfun(32, buf)) {
            token_t token;
            token.type = FUNCTION;
            token.ident = ismathfun(32, buf);

            memset(buf, 0, 32);
            bufcurr = 0;

            return token;
        }

        if (isderivative(32, buf)) {
            token_t token;
            token.type = DERIVATIVE;
            token.ident = derivativeident(32, buf);
            token.order = isderivative(32, buf);

            memset(buf, 0, 32);
            bufcurr = 0;

            return token;
        }
    }

    else if (isdigit(expr[curr])) {
        do {
            buf[bufcurr++] = expr[curr++];
            if (curr == n) break;
        } while (isdigit(expr[curr]) || expr[curr] == '.');

        token_t token;
        token.type = NUMBER;
        token.ident = C;
        token.value = strtof(buf, 0);

        memset(buf, 0, 32);
        bufcurr = 0;

        return token;
    }

    else if (isoperator(expr[curr])) {
        token_t token;
        token.type = OPERATOR;
        token.ident = operatorident(expr[curr]);
        token.precedence = isoperator(expr[curr]);

        ++curr;
        return token;
    }

    else if (expr[curr] == '(') {
        token_t token;
        token.type = LBRACE;
        token.ident = 0;
        token.order = 0;

        ++curr;
        return token;
    } else if (expr[curr] == ')') {
        token_t token;
        token.type = RBRACE;
        token.ident = 0;
        token.order = 0;

        ++curr;
        return token;
    }

    fprintf(stderr, "Could not lex the expression.\n");
    return (token_t){0};
}
