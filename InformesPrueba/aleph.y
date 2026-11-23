%{
#include <stdio.h>
#include <stdlib.h>
#include "TAD_eval.h"
#include "TAD_symtab.h"

extern int yylex(void);
extern int yylineno;
extern FILE *yyin;

void yyerror(const char *s);

/* Tabla de Simbolos global del interprete */
static TTablaSimbolos tabla_global = NULL;

static int es_operacion_binaria(TipoNodoAST tipo) {
    return (tipo == AST_UNION || tipo == AST_INTER || tipo == AST_DIFF ||
            tipo == AST_INSERT || tipo == AST_APPEND);
}

static void mostrar_ast_expr(TAst *n) {
    if (n == NULL) return;
    switch (n->tipo) {
        case AST_LITERAL:
            if (n->literal && n->literal->string) {
                printf("'%c'", n->literal->string->data);
            } else {
                printf("''");
            }
            break;
        case AST_EMPTY:
            printf("empty");
            break;
        case AST_VAR:
            print(n->nombre);
            break;
        case AST_SET_LIT:
            printf("{");
            {
                TAst *it = n->izq;
                while (it != NULL) {
                    mostrar_ast_expr(it->izq);
                    if (it->der != NULL) printf(", ");
                    it = it->der;
                }
            }
            printf("}");
            break;
        case AST_LIST_LIT:
            printf("[");
            {
                TAst *it = n->izq;
                while (it != NULL) {
                    mostrar_ast_expr(it->izq);
                    if (it->der != NULL) printf(", ");
                    it = it->der;
                }
            }
            printf("]");
            break;
        case AST_UNION:
        case AST_INTER:
        case AST_DIFF:
        case AST_INSERT:
        case AST_APPEND: {
            const char *op_str = "";
            switch (n->tipo) {
                case AST_UNION:  op_str = "union"; break;
                case AST_INTER:  op_str = "inter"; break;
                case AST_DIFF:   op_str = "diff"; break;
                case AST_INSERT: op_str = "insert"; break;
                case AST_APPEND: op_str = "append"; break;
                default: break;
            }
            int par_izq = (n->izq && es_operacion_binaria(n->izq->tipo));
            int par_der = (n->der && es_operacion_binaria(n->der->tipo));

            if (par_izq) printf("(");
            mostrar_ast_expr(n->izq);
            if (par_izq) printf(")");

            printf(" %s ", op_str);

            if (par_der) printf("(");
            mostrar_ast_expr(n->der);
            if (par_der) printf(")");
            break;
        }
        default:
            break;
    }
}

static void ejecutar_sentencia(TAst *arbol) {
    Tdata resultado = eval_ast(arbol, &tabla_global);
    if (resultado != NULL) {
        printf("  ");
        if (arbol->tipo == AST_ASSIGN) {
            print(arbol->nombre);
            printf(" = ");
            mostrar_ast_expr(arbol->der);
            printf("  =>  ");
        } else if (arbol->tipo == AST_MULTI_ASSIGN) {
            printf("let ");
            TAst *aux_id = arbol->izq;
            while (aux_id != NULL) {
                print(aux_id->nombre);
                if (aux_id->der != NULL) printf(", ");
                aux_id = aux_id->der;
            }
            printf(" = ");
            TAst *aux_e = arbol->der;
            while (aux_e != NULL) {
                mostrar_ast_expr(aux_e->izq);
                if (aux_e->der != NULL) printf(", ");
                aux_e = aux_e->der;
            }
            printf("  =>  ");
        } else {
            mostrar_ast_expr(arbol);
            printf("  =>  ");
        }
        mostrarArbol(resultado);
        printf("\n");
        free_ast(resultado);
    }
    liberar_ast(arbol);
}
%}

%union {
    char caracter;
    str cadena;
    TAst *ast;
}

%token <caracter> LIT_ELEMENT
%token <cadena> ID
%token KW_SET KW_LIST KW_LET KW_EMPTY
%token OP_UNION OP_INTERSECTION OP_DIFFERENCE KW_APPEND KW_INSERT

%type <ast> sentencia asignacion lts_id lts_expresiones
%type <ast> lts_conjunto lts_lista expresion termino

/* Asociatividad por izquierda y precedencia (inter sobre union/diff/append/insert) */
%left OP_UNION OP_DIFFERENCE KW_APPEND KW_INSERT
%left OP_INTERSECTION

%start programa

%%

programa: /* vacio */
        | programa sentencia ';' {
              ejecutar_sentencia($2);
          }
        | programa error ';' {
              yyerrok;
          }
        ;

sentencia: asignacion { $$ = $1; }
         | expresion  { $$ = $1; }
         ;

asignacion: ID '=' expresion {
                $$ = crear_nodo_asig($1, $3);
            }
          | KW_LET lts_id '=' lts_expresiones {
                $$ = crear_nodo_asig_mult($2, $4);
            }
          ;

lts_id: ID {
            $$ = crear_nodo_lista_id($1, NULL);
        }
      | lts_id ',' ID {
            $$ = agregar_al_final_ast($1, crear_nodo_lista_id($3, NULL));
        }
      ;

lts_expresiones: expresion {
                     $$ = crear_nodo_lista_expr($1, NULL);
                 }
               | lts_expresiones ',' expresion {
                     $$ = agregar_al_final_ast($1, crear_nodo_lista_expr($3, NULL));
                 }
               ;

lts_conjunto: '{' lts_expresiones '}' {
                  $$ = crear_nodo_coleccion(AST_SET_LIT, $2);
              }
            | '{' '}' {
                  $$ = crear_nodo_coleccion(AST_SET_LIT, NULL);
              }
            ;

lts_lista: '[' lts_expresiones ']' {
               $$ = crear_nodo_coleccion(AST_LIST_LIT, $2);
           }
         | '[' ']' {
               $$ = crear_nodo_coleccion(AST_LIST_LIT, NULL);
           }
         ;

expresion: termino {
               $$ = $1;
           }
         | expresion OP_UNION expresion {
               $$ = crear_nodo_op(AST_UNION, $1, $3);
           }
         | expresion OP_INTERSECTION expresion {
               $$ = crear_nodo_op(AST_INTER, $1, $3);
           }
         | expresion OP_DIFFERENCE expresion {
               $$ = crear_nodo_op(AST_DIFF, $1, $3);
           }
         | expresion KW_APPEND expresion {
               $$ = crear_nodo_op(AST_APPEND, $1, $3);
           }
         | expresion KW_INSERT expresion {
               $$ = crear_nodo_op(AST_INSERT, $1, $3);
           }
         ;

termino: ID {
             $$ = crear_nodo_var($1);
         }
       | LIT_ELEMENT {
             $$ = crear_nodo_literal($1);
         }
       | KW_EMPTY {
             $$ = crear_nodo_empty();
         }
       | lts_lista {
             $$ = $1;
         }
       | lts_conjunto {
             $$ = $1;
         }
       | '(' expresion ')' {
             $$ = $2;
         }
       ;

%%

void yyerror(const char *s) {
    fprintf(stderr, "Linea %d - Error sintactico: %s\n", yylineno, s);
}

int main(int argc, char **argv) {
    if (argc > 1) {
        FILE *archivo = fopen(argv[1], "r");
        if (!archivo) {
            fprintf(stderr, "Error: no se pudo abrir el archivo '%s'\n", argv[1]);
            return 1;
        }
        yyin = archivo;
    } else {
        printf("=========================================================\n");
        printf(" INTERPRETE ALEPH - Entrega 5\n");
        printf(" Ingrese sentencias terminadas en ';' (Ctrl+D / Ctrl+Z para salir)\n");
        printf("=========================================================\n\n");
    }

    yyparse();

    mostrar_tabla_simbolos(tabla_global);
    liberar_tabla_simbolos(&tabla_global);

    if (argc > 1 && yyin != NULL) {
        fclose(yyin);
    }
    return 0;
}