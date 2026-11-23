#ifndef TAD_SYMTAB_H
#define TAD_SYMTAB_H

#include <stdio.h>
#include <stdlib.h>
#include "TAD_ast.h"

/* Registro individual de una variable en la Tabla de Simbolos */
typedef struct simbolo {
    str nombre;             /* Identificador */
    int tipo;               /* Tipo: STR (1), SET (2), LIST (3) */
    Tdata valor;            /* Puntero al arbol de datos (TAD_ast) */
    struct simbolo *next;   /* Enlace al siguiente registro de la tabla */
} TSimbolo;

typedef TSimbolo* TTablaSimbolos;

/* Busca una variable por nombre; retorna su registro o NULL si no existe */
TSimbolo* buscar_simbolo(TTablaSimbolos tabla, str nombre);

/* Inserta una variable nueva o actualiza el valor y tipo si ya existia */
TSimbolo* insertar_simbolo(TTablaSimbolos *tabla, str nombre, Tdata valor);

/* Muestra por consola todas las variables almacenadas actualmente */
void mostrar_tabla_simbolos(TTablaSimbolos tabla);

/* Libera toda la memoria reservada por la tabla y sus valores */
void liberar_tabla_simbolos(TTablaSimbolos *tabla);

#endif