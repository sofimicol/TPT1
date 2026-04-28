#include "TAD_AST.h"
#include "TAD_list.h"
void append(Tdata* list, Tdata elem) { //cargar una lista que dentro puede tener datos str, set o list
	Tdata nuevo; 
	nuevo=copia_ast(elem);
	if (estavacio(*list)) {
		(*list) = nuevo; //primer elemento
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
	Tdata aux = list; // Auxiliar (opcional) para despalzarce
	
	while(aux != NULL) { // Desplazamiento
		c++;
		aux = aux->next;
	}
	
	return c; // Tamanio de la lista
}
//posible ast
Tdata copy_list(Tdata list) {//Copia una lista y la retorna  [1,{0,1},[1]]
	Tdata nuevo;
	if (list == NULL) { // Verifica que la lista no este vacia
		return NULL;
	}
	if (list->nodeType == 1) {
		nuevo->string = copy_str(list->string);  
	} else { //Si el elemento es una lista o conjunto entonces tiene mas elementos por lo que llamo a la misma funcion con el sig elem anidado
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
	return nuevo; //Retorno el nuevo arbol 
}
Tdata concatena_list(Tdata L1, Tdata L2) { 
	Tdata resultado = creat_list(); // Declaro set
	Tdata aux;
	Tdata nuevo;
	if(esvacio(L1) == 1 && esvacio(L2) == 1) {
		resultado = copia_ast(L1);
		aux = L2;
		
		while(L2 != NULL) {
			nuevo = creat_list();
			nuevo->data = copia_ast(L2, 2); //{0,1}
			append(&nuevo, L2);
			L2 = L2->next;
		}
		
	}else {
		if(esvacio(L1) == 1) {
			resultado = copia_ast(L1);
			
		}else {
			if(esvacio(L2) == 1){
				resultado = copia_ast(L2);
			}
			else{
				resultado=NULL;
			}
		}
	}
	return resultado;
}

int search(Tdata list, Tdata elem){
	Tdata aux = copy_list(list);
	while (aux != NULL) {
		if (equals_node(aux->data, elem)==1) { //Comparo los nodos
			return 1; // El elemento se ecnontro en la lista
		}
		aux = aux->next; //Contuinua el ciclo hasta que se termine la lista
	}
	return 0; //El elemento no se encontro 
}
