#ifndef TAD_AST_H
#define TAD_AST_H

#include <stdio.h>
#include <stdlib.h>
#include "TAD_str.h"

#define STR 1
#define SET 2
#define LIST 3

typedef struct dataType{
	int nodeType;    1 STR, 2 SET, 3 LIST /
	union{
		str string; 
		struct{
			struct dataType data; // puntero a una estructura que contiene un dato
			struct dataType* next; // puntero a siguiente
		};
	};
}TNodo;
typedef TNodoTdata; // Tdata es un Puntero a un arbol

 // Funciones de creación
Tdata create_str();
Tdata create_set();
Tdata carga_lista();
 // Funciones de eliminación
void eliminarprimero(Tdata* ast);
void free_ast(Tdata ast);
 // Funciones de muestra
void mostrarArbol(Tdata ast);
 // Funciones propias
Tdata copy_ast(Tdata ast);
int esvacio(Tdata);
int equals_node(Tdata nodo, Tdata nodo);
 // Operacione algebraica
Tdata prod_cartesiano(Tdata ast, Tdata ast);
//void append(Tdata* Lista, Tdata elemento);

#endif
