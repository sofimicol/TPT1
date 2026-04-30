
#include "TAD_str.h"

void append_str(str*cabeza, str nuevo);
str create_nodo(char c);

str create_str_ast() { // Crea una estructura de tipo str sin datos 
	return NULL;
}
str create_nodo(char c){ // Crear un nodo del tipo str con un dato pasado por parametro
	str nuevo;
	nuevo=(str)malloc(sizeof(TNodo));
	nuevo->data = c;
	nuevo->next = NULL;
	return nuevo;
}
void append_str(str* hed, str nuevo){ // Insertar lista de caracteres (por cola) estructura FIFO
	str aux;
	if(*hed == NULL){
		*hed = nuevo; // Se inserta el primer elemento de la lista de caracteres
	}
	else {
		aux = *hed;
		while(aux->next != NULL){ // Se recorre la lista hasta la ultima posicion y se inserta el caracter a la lista (por cola)
			aux = aux->next;
		}
		aux->next = nuevo;// Inserta el caracter en la ultima pos
	}
}
str load() { // Insertar una cadena de caracteres por consola y se convierte en una lista enlazada de caracteres
	str lis; // Sera la cabeza de la lista de caracteres (puntero al primer nodo de la lista)
	str nuevo; // Sera el nodo único que contiene un caracter
	lis = nuevo = NULL;
	char c;	// Se utilizara como variable para procesar cada caracter de la cadena
	printf("\nIngrese la cadena a procesar: "); // El buffer de entrada queda en espera de el ingreso de una cadena 
	fflush(stdin); // Limpiar el buffer
	c = getchar(); // Se toma el primer elemento de la cadena
	while(c != '\n'){ // 'n' sera la condicion de parada pues es el salto de linea 
		nuevo = create_nodo(c); // El nodo tiene el valor del caracter actual
		append_str(&lis, nuevo); // Se agrega en una lista enlazada (por cola) el nuevo nodo con el caracter
		c = getchar(); // Se toma el caracter siguiente de la cadena de caracteres
	}
	return lis; // Se retorna la lista enlazada de caracteres
}
str load2(const char* c) { //insertar una cadena de caracteres predeterminado por el programador y se convierte en una lista enlazada de caracteres
	str lis, nuevo; 
	lis=nuevo=NULL;
	int i; //indice de un array de caracteres
	i=0; // se inicializa el indice en 0
	while(c[i]!='\0'){ // ultimo caracter del array agregado por el compilador
		nuevo=create_nodo(c[i]); //inserta el caracter de la posicion i al nodo
		append_str(&lis, nuevo);
		i++;
	}
	return lis;
}
void print (str cad){ // Muestra la lista de caracteres
	if(cad != NULL){
		while(cad != NULL){
			printf("%c", cad->data);
			cad = cad->next;
		}
	}else{
		printf("\nCadena vacia");
	}
}
str concat(str cad1, str cad2){
	str concat; // Se crea un nuevo dato str para evitar la modificacion de cadenas a concatenar
	str nuevo, aux;
	concat = nuevo = aux = NULL;
	aux = cad1;
	while(aux != NULL) {
		nuevo = create_nodo(aux->data);
		append_str(&concat, nuevo);
		aux = aux->next;
	}
	aux = cad2;
	while(aux != NULL) {
		nuevo = create_nodo(aux->data);
		append_str(&concat, nuevo);
		aux = aux->next;
	}
	return concat;
}
int compare_str(str cad1, str cad2) {
	while(cad1 != NULL && cad2 != NULL) {
			if(cad1->data!=cad2->data) {
				return cad1->data-cad2->data; // Compara los codigos ASCII de los caracteres evitando tomarlos como char 
			}
			cad1=cad1->next; // Sigue al siguiente caracter
			cad2=cad2->next;
	}
	 // si ambas terminaron iguales
	if(cad1 == NULL && cad2 == NULL) {
		return 0;
	}
	 // si una es más larga
	if(cad1 == NULL){
		return -1; // s1 es más corta
	} else {
		return 1;  // s2 es más corta
	}
}
void free_str(str c){ // Libera memoria de str 
	str aux;
	while(c != NULL){
		aux = c;
		c = c->next;
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

