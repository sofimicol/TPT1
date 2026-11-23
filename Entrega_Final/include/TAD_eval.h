#ifndef TAD_EVAL_H
#define TAD_EVAL_H

#include <stdio.h>
#include <stdlib.h>
#include "TAD_ast.h"
#include "TAD_symtab.h"

typedef enum {
    AST_LITERAL,      /* Literal elemental: 'a', '0', etc. */
    AST_EMPTY,        /* Constante de coleccion vacia: empty */
    AST_VAR,          /* Referencia a variable (r-value): ID */
    AST_SET_LIT,      /* Construccion de conjunto: { ... } */
    AST_LIST_LIT,     /* Construccion de lista: [ ... ] */
    AST_UNION,        /* Operacion binaria: expr union term */
    AST_INTER,        /* Operacion binaria: expr inter term */
    AST_DIFF,         /* Operacion binaria: expr diff term */
    AST_INSERT,       /* Operacion binaria: expr insert term */
    AST_APPEND,       /* Operacion binaria: expr append term */
    AST_ASSIGN,       /* Asignacion simple: ID = expr */
    AST_MULTI_ASSIGN, /* Asignacion multiple: let ID1, ID2 = expr1, expr2 */
    AST_EXPR_LIST,    /* Lista enlazada de subexpresiones */
    AST_ID_LIST       /* Lista enlazada de identificadores */
} TipoNodoAST;

typedef struct nodoAST {
    TipoNodoAST tipo;
    str nombre;             /* Identificador para AST_VAR, AST_ASSIGN y AST_ID_LIST */
    Tdata literal;          /* Dato elemental para AST_LITERAL */
    struct nodoAST *izq;    /* Operando izquierdo / lista de IDs / elementos */
    struct nodoAST *der;    /* Operando derecho / lista de expresiones / siguiente */
} TAst;

/* Constructores de nodos del AST */
TAst* crear_nodo_literal(char c);
TAst* crear_nodo_empty(void);
TAst* crear_nodo_var(str nombre);
TAst* crear_nodo_coleccion(TipoNodoAST tipo, TAst *elementos);
TAst* crear_nodo_op(TipoNodoAST tipo, TAst *izq, TAst *der);
TAst* crear_nodo_asig(str nombre, TAst *expr);
TAst* crear_nodo_asig_mult(TAst *ids, TAst *exprs);
TAst* crear_nodo_lista_expr(TAst *expr, TAst *siguiente);
TAst* crear_nodo_lista_id(str nombre, TAst *siguiente);
TAst* agregar_al_final_ast(TAst *lista, TAst *nuevo);

/* Motor de evaluacion (Tree-Walk Evaluator) */
Tdata eval_ast(TAst *nodo, TTablaSimbolos *tabla);

/* Liberacion completa de memoria del AST */
void liberar_ast(TAst *nodo);

#endif