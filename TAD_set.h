#ifndef TAD_SET_H
#define TAD_SET_H

#include "TAD_ast.h"

int belongs(Tdata set, Tdata elem); //retorna 0 si no pertenece, 1 si pertenece el elemento al conjunto
void remove_set(Tdata* set, Tdata elem); //elimina un elemento del conjunto
// Operaciones algebraicas
Tdata unionset(Tdata A, Tdata B); 
Tdata intersection_set(Tdata A, Tdata B); 
Tdata difference_set(Tdata A, Tdata B); // son los elementos que pertenecen al primer conjunto y no pertenecen al segundo conjunto
int subset(Tdata A, Tdata B);//retorna 1 si es subconjunto y 0 si no lo es
int equals_set(Tdata A, Tdata B); //retorna 1 si son iguales y 0 si no lo son
void insert_set(Tdata* set, Tdata elem);
#endif

