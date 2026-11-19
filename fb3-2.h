/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   fb3-2.h
 * Author: javiert
 *
 * Created on 3 de noviembre de 2016, 0:55
 */

/*
 * Declaraciones para una calculadora con funciones
 */

/* interfaz para el lexer */
extern int yylineno;
void yyerror(char *s, ...);

/* tabla de símbolos */
struct symbol{
    char *name;
    double value;
    struct ast *func;
    struct symlist *syms;
};

/* tabla de símbolos simple de tamaño fijo */
#define NHASH 9997
struct symbol symtab[NHASH];

struct symbol *lookup(char*);

/* lista de símbolos, para una lista de argumentos */
struct symlist{
    struct symbol *sym;
    struct symlist *next;
};

struct symlist *newsymlist(struct symbol *sym, struct symlist *next);
void symlistfree(struct symlist *sl);

/* tipos de nodos
 * + - * / |
 * 0-7 ops de comparación, codificados en bits 04 igual, 02 menor, 01 mayor
 * M menos unario
 * L expresión o lista de sentencias
 * I sentencia IF
 * W sentencia WHILE
 * N referencia a un símbolo
 * = asignación
 * S lista de símbolos
 * F llamada a una función incorporada
 * C llamada a una función de usuario
 */

enum bifs{              /* funciones incorporadas */
    B_sqrt = 1,
    B_exp,
    B_log,
    B_print
};

/* nodos en el árbol de sintaxis abstracta */
/* todos tienen un nodetype inicial en común */

struct ast{
    int nodetype;
    struct ast *l;
    struct ast *r;
};

struct fncall{
    int nodetype;
    struct ast *l;
    enum bifs functype;
};

struct ufncall{         /* funciones de usuario */
    int nodetype;
    struct ast *l;
    struct symbol *s;
};

struct flow{
    int nodetype;       /* tipo I o W */
    struct ast *cond;   /* condición */
    struct ast *tl;     /* rama "then" o lista "do" */
    struct ast *el;     /* rama opcional "else" */
};

struct numval{
    int nodetype;       /* tipo K */
    double number;
};

struct symref{
    int nodetype;       /* tipo N */
    struct symbol *s;
};

struct symasgn{
    int nodetype;       /* tipo = */
    struct symbol *s;
    struct ast *v;      /* valor */
};

/* construcción del AST */
struct ast *newast(int nodetype, struct ast *l, struct ast *r);
struct ast *newcmp(int cmptype, struct ast *l, struct ast *r);
struct ast *newfunc(int functype, struct ast *l);
struct ast *newcall(struct symbol *s, struct ast *l);
struct ast *newref(struct symbol *s);
struct ast *newasgn(struct symbol *s, struct ast *v);
struct ast *newnum(double d);
struct ast *newflow(int nodetype, struct ast *cond, struct ast *tl, struct ast *tr);

/* definir una función */
void dodef(struct symbol *name, struct symlist *syms, struct ast *stmts);

/* evaluar un AST */
double eval(struct ast *);

/* borrar y liberar un AST */
void treefree(struct ast *);


