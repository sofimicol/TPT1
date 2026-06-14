#include "TAD_AFND.h"
#include <stdio.h>
#include <stdlib.h>

int main() {
	AF aut = create_automata();
	if (cargar_automata_desde_archivo(aut, "automata.txt")) {
		mostrar_automata(aut);
	} else {
		printf("Error al cargar el archivo automata.txt\n");
	}
	return 0;
}
