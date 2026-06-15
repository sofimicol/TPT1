#include <stdio.h>
#include <stdlib.h>
#include "TAD_ast.h"
#include "TAD_set.h"
#inlcude "TAD_TablaMapeo.h"
#define MAX_ESTADOS 50 
void inicializar_tabla(TablaMapeo* tabla) {
	tabla->cantidad_actual = 0;
	for(int i = 0; i < MAX_ESTADOS; i++) {
		tabla->estados_originales[i] = NULL;
	}
}

int renombrar_estado(TablaMapeo* tabla, Tdata estado_generado) { //recibe la tabla de mapeo y el nuevo estado
	for (int i = 0; i < tabla->cantidad_actual; i++) { // busca que el estado en la tabla
		if (equals_set(tabla->estados_originales[i], estado_generado) == 1) {
			return i; 
		}
	}
	if (tabla->cantidad_actual < MAX_ESTADOS) {
		tabla->estados_originales[tabla->cantidad_actual] = copy_ast(estado_generado);
		int nuevo_id = tabla->cantidad_actual;
		tabla->cantidad_actual++; 
		return nuevo_id;
	} else {
		printf("\nError: Se alcanzó el límite máximo de estados en el AFD.\n");
		return -1; 
	}
}
