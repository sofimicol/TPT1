#include "TAD_ast.h"
#include "TAD_set.h"

void insert_set(Tdata* set, Tdata elem) {
	Tdata aux, nuevo;
	if (set == NULL || *set == NULL || elem == NULL) return;
	if (belongs(*set, elem)) return;
	
	nuevo = malloc(sizeof(TNodo));
	nuevo->nodeType = 2;
	nuevo->data = copy_ast(elem);
	nuevo->next = NULL;
	if( (*set)->data == NULL ) {
		(*set)->data = nuevo;
		return;
	}
	aux = (*set)->data;
	while (aux->next != NULL){
		aux = aux->next;
		aux->next = nuevo;
	}
}
/*void remove_set(Tdata* set, Tdata elem){ // Elemento es str o set-list
	Tdata actual;
	Tdata anterior = NULL;
	if(esvacio(*set) == 1){
		printf("\nEl conjunto es vacio, no se pudo eliminar elemento");
	}else {
		actual = *set;
		while(actual->next != NULL && equals_node(actual->data,elem) == 0){ // Funcion generica
			anterior = actual;
			actual = actual->next;
		}
		if(equals_node(actual->data, elem) == 1){ // Elemento a eliminar encontrado, se debe eliminar
			if(anterior == NULL){
				eliminarprimero(set);
			}else {
				anterior->next = actual->next;
				free(actual);
				actual = NULL;
			}
		}else {
			printf("\nElemento no encontrado");
		}
	}
}*/
int belongs(Tdata set, Tdata elem) { // Devuelve 1 si pertenece al conjunto
	Tdata aux;
	if (set == NULL || set->data == NULL) return 0;
	
	aux = set->data;
	while(aux != NULL && equals_node(aux->data, elem) == 0){
		aux = aux->next;
	}
	if(aux == NULL ||set->data==NULL) {
		return 0;
	}else {
		return 1;
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
	// Si A y B tienen elementos aplico prop. de union
	aux = A->data; // Agregar A al resultado
	while(aux != NULL){
		insert_set(&resultado, aux->data);
		aux = aux->next;
	}
	aux = B->data; // Agregar B sin duplicados
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
 // Tomando el encuenta el oreden de los parametros aplico A - B
Tdata difference_set(Tdata A, Tdata B) {
	Tdata resultado = create_set();
	Tdata aux = A->data;
	if(esvacio(A) == 1) {
		printf("\nConjunto vacio, NO se puede reslizar la diferencia");
		return NULL;
	}
	if(esvacio(B) == 1) {
		while(aux != NULL){
			insert_set(&resultado,aux->data);
			aux = aux->next;
		}
		return resultado;
	}
	// Si A y B tiene elementos aplico porp. de diferencia
	while(aux != NULL) {
		if(belongs(B, aux->data) == 0) {
			insert_set(&resultado, aux->data);
		}
		aux = aux->next;
	}
	return resultado;
}
int subset(Tdata A, Tdata B) {
	Tdata aux = A->data;
	if(esvacio(A)) return 1;
	if(esvacio(B)) return 0;
	
	while(aux != NULL) {
		if(belongs(B, aux->data) == 0) {
			return 0;
		}
		aux = aux->next;
	}
	return 1;
}
// Son los elementos que pertenecen al primer conjunto y no pertenecen al segundo conjunto
int equals_set(Tdata A, Tdata B) {
	int a_en_b = subset(A, B);
	int b_en_a = subset(B, A);
	if (a_en_b == 1 && b_en_a == 1) { // Si A esta contenida en B y B esta contenida en A, entonces son iguales
		return 1;
	} else {
		return 0;
	}
}
