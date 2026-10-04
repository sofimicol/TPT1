#include <stdio.h>
#include <stdlib.h>
#include "TAD_AFND.h"

int main() {
	AF automata = NULL;
	AF dfa = NULL;
	int exito;
	
	// 1. Crear un autómata vacío
	automata = create_automata();
	if (automata == NULL) {
		printf("Error: no se pudo crear el autómata.\n");
		return 1;
	}
	
	// 2. Cargar desde archivo (debe existir "automata.txt" en el mismo directorio)
	printf("Cargando automata desde 'automata.txt'...\n");
	exito = cargar_automata_desde_archivo(automata, "automata.txt");
	
	if (exito == 0) {
		printf("Error: falló la carga del archivo.\n");
		free_automata(automata);
		return 1;
	}
	
	// 3. Actualizar contadores (por si la carga no lo hizo)
	actualizar_contadores(automata);
	
	// 4. Mostrar el autómata cargado
	printf("\n--- AUTOMATA ORIGINAL (AFND) ---\n");
	mostrar_automata(automata);
	
	// 5. Convertir a AFD
	printf("\n\n=== EJECUTANDO ALGORITMO DE SUBCONJUNTOS ===\n");
	dfa = convertir_AFND_a_AFD(automata);
	
	if (dfa != NULL) {
		printf("\n--- AUTOMATA CONVERTIDO (AFD) ---\n");
		mostrar_automata(dfa);
	} else {
		printf("\nError: La conversión falló y devolvió NULL.\n");
	}
	
	// 6. Liberar memoria
	free_automata(automata);
	if (dfa != NULL) {
		free_automata(dfa);
	}
	
	printf("\nPrograma finalizado correctamente.\n");
	return 0;
}
