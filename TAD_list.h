#ifndef TAD_LIST_H
#define TAD_LIST_H

typedef struct dataType* Tdata;

 // Operaciones sobre LIST
void append(Tdata* lista, Tdata elemento);
int length(Tdata lista);
Tdata copy_list(Tdata lista);
Tdata concat(Tdata lista1, Tdata lista2);
int search(Tdata lista, Tdata elemento);

#endif
