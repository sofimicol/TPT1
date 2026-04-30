#ifndef TAD_LIST_H
#define TAD_LIST_H

typedef struct dataType* Tdata;

 // Operaciones sobre LIST
void append(Tdata* list, Tdata elemento);
int length(Tdata list);
Tdata copy_list(Tdata list);
Tdata concat_list(Tdata list1, Tdata list2);
int search(Tdata list, Tdata elemento);

#endif
