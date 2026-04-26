#include "TAD_set.h"
#include <stdio.h>
#include <stdlib.h>
#include "TAD_str.h"

int esvacio(Tdata set);
Tdata copia_ast(Tdata elemento);

Tdata create_gen(Tdata data,int tipo){ //crea conjunto vacio
	Tdata gen;  
	gen=(Tdata)
	gen->nodeType=tipo; // [2] o [3] 
	gen->data=data; 
	gen->next=NULL; 
	return gen;
}
Tdata create_str(){
	Tdata str; 
	set->nodeType=1;
	return str;
}
Tdata create_ast(){
	Tdata nuevo;
	int tipo;
	printf("\nIngrese el tipo de dato que quiere ingresar: [1] str, [2] set y [3] list");
	scanf("%d",&tipo);
	do{ // verifica que se esten ingresando numeros dentro del rango solicitado entre 1 y 3
		printf("\nIngrese el tipo de dato que quiere ingresar: [1] str, [2] set y [3] list");
		scanf("%d",&tipo);
	} while(tipo<1 && tipo>3);
	
	if(tipo==1){
		nuevo=create_str(); //crea un Tdata del tipo str
		nuevo->string=load(); //leo el str del conjunto o lista
	}
	else {
		Tdata data=create_ast();
		nuevo=create_gen(data,tipo); //crea un Tdata del tipo set o list
	}
	return nuevo;
}

Tdata copia_ast(Tdata elemento){ 
	Tdata ast;
	ast=(Tdata)malloc(sizeof(Tdata));
	ast->nodeType=elemento->nodeType;// numero que representa el tipo de dato a almacenar 
	ast->data=elemento->data;
	ast->next=NULL;
	return ast;
}
Tdata cargar_conjunto(){
	Tdata arbol;
	Tdata elem;
	printf("\nIngrese el 0 para terminar");
	int opcion;
	opcion=1;
	do{
		elem=create_ast();
		insert_set(&arbol,elem);
	} while(opcion!=0);
	
}
void insert_set(Tdata* set, Tdata elem){ //insertar un elemento en un conjunto sin duplicados
	Tdata aux;
	if(esvacio(*set)==1){
		*set->data=elem;
	}
	else{
		aux=*set;
		while(aux->next!=NULL){
			aux=aux->next;
		}
		aux->next=copia_ast(elem);
	}
}
	
int belongs(Tdata set, Tdata elem){ //pertenece elem es str o set-list
	Tdata aux;
	if(esvacio(set)==1){
		return 0;
	}
	aux=set;
	while(aux->sig!=NULL && compara(aux->data,elem->data)){  // crear una funcion que sea son distintos y se pasen los data?
		aux=aux->next;
	}
	if(aux->sig==NULL){
		return 0; //elem no pertenece al conjunto, llegamos al final del conjunto
	}
	else{
		return 1; //el elem pertenece al conjunto
	}
	
void remove_set(Tdata* set, Tdata elem){ //elem es str o set-list
	Tdata actual;
	Tdata anterior=NULL;
	if(esvacio(*set)==1){
		printf("\nEl conjunto es vacio, no se pudo eliminar elemento");
	}else{
		actual=*set;
		while(actual->sig!=NULL && actual->data!=elem->data){ // funcion generica
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
Tdata union_set(Tdata A, Tdata B) {
	Tdata resultado = create_set(); // Declaro set
	Tdata fin = resultado;
	Tdata aux; 
	Tdata nuevo;
	
	if(esvacio(A) == 1 && esvacio(B) == 1) {
		resultado = copy_ast(A); // Copio los datos de set A a set resultado
		aux = B;
		
		while(fin != NULL) {
			fin = fin->next;
		}
		while(aux != NULL) {
			if (belong(fin->data, aux->data) == 0) {
				nuevo = create_set(); // Declaro nuevo set
				//nuevo->data = copia_ast(aux->data); //Copio los datos de set B a set nuevo
				
				nuevo->next = fin; // Carga al final de la lista
				fin = nuevo;
			}
			aux = aux->next;
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
Tdata intersection_set(Tdata A, Tdata B); 
Tdata difference_set(Tdata A, Tdata B); // son los elementos que pertenecen al primer conjunto y no pertenecen al segundo conjunto
int subset(Tdata A, Tdata B){ // {1,2} {1,3,4,5,6} o {1,2,3,4,5,6} {1,2} // crear un modulo que retorne el tamaño y se comparen, A debe ser mas pequeño de B
	//suponiendo que la funcion pregunta que A contiene a B
	int ban=0;
	Tdata auxA=auxB=create_gen(2);
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


/*-------------------------------------------------------------
-----------------------------------------------------
---------------------------------------------------*/
//Operaciones para listas
Tdata create_list(){ //crea conjunto vacio
	Tdata list; 
	set->nodeType=3;
	set->data=NULL;
	set->next=NULL; 
	return list;
}

// Objetivo append (Conentar un elemento al ultimo de la lista)	
// Condiciones (Modificar Listas originales)
// Uso de referencias (Para desplazarse) - (Para guardar datos del nodo) - (Para encontrar el anteultimo nodo) - (Para enlazar nodos)
	
	Tdata append(Tdata* L1, Tdata elemento) { //cargar una lista que dentro puede tener datos str, set o list
		Tata aux = *L1; // Referencia para desplazamiento
		Tdata nuevo = create_ast(3); // Nuevo nodo para cargar elemento
		
		///nuevo->nodoType = // Elemento generico
		
		///copy_str(nuevo->data, elemento); // Berificar argumentos que recibe copy_str
		
		if (*(L1 == NULL) {
			*L1 = nuevo;
			return NULL;
		}else {
			while (aux->next != NULL) { // Desplazamiento para obtener posicion del anteultimo nodo
				aux = aux->next;
			}
			
			aux->next = nuevo; // Cargo el elemento al final de la lisa
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

// Objetivo de concatenar (Conentar ultimo nodo de L1 con L2)
// Condiciones (Conservar Listas originales) - (Retornar una nueva Lista)
// Uso de referencias (Para desplazarse) - (Para guardar datos del nodo) - (Para encontrar el ultimo nodo) - (Para enlazar nodos)

Tdata concat(Tdata L1, Tdata L2) { 
	Tdata resultado;
	Tdata fin; // referencia a ultimo nodo de L1
	Tdata aux; // nodo aux para recorrer L2
	
	if (L1 == NULL && L2 == NULL) { // posible funcion esVacio
		return NULL;
	}else{
		resultado = copy_list(L1); // No se mocifica L1 se guarda en resultado
		fin = resultado; // referencia a ultimo nodo de L1
		
		while(fin->next != NULL) { // busco el ultimo de L1
			fin = fin->next;
		}
	}
	
	aux = L2; // Referencia para desplazarce sobre L2 y copiar datos de L2 a resultado
	
	while(aux != NULL) {
		Tdata nuevo = (Tdata)malloc(sizeof(Tdata)); // Creo nuevo nodo para guardar datos de L2
		
		//Berificar argumentos que recibe copy_str
		///copy_str(nuevo->data, aux->data); // Funcion que reeplazaria a strcpy de libreria strin.h
		
		// Concateno ultimo nodo de L1 con el nodo de L2
		fin->next = nuevo;
		fin = nuevo;
		
		aux = aux->next; // Muevo referencia al siguiente
	}
	return resultado;
}
/*-------------------------------------------
------------------------------------------------
-----------------------------------------------*/

Tdata copy_set(Tdata A) {
	Tdata nuevo;
	while(B!=NULL){
		insert_set(&nuevo,B->data);
	}
	return nuevo;
}

Tdata union_set(Tdata A, Tdata B) {
	Tdata resultado; //creategen
	Tdata aux = B; 
	Tdata nuevo;
	resultado=copy_set(A); 	
	Tdata fin = resultado;
	while(fin != NULL) {
		fin = fin->next;
	}
		while(aux != NULL) {
			if (belong(fin->data, aux->data)==0) {
				
				nuevo = create_gen(null,2); //no esta
				nuevo->data=copia_ast(aux->data);
				
				nuevo->next = fin;
				fin = nuevo;
			}
			aux = aux->next;
		}
	}
	return resultado;	
}

Tdata intersection_set(Tdata A, Tdata B) {
	Tdata resultado;
	Tdata aux1 = A;
	Tdata aux2 = B;
	Tdata fin;
	
	copy_set(resultado, A);
	
	if(!esvacia(A) && !esvacia(B) {
		return -1;
	}else{
		while(aux1 != NULL) {
			while(aux2 != NULL && belong(aux1->data, aux2->data) != 0) {
				aux2 = aux2->next;
			}
			
			nuevo = create_set();
			copy_ser(nuevo->data, aux2->data);
			
			nuevo->next = resultado;
			resultado = nuevo;
			
			aux1 = aux1->next;
		}
	}
	return resultado;
}

Tdata difference_set(Tdata A, Tdata B) {
	Tdata resultado;
	Tdata aux1 = A;
	Tdata aux2 = B;
	Tdata fin;
	
	copy_set(resultado, A);
	
	if(!esvacia(A) && !esvacia(B) {
		return -1;
	}else{
		while(aux1 != NULL) {
			while(aux2 != NULL && belong(aux1->data, aux2->data) != 1) {
				aux2 = aux2->next;
			}
			
			nuevo = create_set();
			nuevo->data= copia_ast(aux2->data);
			
			nuevo->next = resultado;
			resultado = nuevo;
			
			aux1 = aux1->next;
		}
	}
	return resultado;
}
int equals_node(Tdata n1, Tdata n2) {
	if (n1 == NULL && n2 == NULL) 
		return 1;
	else {
		if(n1 == NULL || n2 == NULL)
			return 0;
		else { 
			if (n1->nodeType != n2->nodeType) 
				return 0;
			else {
				if (n1->nodeType == 1) { 
					return compare_str(n1->string, n2->string);
				} else { // Comparación estructural para Listas/Conjuntos
					return equals_node(n1->data, n2->data) && equals_node(n1->next, n2->next);
				}
			}
		}
	}
}

int search(Tdata list, Tdata elem) {
	Tdata aux = list;
	while (aux != NULL) {
		if (equals_node(aux->data, elem)) { //Comparo los nodos
			return 1; // El elemento se ecnontro en la lista
		}
		aux = aux->next; //Contuinua el ciclo hasta que se termine la lista
	}
	return 0; //El elemento no se encontro 
}

Tdata copy_list(Tdata list) {//Copia una lista y la retorna
	if (list == NULL) { // Verifica que la lista no este vacia
		return NULL;
	}
	Tdata nuevo = copia_ast(list);
	if (list->nodeType == 1) {
		nuevo->string = //copy_str(list->string);  
	} else { //Si el elemento es una lista o conjunto entonces tiene mas elementos por lo que llamo a la misma funcion con el sig elem anidado
		nuevo->data = copy_list(list->data); 
		nuevo->next = copy_list(list->next);
	}
	return nuevo; //Retorno el nuevo arbol 
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

int equals_set(Tdata A, Tdata B) {
	int a_en_b = subset(A, B);
	int b_en_a = subset(B, A);
	if (a_en_b == 1 && b_en_a == 1) {//Si A esta contenida en B y B esta contenida en A, entonces son iguales
		return 1;
	} else {
		return 0;
	}
}

#endif
