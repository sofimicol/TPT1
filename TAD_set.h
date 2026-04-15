#ifndef TAD_SET_H
#define TAD_SET_H
#include "String.h"
#include <stdio.h>
#include <stdlib.h>
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

void insert_set(Tdata* set, Tdata elem); //pasa el conjunto por referencia y se inserta un nuevo elemento (sin duplicados)
int belongs(Tdata set, Tdata elem); //retorna 0 si no pertenece, 1 si pertenece el elemento al conjunto
void remove_set(Tdata* set, Tdata elem); //elimina un elemento del conjunto
Tdata create_set();
// Operaciones algebraicas
Tdata union_set(Tdata A, Tdata B); 
Tdata intersection_set(Tdata A, Tdata B); 
Tdata difference_set(Tdata A, Tdata B); // son los elementos que pertenecen al primer conjunto y no pertenecen al segundo conjunto
int subset(Tdata A, Tdata B);//retorna 1 si es subconjunto y 0 si no lo es
int equals_set(Tdata A, Tdata B); //retorna 1 si son iguales y 0 si no lo son
#endif
