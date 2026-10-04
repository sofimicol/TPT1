#include "TAD_AST.h"
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
	void remove_set(Tdata* set, Tdata elem){ //elem es str o set-list
		Tdata actual;
		Tdata anterior = NULL;
		if(esvacio(*set)==1){
			printf("\nEl conjunto es vacio, no se pudo eliminar elemento");
		}else{
			actual=(*set)->next;
			while(actual->next!=NULL && !equals_node(actual->data,elem)){ // funcion generica
				anterior=actual;
				actual=actual->next;
			}
			if(equals_node(actual->data, elem)){ //elemento a eliminar encontrado, se debe eliminar
				if(anterior==NULL){
					eliminarprimero(set);
				}
				else{
					anterior->next=actual->next;
					free(actual);
					actual=NULL;
				}
			}else{
				printf("\nElemento no encontrado");
			}
		}
	}
		Tdata unionset(Tdata A, Tdata B) { 
			Tdata resultado = create_set();
			Tdata aux;
			
			if(esvacio(A) == 1 && esvacio(B) == 1) {
				printf("\nConjuntos vacios ");
				return NULL;
			}
			
			if(esvacio(A) == 1 && esvacio(B) == 0) { // Si A es vacio y B no, devuelvo B
				aux = B->data;
				while(aux != NULL){
					insert_set(&resultado, aux->data);
					aux = aux->next;
				}
			}
			
			if(esvacio(B) == 1 && esvacio(A) == 0) { // Si B es vacio y A no, devuelvo A
				aux = A->data;
				while(aux != NULL){
					insert_set(&resultado, aux->data);
					aux = aux->next;
				}
			}
			
			aux = A->data;
			while(aux != NULL){
				insert_set(&resultado, aux->data);
				aux = aux->next;
			}
			
			// Agregar B sin duplicados
			aux = B->data;
			while(aux != NULL){
				if(belongs(resultado, aux->data) == 0){
					insert_set(&resultado, aux->data);
				}
				aux = aux->next;
			}
			
			return resultado;
		}
		Tdata intersection_set(Tdata A, Tdata B) {
			Tdata resultado = create_set();
			Tdata aux;
			
			if(esvacio(A) == 1 || esvacio(B) == 1) { // Si A o B no tienen elementos
				return NULL;
			}
			// Si A y B tiene elementos aplico porp. de interseccion
			aux = A->data;
			while(aux != NULL) {
				if(belongs(B, aux->data) == 1) {
					insert_set(&resultado, aux->data);
				}
				aux = aux->next;
			}
			return resultado;
		}
		Tdata difference_set(Tdata A, Tdata B) { // Tomando el encuenta orede de los parametros de tipo A - B
			Tdata resultado = create_set();
			Tdata aux = A->data;
			
			if(esvacio(A) == 1) {
				printf("\nConjunto vacio, NO se puede reslizar la diferencia");
				return NULL;// Si A y B tiene elementos aplico porp. de diferencia
			}
			
			if(esvacio(B) == 1) {
				while(aux != NULL){
					insert_set(&resultado,aux->data);
					aux = aux->next;
				}
				//inserter_set(&resultado, copy_ast(A));
				return resultado;
			}
			
			while(aux != NULL) {
				if(belongs(B, aux->data) == 0) {
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
			Tdata aux = A->data;
			
			// A vac?o ? cualquier conjunto
			if(esvacio(A)) return 1;
			
			// A no vac?o ? vac?o
			if(esvacio(B)) return 0;
			while(aux != NULL) {
				if(belongs(B, aux->data) == 0) {
					return 0;
				}
				aux = aux->next;
			}
			return 1;
		}
		
