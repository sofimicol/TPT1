
#include "TAD_str.h"
void append_str(str*cabeza, str nuevo);
str create_nodo(char c);
str create_str_ast(){//crea una estructura de tipo str sin datos 
	return NULL;
}
str create_nodo(char c){ //crear un nodo del tipo str con un dato pasado por parametro
	str nuevo;
	nuevo=(str)malloc(sizeof(TNodo));
	nuevo->data=c;
	nuevo->next=NULL;
	return nuevo;
}
void append_str(str*cabeza,str nuevo){ //insertar lista de caracteres (por cola) estructura FIFO
	str aux;
	if(*cabeza==NULL){
		*cabeza=nuevo; // se inserta el primer elemento de la lista de caracteres
	}
	else {
		aux=*cabeza;
		while(aux->next!=NULL){ // se recorre la lista hasta la ultima posicion y se inserta el caracter a la lista (por cola)
			aux=aux->next;
		}
		aux->next=nuevo;// inserta el caracter en la ultima pos
	}
}

str load(){ //insertar una cadena de caracteres por consola y se convierte en una lista enlazada de caracteres
	str lis;// sera la cabeza de la lista de caracteres (puntero al primer nodo de la lista)
	str nuevo; // sera el nodo único que contiene un caracter
	lis=nuevo=NULL;
	char c;	//se utilizara como variable para procesar cada caracter de la cadena
	printf("\nIngrese la cadena a procesar: "); //el buffer de entrada queda en espera de el ingreso de una cadena 
	fflush(stdin); //limpiar el buffer
	c=getchar(); // se toma el primer elemento de la cadena
	while(c!='\n'){ //n sera la condicion de parada pues es el salto de linea 
		nuevo=create_nodo(c); //el nodo tiene el valor del caracter actual
		append_str(&lis,nuevo); // se agrega en una lista enlazada (por cola) el nuevo nodo con el caracter
		c=getchar(); // se toma el caracter siguiente de la cadena de caracteres
	}
	return lis; // se retorna la lista enlazada de caracteres
}
str load2(const char*c){ //insertar una cadena de caracteres predeterminado por el programador y se convierte en una lista enlazada de caracteres
	str lis, nuevo; 
	lis=nuevo=NULL;
	int i; //indice de un array de caracteres
	i=0; // se inicializa el indice en 0
	while(c[i]!='\0'){ // ultimo caracter del array agregado por el compilador
		nuevo=create_nodo(c[i]); //inserta el caracter de la posicion i al nodo
		append_str(&lis,nuevo);
		i++;
	}
	return lis;
}
void print (str cad){ // muestra la lista de caracteres
	if(cad!=NULL){
		while(cad!=NULL){
			printf("%c",cad->data);
			cad=cad->next;
		}
	}else{
		printf("\nCadena vacia");
	}
}
str concat(str cad1,str cad2){
	str concat; // se crea un nuevo dato str para evitar la modificacion de cadenas a concatenar
	str nuevo,aux;
	concat=nuevo=aux=NULL;
		aux=cad1;
		while(aux!=NULL){
			nuevo=create_nodo(aux->data);
			append_str(&concat,nuevo);
			aux=aux->next;
		}
		aux=cad2;
		while(aux!=NULL){
			nuevo=create_nodo(aux->data);
			append_str(&concat,nuevo);
			aux=aux->next;
		}
	return concat;
}
int compare_str(str cad1, str cad2){
	while(cad1!=NULL && cad2!=NULL){
			if(cad1->data!=cad2->data){
				return cad1->data-cad2->data; // compara los codigos ASCII de los caracteres evitando tomarlos como char 
			}
			cad1=cad1->next; //sigue al siguiente caracter
			cad2=cad2->next;
		}
		
		//si ambas terminaron iguales
		if(cad1==NULL && cad2==NULL){
			return 0;
		}
		
		// si una es más larga
		if(cad1==NULL){
			return -1; //s1 es más corta
		} else {
			return 1;  //s2 es más corta
		}
}
void free_str(str c){ //libera memoria de str 
	str aux;
	while(c!=NULL){
		aux=c;
		c=c->next;
		free(aux);
		}
}
str copy_str(str original) {
	str nueva_lista = NULL; 
	str aux = original;     
	str nuevo_nodo;
	while (aux != NULL) {
		nuevo_nodo = create_nodo(aux->data);
		append_str(&nueva_lista, nuevo_nodo);
		aux = aux->next;
	}
	return nueva_lista; 
}
