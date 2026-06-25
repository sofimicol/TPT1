#ifndef TAD_AST_H
#define TAD_AST_H
#include <stdio.h>
#include <stdlib.h>
#include "TAD_str.h"
#define STR 1
#define SET 2
#define LIST 3

typedef struct dataType{
	int nodeType;   /* 1 STR, 2 SET, 3 LIST */
	union{
		str string; 
		struct{
			struct dataType* data; // puntero a una estructura que contiene un dato
			struct dataType* next; // puntero a siguiente
		};
	};
}TNodo;
typedef TNodo*Tdata; // Tdata es un Puntero a un arbol
Tdata create_str();
Tdata prod_cartesiano(Tdata a, Tdata b);
Tdata carga_lista();
Tdata create_list();
Tdata create_set();
void mostrarArbol(Tdata arbol);
Tdata prod_cartesiano(Tdata a, Tdata b);
Tdata copy_ast(Tdata);
int esvacio(Tdata);
void eliminarprimero(Tdata* set);
void append(Tdata* list, Tdata elem);
int equals_node(Tdata n1, Tdata n2);
int return_type(Tdata a);
str indice_a_str(Tdata set, int indice);
str obtener_string(Tdata nodo);
void free_ast(Tdata ast);
str indice_a_str(Tdata set, int indice);
Tdata obtener_data(Tdata nodo);
Tdata obtener_next(Tdata nodo);
Tdata cargar_conjunto();
void insert_set(Tdata* set, Tdata elem);
#endif
