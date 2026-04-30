#include "TAD_AST.h"
#include "TAD_list.h"

 // Cargar una lista que dentro puede tener datos str, set o list
void append(Tdata* list, Tdata elem) {
	Tdata nuevo; 
	nuevo = copia_ast(elem);
	if (estavacio(*list)) {
		(*list) = nuevo; // Primer elemento
	}else {
		Tdata aux = *list;
		while (aux->next != NULL) { // Desplazamiento para obtener posicion del anteultimo nodo
			aux = aux->next;
		}
		aux->next = nuevo;// Cargo el elemento al final de la lisa
	}
}
int length(Tdata list) { // Calcula el tamanio de lalista
	int c = 0; // Contador
	Tdata aux = list;
	while(aux != NULL) {
		c++;
		aux = aux->next;
	}
	return c; // Retorno tamanio de lista
}
 // Copia una lista y la retorna  [1,{0,1},[1]]
Tdata copy_list(Tdata list) {
	Tdata nuevo;
	if (list == NULL) { // Verifica que la lista no este vacia
		return NULL;
	}
	if (list->nodeType == 1) {
		nuevo->string = copy_str(list->string);  
	} else { // Si el elemento es una lista o conjunto entonces tiene mas elementos por lo que llamo a la misma funcion con el sig elem anidado
		if(list->nodeType == 2){
			nuevo=create_set();
			nuevo->data = copy_list(list->data); 
			nuevo->next = copy_list(list->next);
		}
		else{
			nuevo=create_list();
			nuevo->data = copy_list(list->data); 
			nuevo->next = copy_list(list->next);
		}
	}
	return nuevo; // Retorno el nuevo arbol 
}
Tdata concat(Tdata L1, Tdata L2) {
    Tdata resultado = create_list();
    Tdata aux;
	 // Copio los elementos de L1 a resultado
    aux = L1;
    while(aux != NULL){
        append(&resultado, aux->data);
        aux = aux->next;
    }
    aux = L2; 
	 // Copio los elementos de L2 al final de resultado
    while(aux != NULL){
        append(&resultado, aux->data);
        aux = aux->next;
    }
    return resultado;
}
int search(Tdata list, Tdata elem) {
	Tdata aux = copy_list(list);
	while (aux != NULL) {
		if (equals_node(aux->data, elem) == 1) { // Comparo los nodos
			return 1; // El elemento se ecnontro en la lista
		}
		aux = aux->next; // Contuinua el ciclo hasta que se termine la lista
	}
	return 0; // El elemento no se encontro 
}
