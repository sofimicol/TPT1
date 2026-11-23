#include "TAD_ast.h"
#include "TAD_set.h"
void insert_set(Tdata* set, Tdata elem) {
	if (set == NULL || *set == NULL || elem == NULL)
		return;
	if (belongs(*set, elem))
		return;
	Tdata nuevo = malloc(sizeof(TNodo));
	nuevo->nodeType = 2;
	nuevo->data = copy_ast(elem);
	nuevo->next = NULL;
	if ((*set)->data == NULL) {
		(*set)->data = nuevo;
		return;
	}
	Tdata aux = (*set)->data;
	while (aux->next != NULL)
		aux = aux->next;
	aux->next = nuevo;
}
int belongs(Tdata set, Tdata elem){ // devuelve 1 si pertenece al conjunto
	if (set == NULL || set->data == NULL)
		return 0;
	Tdata aux = set->data;
	while(aux != NULL && equals_node(aux->data, elem) == 0){
		aux = aux->next;
	}
	if(aux == NULL ||set->data==NULL) {
		return 0;
	}else {
		return 1;
	}
}
void remove_set(Tdata* set, Tdata elem) {
    if (set == NULL || esvacio(*set) == 1 || elem == NULL) {
        return;
    }

    Tdata actual = (*set)->data;
    Tdata anterior = NULL;

    while (actual != NULL && !equals_node(actual->data, elem)) {
        anterior = actual;
        actual = actual->next;
    }

    if (actual != NULL) {
        if (anterior == NULL) {
            (*set)->data = actual->next;
        } else {
            anterior->next = actual->next;
        }
        actual->next = NULL;
        free_ast(actual->data);
        free(actual);
    }
}
Tdata unionset(Tdata A, Tdata B) {
    Tdata resultado = create_set();
    Tdata aux;

    if (esvacio(A) == 0) {
        aux = A->data;
        while (aux != NULL) {
            insert_set(&resultado, aux->data);
            aux = aux->next;
        }
    }

    if (esvacio(B) == 0) {
        aux = B->data;
        while (aux != NULL) {
            insert_set(&resultado, aux->data);
            aux = aux->next;
        }
    }
    return resultado;
}

Tdata intersection_set(Tdata A, Tdata B) {
    Tdata resultado = create_set();

    if (esvacio(A) == 1 || esvacio(B) == 1) {
        return resultado;
    }

    Tdata aux = A->data;
    while (aux != NULL) {
        if (belongs(B, aux->data) == 1) {
            insert_set(&resultado, aux->data);
        }
        aux = aux->next;
    }
    return resultado;
}
Tdata difference_set(Tdata A, Tdata B) {
    Tdata resultado = create_set();

    if (esvacio(A) == 1) {
        return resultado;
    }

    Tdata aux = A->data;

    if (esvacio(B) == 1) {
        while (aux != NULL) {
            insert_set(&resultado, aux->data);
            aux = aux->next;
        }
        return resultado;
    }

    while (aux != NULL) {
        if (belongs(B, aux->data) == 0) {
            insert_set(&resultado, aux->data);
        }
        aux = aux->next;
    }
    return resultado;
}
		int equals_set(Tdata A, Tdata B) {
			int a_en_b = subset(A, B);
			int b_en_a = subset(B, A);
			if (a_en_b == 1 && b_en_a == 1) {//Si A esta contenida en B y B esta contenida en A, entonces son iguales
				return 1;
			} else {
				return 0;
			}
		}
		
int subset(Tdata A, Tdata B) {
    // Conjunto vacio es subconjunto de cualquier conjunto
    if (esvacio(A)) return 1;

    if (esvacio(B)) return 0;

    Tdata aux = A->data;
    while (aux != NULL) {
        if (belongs(B, aux->data) == 0) {
            return 0;
        }
        aux = aux->next;
    }
    return 1;
}
		
