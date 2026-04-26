#ifndef TAD_AST_H
#define TAD_AST_H

#include <stdio.h>
#include <stdlib.h>
#include "TAD_str.h"

#define STR 1
#define SET 2
#define LIST 3


struct dataType{
	int nodeType;   /* 1 STR, 2 SET, 3 LIST */
	union{
		str string; 
		struct{
			struct dataType* data; // puntero a una estructura que contiene un dato
			struct dataType* next; // puntero a siguiente
		};
	};
};
typedef struct dataType* Tdata; // Tdata es un Puntero a un arbol
Tdata create_str();
Tdata create_set();
Tdata prod_cartesiano(Tdata a, Tdata b);
Tdata cargar_conjunto();
Tdata cargar_lista();
Tdata create_list();
#endif
