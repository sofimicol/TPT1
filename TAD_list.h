#ifndef TAD_LIST_H
#define TAD_LIST_H

typedef struct dataType* Tdata;

void append(Tdata* list, Tdata elem);
int length(Tdata list);
Tdata copy_list(Tdata list);
Tdata concatena_list(Tdata list1, Tdata list2);
int search(Tdata list, Tdata elem);

#endif
