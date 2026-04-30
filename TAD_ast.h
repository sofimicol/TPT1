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

 // Prototipos
Tdata create_str();
Tdata create_set();
Tdata carga_lista();
void mostrarArbol(Tdata ast);
Tdata copy_ast(Tdata ast);
//void append(Tdata* Lista, Tdata elemento);
int esvacio(Tdata);
void eliminarprimero(Tdata* ast);
void free_ast(Tdata ast);
int equals_node(Tdata nodo, Tdata nodo);
Tdata prod_cartesiano(Tdata ast, Tdata ast);

#endif
