#include <stdbool.h>
#include <stddef.h>

typedef enum token_type_e token_type_e;
enum token_type_e {
    EMPTY = 0, NUMBER, VARIABLE, OPERATOR, FUNCTION, DERIVATIVE, BRACE
};

typedef struct operator_t operator_t;
struct operator_t {
    char ident;
    size_t precedence;
};

typedef struct derivative_t derivative_t;
struct derivative_t {
    char ident;
    size_t order;
};

typedef struct token_t token_t;
struct token_t {
    token_type_e type;
    union {
        float value;
        char ident;
        operator_t oper;
        char name[5];
        derivative_t derivative;
        bool open;
    };
};

token_t lexer(size_t const n, char const expr[static restrict n]);

token_t * parser(size_t const n, char const expr[static restrict n]);
