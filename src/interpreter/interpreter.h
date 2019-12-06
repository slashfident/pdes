#include <stdbool.h>
#include <stddef.h>

typedef enum token_type_e token_type_e;
enum token_type_e {
    EMPTY = 0, NUMBER, VARIABLE, OPERATOR, FUNCTION, DERIVATIVE, LBRACE, RBRACE
};

typedef enum identifier_e identifier_e;
enum identifier_e {
    C = 1, X, Y, Z, T, F, DX, DY, DZ, DT, ADD, SUB, MUL, DIV, SIN, COS, EXP, LOG
};

typedef struct token_t token_t;
struct token_t {
    token_type_e type;
    identifier_e ident;
    union {
        float value;
        size_t precedence;
        size_t order;
    };
};

typedef struct ast_node_t ast_t;
struct ast_node_t {
    token_t token;
    ast_t * lchild;
    ast_t * rchild;
};

token_t lexer(size_t const n, char const expr[static restrict n]);

token_t * parser(size_t const n, char const expr[static restrict n]);

float evaluate(size_t const n, token_t const rpn[static restrict n]);

