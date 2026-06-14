#ifndef TAD_AFND_H
#define TAD_AFND_H 

#include "TAD_ast.h"
#include "TAD_set.h"
#include "TAD_str.h"
#include "TAD_lista.h"
#include <stdio.h>

/* Estructura del aut?mata con matriz dispersa (Propuesta 3) */
typedef struct {
	Tdata Q;                /* conjunto de estados (cada uno es Tdata STR) */
	Tdata Sigma;            /* conjunto de s?mbolos (cada uno es Tdata STR) */
	Tdata** delta;          /* matriz de transiciones: delta[estado][s?mbolo] = conjunto de destinos */
	int num_estados;        /* |Q|, tama?o de la primera dimensi?n */
	int num_simbolos;       /* |S|, tama?o de la segunda dimensi?n */
	int q0;                 /* ?ndice del estado inicial */
	Tdata F;                /* conjunto de estados finales (Tdata STR) */
} Automata;

typedef Automata* AF;

/* ========== Carga desde archivo txt ========== */
/* Formato del archivo (5 l?neas):
L?nea 1: estados separados por coma (ej: q0,q1,q2)
L?nea 2: alfabeto separado por coma (ej: a,b)
L?nea 3: transiciones en formato origen,simb,destino (pueden ser m?ltiples, separadas por coma)
Para mas destinos usamos ';' (ej: q0,a,q1;q2)
L?nea 4: estado inicial (ej: q0)
L?nea 5: estados finales separados por coma (ej: q2)
*/
AF create_automata();                       /* Crea y devuelve un automata vacio */
void mostrar_automata(AF aut);
void free_automata(AF aut);                 /* Libera memoria ocupada por un automata */
int cargar_automata_desde_archivo(AF aut, const char* ruta);   /* Lee un archivo de texto y carga el aut?mata */
/* ruta sera el nombre del archivo. Devolvera 1 si se leyo correctamente, 0 si error */

/* ========== Operaciones usando str (cadenas de TAD_str) ========== */
/* Agregar una transici?n (por nombres, usando str) */
void agregar_transicion(AF aut, str from, str symbol, str to);

/* Obtener destinos a partir de nombres (devuelve un SET de nombres de estados) */
Tdata transicion_por_nombre(AF aut, str estado, str simbolo);

/* Convertir nombre de estado a ?ndice (devuelve -1 si no existe) */
int estado_a_indice(AF aut, str nombre);

/* Convertir nombre de s?mbolo a ?ndice (devuelve -1 si no existe) */
int simbolo_a_indice(AF aut, str nombre);

/* ========== Operaciones por ?ndice (r?pidas - acceso O(1)) ========== */
/* Obtener destinos a partir de ?ndices (devuelve un SET de strings con los nombres) */
Tdata transicion_por_indice(AF aut, int from, int symbol);   /* Si no hay transici?n, devuelve conjunto vac?o */

/* ========== Determinismo ========== */
int esDeterminista(AF aut);

/* ========== Validacion de Cadena ========== */
int validar_cadena(AF aut, str w);

#endif
