#include "TAD_symtab.h"

TSimbolo* buscar_simbolo(TTablaSimbolos tabla, str nombre) {
    if (nombre == NULL) {
        return NULL;
    }

    TSimbolo *aux = tabla;
    while (aux != NULL) {
        if (compare_str(aux->nombre, nombre) == 1) {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

TSimbolo* insertar_simbolo(TTablaSimbolos *tabla, str nombre, Tdata valor) {
    if (tabla == NULL || nombre == NULL) {
        return NULL;
    }

    TSimbolo *existente = buscar_simbolo(*tabla, nombre);
    if (existente != NULL) {
        /* Actualizacion de variable existente: liberamos el valor anterior */
        free_ast(existente->valor);
        existente->valor = copy_ast(valor);
        existente->tipo = return_type(valor);
        return existente;
    }

    /* Creacion de una nueva entrada en la tabla */
    TSimbolo *nuevo = (TSimbolo*)malloc(sizeof(TSimbolo));
    if (nuevo == NULL) {
        fprintf(stderr, "Error critico: sin memoria para la Tabla de Simbolos.\n");
        exit(EXIT_FAILURE);
    }

    nuevo->nombre = copy_str(nombre);
    nuevo->valor = copy_ast(valor);
    nuevo->tipo = return_type(valor);
    nuevo->next = *tabla;
    *tabla = nuevo;
    return nuevo;
}

static const char* nombre_tipo(int tipo) {
    switch (tipo) {
        case STR:  return "STR";
        case SET:  return "SET";
        case LIST: return "LIST";
        default:   return "DESCONOCIDO";
    }
}

void mostrar_tabla_simbolos(TTablaSimbolos tabla) {
    printf("\n================ TABLA DE SIMBOLOS ================\n");
    if (tabla == NULL) {
        printf("  (vacia)\n");
        printf("===================================================\n");
        return;
    }

    TSimbolo *aux = tabla;
    while (aux != NULL) {
        printf("  ");
        print(aux->nombre);
        printf("  |  Tipo: %-4s  |  Valor: ", nombre_tipo(aux->tipo));
        mostrarArbol(aux->valor);
        printf("\n");
        aux = aux->next;
    }
    printf("===================================================\n");
}

void liberar_tabla_simbolos(TTablaSimbolos *tabla) {
    if (tabla == NULL) {
        return;
    }

    TSimbolo *aux = *tabla;
    while (aux != NULL) {
        TSimbolo *sig = aux->next;
        free_str(aux->nombre);
        free_ast(aux->valor);
        free(aux);
        aux = sig;
    }
    *tabla = NULL;
}