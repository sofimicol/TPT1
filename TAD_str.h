#ifndef TAD_STR_H
#define TAD_STR_H
#include <stdio.h>
#include <stdlib.h>
//estructura de lista enlazada
typedef struct Nodo{
	char data;
	struct Nodo*next;
}TNodo;
typedef TNodo*str;
str create_str_ast(); //crea una estructura de tipo str
str load();//insertar una lista de caracteres por consola 
str load2(const char*); //insertar una lista de caracteres a partir de una cadena de caracteres predeterminada
void print (str); //mostrar lista de caracteres
str concat(str,str); //concatenacion de str
void free_str(str); //libera memoria de str
int compare_str(str cad1, str cad2); //compara si dos listas de caracteres son iguales 
#endif
