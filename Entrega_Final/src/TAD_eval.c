#include "TAD_eval.h"
#include "TAD_set.h"

/* ---------------- FUNCIONES AUXILIARES PRIVADAS ---------------- */

static TAst* reservar_nodo_ast(TipoNodoAST tipo) {
    TAst *n = (TAst*)calloc(1, sizeof(TAst));
    if (n == NULL) {
        fprintf(stderr, "Error critico: sin memoria para el AST.\n");
        exit(EXIT_FAILURE);
    }
    n->tipo = tipo;
    return n;
}

static int contar_lista_ast(TAst *lista) {
    int c = 0;
    while (lista != NULL) {
        c++;
        lista = lista->der;
    }
    return c;
}

static void imprimir_str_stderr(str cad) {
    str aux = cad;
    while (aux != NULL) {
        fprintf(stderr, "%c", aux->data);
        aux = aux->next;
    }
}

/* ---------------- CONSTRUCTORES DEL AST ---------------- */

TAst* crear_nodo_literal(char c) {
    TAst *n = reservar_nodo_ast(AST_LITERAL);
    n->literal = create_str();
    n->literal->string = create_nodo(c);
    return n;
}

TAst* crear_nodo_empty(void) {
    return reservar_nodo_ast(AST_EMPTY);
}

TAst* crear_nodo_var(str nombre) {
    TAst *n = reservar_nodo_ast(AST_VAR);
    n->nombre = nombre;
    return n;
}

TAst* crear_nodo_coleccion(TipoNodoAST tipo, TAst *elementos) {
    TAst *n = reservar_nodo_ast(tipo);
    n->izq = elementos;
    return n;
}

TAst* crear_nodo_op(TipoNodoAST tipo, TAst *izq, TAst *der) {
    TAst *n = reservar_nodo_ast(tipo);
    n->izq = izq;
    n->der = der;
    return n;
}

TAst* crear_nodo_asig(str nombre, TAst *expr) {
    TAst *n = reservar_nodo_ast(AST_ASSIGN);
    n->nombre = nombre;
    n->der = expr;
    return n;
}

TAst* crear_nodo_asig_mult(TAst *ids, TAst *exprs) {
    TAst *n = reservar_nodo_ast(AST_MULTI_ASSIGN);
    n->izq = ids;
    n->der = exprs;
    return n;
}

TAst* crear_nodo_lista_expr(TAst *expr, TAst *siguiente) {
    TAst *n = reservar_nodo_ast(AST_EXPR_LIST);
    n->izq = expr;
    n->der = siguiente;
    return n;
}

TAst* crear_nodo_lista_id(str nombre, TAst *siguiente) {
    TAst *n = reservar_nodo_ast(AST_ID_LIST);
    n->nombre = nombre;
    n->der = siguiente;
    return n;
}

TAst* agregar_al_final_ast(TAst *lista, TAst *nuevo) {
    if (lista == NULL) {
        return nuevo;
    }
    TAst *aux = lista;
    while (aux->der != NULL) {
        aux = aux->der;
    }
    aux->der = nuevo;
    return lista;
}

/* ---------------- EVALUADOR DEL AST ---------------- */

Tdata eval_ast(TAst *nodo, TTablaSimbolos *tabla) {
    if (nodo == NULL) {
        return NULL;
    }

    switch (nodo->tipo) {
        case AST_LITERAL:
            return copy_ast(nodo->literal);

        case AST_EMPTY:
            return create_set();

        case AST_VAR: {
            TSimbolo *sim = buscar_simbolo(*tabla, nodo->nombre);
            if (sim == NULL) {
                fprintf(stderr, "Error semantico: variable '");
                imprimir_str_stderr(nodo->nombre);
                fprintf(stderr, "' no declarada.\n");
                return NULL;
            }
            return copy_ast(sim->valor);
        }

        case AST_SET_LIT: {
            Tdata set = create_set();
            TAst *it = nodo->izq;
            while (it != NULL) {
                Tdata elem = eval_ast(it->izq, tabla);
                if (elem == NULL) {
                    free_ast(set);
                    return NULL;
                }
                insert_set(&set, elem);
                free_ast(elem);
                it = it->der;
            }
            return set;
        }

        case AST_LIST_LIT: {
            Tdata list = create_list();
            TAst *it = nodo->izq;
            while (it != NULL) {
                Tdata elem = eval_ast(it->izq, tabla);
                if (elem == NULL) {
                    free_ast(list);
                    return NULL;
                }
                append(&list, elem);
                free_ast(elem);
                it = it->der;
            }
            return list;
        }

        case AST_UNION:
        case AST_INTER:
        case AST_DIFF: {
            Tdata izq = eval_ast(nodo->izq, tabla);
            Tdata der = eval_ast(nodo->der, tabla);
            Tdata res = NULL;

            if (izq != NULL && der != NULL) {
                if (return_type(izq) == SET && return_type(der) == SET) {
                    if (nodo->tipo == AST_UNION)      res = unionset(izq, der);
                    else if (nodo->tipo == AST_INTER) res = intersection_set(izq, der);
                    else                              res = difference_set(izq, der);
                } else {
                    fprintf(stderr, "Error semantico de tipo: los operadores conjuntistas requieren dos conjuntos (SET).\n");
                }
            }
            free_ast(izq);
            free_ast(der);
            return res;
        }

        case AST_INSERT: {
            Tdata izq = eval_ast(nodo->izq, tabla);
            Tdata der = eval_ast(nodo->der, tabla);
            Tdata res = NULL;

            if (izq != NULL && der != NULL) {
                if (return_type(izq) == SET) {
                    res = copy_ast(izq);
                    insert_set(&res, der);
                } else {
                    fprintf(stderr, "Error semantico de tipo: 'insert' requiere un conjunto (SET) como operando izquierdo.\n");
                }
            }
            free_ast(izq);
            free_ast(der);
            return res;
        }

        case AST_APPEND: {
            Tdata izq = eval_ast(nodo->izq, tabla);
            Tdata der = eval_ast(nodo->der, tabla);
            Tdata res = NULL;

            if (izq != NULL && der != NULL) {
                if (return_type(izq) == LIST) {
                    res = copy_ast(izq);
                    append(&res, der);
                } else {
                    fprintf(stderr, "Error semantico de tipo: 'append' requiere una lista (LIST) como operando izquierdo.\n");
                }
            }
            free_ast(izq);
            free_ast(der);
            return res;
        }

        case AST_ASSIGN: {
            Tdata val = eval_ast(nodo->der, tabla);
            if (val == NULL) {
                return NULL;
            }
            insertar_simbolo(tabla, nodo->nombre, val);
            return val;
        }

        case AST_MULTI_ASSIGN: {
            int n_ids = contar_lista_ast(nodo->izq);
            int n_exprs = contar_lista_ast(nodo->der);

            if (n_ids != n_exprs) {
                fprintf(stderr, "Error semantico: la asignacion multiple tiene %d variables y %d expresiones.\n",
                        n_ids, n_exprs);
                return NULL;
            }

            Tdata *valores = (Tdata*)calloc(n_exprs, sizeof(Tdata));
            TAst *it_e = nodo->der;
            int error = 0;
            for (int i = 0; i < n_exprs; i++) {
                valores[i] = eval_ast(it_e->izq, tabla);
                if (valores[i] == NULL) {
                    error = 1;
                }
                it_e = it_e->der;
            }

            Tdata resumen = NULL;
            if (!error) {
                resumen = create_list();
                TAst *it_id = nodo->izq;
                for (int i = 0; i < n_ids; i++) {
                    insertar_simbolo(tabla, it_id->nombre, valores[i]);
                    append(&resumen, valores[i]);
                    it_id = it_id->der;
                }
            }

            for (int i = 0; i < n_exprs; i++) {
                free_ast(valores[i]);
            }
            free(valores);
            return resumen;
        }

        default:
            fprintf(stderr, "Error interno: tipo de nodo AST desconocido (%d).\n", nodo->tipo);
            return NULL;
    }
}

/* ---------------- LIBERACION DEL AST ---------------- */

void liberar_ast(TAst *nodo) {
    if (nodo == NULL) {
        return;
    }
    liberar_ast(nodo->izq);
    liberar_ast(nodo->der);
    if (nodo->nombre != NULL) {
        free_str(nodo->nombre);
    }
    if (nodo->literal != NULL) {
        free_ast(nodo->literal);
    }
    free(nodo);
}