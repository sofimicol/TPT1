#include "TAD_set.h"
#include <stdio.h>
#include <stdlib.h>

int esvacio(Tdata set);

Tdata create_set(){ //crea conjunto vacio
	Tdata set; 
	set->nodeType=2;
	set->data=NULL;
	set->next=NULL; 
	return set;
}
void insert_set(Tdata* set, Tdata elem){
	Tdata aux;
	if(esvacio(*set)==1){
		*set->data=elem;
	}
	else{
		aux=*set;
		while(aux->next!=NULL){
			aux=aux->next;
		}
		aux->next=elem;
	}
}
int belongs(Tdata set, Tdata elem){ //pertenece elem es str o set-list
	Tdata aux;
	if(esvacio(set)==1){
		return 0;
	}
	aux=set;
	while(aux->sig!=NULL && aux->data!=elem->data){
		aux=aux->sig;
	}
	if(aux->sig==NULL){
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
		while(actual->sig!=NULL && actual->data!=elem->data){
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
void eliminarprimero(Tdata* set){
	Tdata aux;
	if(*set!=NULL){
		aux=*set;
		(*set)=(*set)->sig;
		free(aux);
	}
}

// Operaciones algebraicas
Tdata union_set(Tdata A, Tdata B); 
Tdata intersection_set(Tdata A, Tdata B); 
Tdata difference_set(Tdata A, Tdata B); // son los elementos que pertenecen al primer conjunto y no pertenecen al segundo conjunto
int subset(Tdata A, Tdata B){ // {1,2} {1,3,4,5,6} o {1,2,3,4,5,6} {1,2} // crear un modulo que retorne el tamaño y se comparen, A debe ser mas pequeño de B
	//suponiendo que la funcion pregunta que A contiene a B
	int ban=0;
	Tdata auxA=auxB=NULL;
	if(tamanioset(A)<tamanioset(B)){
		auxA=A;
		while(auxA->next!=NULL&&b==0){
			auxB=B; // ban=pertenece(B,a->data);
			while(auxB->next!=NULL){
				if(auxA->data!=auxB->data){
					ban=1; // un elemento de A no pertenece a B
				}
				auxB=auxB->next;
			}
			auxA=auxA->next;
		}
		if(b==0){
			return 1; // A es un subconjunto de B
		}
		else{
			return 0; // A no es subconjunto de B
		}
	}else{
		printf("\No se cumple la definicion de subconjunto");
	}
}
int equals_set(Tdata A, Tdata B); //retorna 1 si son iguales y 0 si no lo son
#endif
