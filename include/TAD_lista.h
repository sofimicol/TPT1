#ifndef TAD_LISTA_H
#define TAD_LISTA_H
#include "TAD_ast.h"

int length(Tdata list);
Tdata copy_list(Tdata list);
Tdata concatena_list(Tdata list1, Tdata list2);
int search(Tdata list, Tdata elem);

#endif
