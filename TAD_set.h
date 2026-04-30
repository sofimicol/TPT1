#ifndef TAD_SET_H
#define TAD_SET_H

void insert_set(Tdata* set, Tdata elem); // Pasa el conjunto por referencia y se inserta un nuevo elemento (sin duplicados)
void remove_set(Tdata* set, Tdata elem); // Elimina un elemento del conjunto
int belongs(Tdata set, Tdata elem); // Retorna 0 si no pertenece - 1 si pertenece el elemento al conjunto
int equals_set(Tdata A, Tdata B); // Retorna 1 si son iguales - 0 si no lo son
 // Operaciones algebraicas
Tdata union_set(Tdata A, Tdata B); // Retorna la union de dos conjuntos
Tdata intersection_set(Tdata A, Tdata B); // Retorna la interseccion de dos conjuntos
Tdata difference_set(Tdata A, Tdata B); // Retorna la diferencia de dos conjuntos
int subset(Tdata A, Tdata B);// Retorna 1 si es subconjunto - 0 si no lo es

#endif
