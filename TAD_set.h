#ifndef TAD_SET_H
#define TAD_SET_H

 // Operaciones sobre SET
void insert_set(Tdata* set, Tdata elemento); // Pasa el conjunto por referencia y se inserta un nuevo elemento (sin duplicados)
void remove_set(Tdata* set, Tdata elemento); // Elimina un elemento del conjunto
int belongs(Tdata set, Tdata elemento); // Retorna 0 si no pertenece - 1 si pertenece el elemento al conjunto
 // Operaciones algebraicas
Tdata union_set(Tdata set1, Tdata set2); // Retorna la union de dos conjuntos
Tdata intersection_set(Tdata set1, Tdata set2); // Retorna la interseccion de dos conjuntos
Tdata difference_set(Tdata set1, Tdata set2); // Retorna la diferencia de dos conjuntos
int subset(Tdata set1, Tdata set2);// Retorna 1 si es subconjunto - 0 si no lo es
int equals_set(Tdata set1, Tdata set2); // Retorna 1 si son iguales - 0 si no lo son

#endif
