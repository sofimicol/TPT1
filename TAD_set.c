#include "TAD_AST.h"
#include "TAD_set.h"
void insert_set(Tdata* set, Tdata elem){ //insertar un elemento en un conjunto sin duplicados
	Tdata aux;
	if(esvacio(*set)){
		(*set)=copia_ast(elem);
	}
	else{
		aux=*set;
		while(aux->next!=NULL){
			aux=aux->next;
		}
		aux->next=copia_ast(elem);
	}
}
int belongs(Tdata set, Tdata elem) { //pertenece elem es str o set-list
	Tdata aux;
	if(esvacio(set)==1){
		return 0;
	}
	aux=set;
	while(aux->next!=NULL && compara(aux->data,elem->data)) {  // crear una funcion que sea son distintos y se pasen los data?
		aux=aux->next;
	}
	if(aux->next==NULL){
		return 0; //elem no pertenece al conjunto, llegamos al final del conjunto
	}
	else{
		return 1; //el elem pertenece al conjunto
	}
}
	void remove_set(Tdata* set, Tdata elem){ //elem es str o set-list
		Tdata actual;
		Tdata anterior=NULL;
		if(esvacio(*set)==1){
			printf("\nEl conjunto es vacio, no se pudo eliminar elemento");
		}else{
			actual=*set;
			while(actual->next!=NULL && !equals_node(actual->data,elem->data)){ // funcion generica
				anterior=actual;
				actual=actual->next;
			}
			if(actual->data==elem->data){ //elemento a eliminar encontrado, se debe eliminar
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
// Operaciones algebraicas
Tdata union_set(Tdata A, Tdata B) {
	Tdata resultado = create_set(); // Declaro set
	Tdata aux;
	Tdata nuevo;
			
	if(esvacio(A) == 1 && esvacio(B) == 1) {
			resultado = copy_ast(A); // Copio los datos de A a set resultado
			while(B != NULL) {
				aux = A;
				while(aux != NULL && belongs(B->data, A->data) == 1) {
					aux = aux->next;
				}
				nuevo = create_set(); // Declaro nuevo set
				nuevo->data = copia_ast(aux->data); // Copio los datos de  A a nuevo
				append(&resultado, nuevo); // 
				B = B->next;
		}		
		}else {
			if(esvacio(A) == 1 && esvacio(B) == 0) {
					resultado = copy_ast(A); // Copio los datos de set A a set resultado
			}else {
					resultado = copy_ast(B); // Copio los datos de set B a set resultado
			}
		}
	return resultado;
} 
Tdata intersection_set(Tdata A, Tdata B) {
	Tdata resultado = create_set();
	Tdata aux1;
	Tdata aux2;
	Tdata nuevo=NULL;
	
	if (esvacio(A == 0)) {
		resultado = copia_ast(A);
	}else {
		if(estavacio(B) == 0) {
			resultado = copia_ast(B);
		}else {
			resultado = copia_ast(A);
			aux1 = A;
			while(aux1 != NULL) {
				aux2 = B
				while(aux2 != NULL && belongs(aux1->data, aux2->data) == 0) {
					aux2 = aux2->next;
				}
				nuevo = create_set();
				nuevo->data = copia_ast(aux2->data);
				append(&rasultado, nuevo);
				aux1 = aux1->next;
			}
		}
	}
	return resultado;
}
Tdata difference_set(Tdata A, Tdata B) { // Tomando el encuenta orede de los parametros de tipo A - B
	Tdata resultado=create_set();
	Tdata aux1;
	Tdata aux2;
	Tdata nuevo=NULL;
	if(esvacio(A) == 0 || esvacio(B) == 0){
		resultado = copia_ast(A);
	}else {
		resultado = copia_ast(A);
		aux1 = A;
		while(aux1 != NULL) {
			aux2 = B;
			while(aux2 != NULL && belongs(aux1->data, aux2->data) == 1) {
				aux2 = aux2->next;
			}
			nuevo = create_set();
			nuevo->data= copia_ast(aux2->data);
			append(&resultado, nuevo);
			aux1 = aux1->next;
		}
	}
	return resultado;
} // son los elementos que pertenecen al primer conjunto y no pertenecen al segundo conjunto
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
	Tdata auxA = A;
	while (auxA != NULL) {
		if (search(B, auxA->data) == 0) {//Busca el elemento en la lista 
			return 0;//El elemento no esta en el conjunto 
		}
		auxA = auxA->next;
	}
	return 1; //Todos los elementos de B estan en A
}
}

