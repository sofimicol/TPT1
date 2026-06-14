
#include "TAD_lista.h"

int length(Tdata list) { // Calcula el tamanio de lalista
	int c = 0; // Contador
	Tdata aux = list; // Auxiliar (opcional) para despalzarce
	
	while(aux != NULL) { // Desplazamiento
		c++;
		aux = aux->next;
	}
	
	return c; // Tamanio de la lista
}

Tdata concatena_list(Tdata L1, Tdata L2) { 
	Tdata resultado = create_list(); // Declaro set
	Tdata nuevo;
	if(esvacio(L1) == 0 && esvacio(L2) == 0) {
		resultado = copy_ast(L1);
		Tdata fin = resultado->data;
		while(fin->next != NULL) {
			fin = fin->next;
		}
		Tdata aux = L2->data;
		while(aux != NULL) {
			nuevo = create_list();
			nuevo->data = copy_ast(aux->data); //{0,1}
			//append
			fin->next = nuevo;
			fin = nuevo;
			
			aux= aux->next;
		}
	}else {
		if(esvacio(L1) == 1) {
			resultado = copy_ast(L2);
			
		}else {
			if(esvacio(L2) == 1){
				resultado = copy_ast(L1);
			}
			else{
				resultado=create_list();
			}
		}
	}
	return resultado;
}
int search(Tdata list, Tdata elem) {
	if (list != NULL && elem != NULL) {
		// SALTAMOS al primer eslab?n real de la lista
		Tdata aux = list->data; 
		
		while (aux != NULL) {
			// Comparamos el dato guardado en este eslab?n con el buscado
			if (equals_node(aux->data, elem) == 1) { 
				return 1; // ?Encontrado!
			}
			aux = aux->next; 
		}
		return 0; // Se recorri? toda la cadena y no estaba
	}
	return 0;
}
