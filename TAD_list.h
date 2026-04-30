#ifndef TAD_LIST_H
#define TAD_LIST_H

typedef struct dataType* Tdata;

 // Operaciones sobre LIST
void append(Tdata* list, Tdata elem);
int length(Tdata list);
Tdata copy_list(Tdata list);
Tdata concat(Tdata list1, Tdata list2);
int search(Tdata list, Tdata elem);

#endif
