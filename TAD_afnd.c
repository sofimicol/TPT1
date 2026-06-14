#include "TAD_AFND.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* ========== Funciones auxiliares internas (prototipos) ========== */
str indice_a_str(Tdata set, int indice);
void leer_conjunto_desde_archivo(FILE* f, Tdata* conjunto);
void procesar_linea_transiciones(AF aut, char* linea);
int buscar_indice_en_conjunto(Tdata conjunto, str nombre);
void inicializar_matriz(AF aut);
void destruir_matriz(AF aut);

/* ========== Creaci?n y destrucci?n ========== */
AF create_automata(void) {
	AF nuevo;   /* declaraci?n al inicio */
	/* Crea y devuelve un aut?mata vac?o */
	nuevo = (AF)malloc(sizeof(Automata));
	if (nuevo == NULL) {
		return NULL;
	}
	nuevo->Q = create_set();
	nuevo->Sigma = create_set();
	nuevo->delta = NULL;
	nuevo->num_estados = 0;
	nuevo->num_simbolos = 0;
	nuevo->q0 = -1;
	nuevo->F = create_set();
	return nuevo;
}

/* ========== Inicializaci?n de la matriz (despu?s de conocer Q y Sigma) ========== */
void inicializar_matriz(AF aut) {
	Tdata aux;      /* auxiliar para recorrer conjuntos */
	int i, j;       /* ?ndices para la reserva de memoria */
	
	/* Si ya est? inicializada, no hacer nada */
	if (aut->delta != NULL) {
		return;
	}
	/* Contar estados */
	aut->num_estados = 0;
	aux = obtener_data(aut->Q);
	while (aux != NULL) {
		aut->num_estados++;
		aux = obtener_next(aux);
	}
	/* Contar s?mbolos */
	aut->num_simbolos = 0;
	aux = obtener_data(aut->Sigma);
	while (aux != NULL) {
		aut->num_simbolos++;
		aux = obtener_next(aux);
	}
	/* Reservar matriz: filas = estados, columnas = s?mbolos */
	aut->delta = (Tdata**)malloc(aut->num_estados * sizeof(Tdata*));
	for (i = 0; i < aut->num_estados; i++) {
		aut->delta[i] = (Tdata*)malloc(aut->num_simbolos * sizeof(Tdata));
		for (j = 0; j < aut->num_simbolos; j++) {
			aut->delta[i][j] = create_set();   /* conjunto vac?o */
		}
	}
}

/* ========== Destrucci?n de la matriz ========== */
void destruir_matriz(AF aut) {
	int i, j;       /* ?ndices para recorrer la matriz */
	
	if (aut->delta == NULL) {
		return;
	}
	for (i = 0; i < aut->num_estados; i++) {
		for (j = 0; j < aut->num_simbolos; j++) {
			if (aut->delta[i][j] != NULL) {
				free_ast(aut->delta[i][j]);
			}
		}
		free(aut->delta[i]);
	}
	free(aut->delta);
	aut->delta = NULL;
}

/* ========== Liberaci?n completa del aut?mata ========== */
void free_automata(AF aut) {
	if (aut == NULL) {
		return;
	}
	destruir_matriz(aut);
	free_ast(aut->Q);
	free_ast(aut->Sigma);
	free_ast(aut->F);
	free(aut);
}

/* ========== Funci?n auxiliar: busca un nombre (str) en un conjunto de Tdata ========== */
int buscar_indice_en_conjunto(Tdata conjunto, str nombre) {
	Tdata actual;   /* puntero para recorrer la lista de elementos */
	int indice;      /* ?ndice asociado al elemento actual */
	
	/* Verifica que el conjunto exista y no est? vac?o */
	if (conjunto == NULL || obtener_data(conjunto) == NULL) {
		return -1;  /* Si no existe o est? vac?o, el nombre no puede encontrarse */
	}
	/* Comienza el recorrido desde el primer elemento real */
	actual = obtener_data(conjunto);
	indice = 0;
	/* Recorre todos los elementos del conjunto */
	while (actual != NULL) {
		Tdata dato;     /* el nodo que contiene el string */
		dato = obtener_data(actual);
		/* Verifica que el dato exista y sea un string */
		if (dato != NULL && return_type(dato) == STR) {
			/* Compara el nombre almacenado con el nombre buscado */
			if (compare_str(obtener_string(dato), nombre) == 1) {
				/* Devuelve la posici?n donde se encontr? */
				return indice;
			}
		}
		/* Avanza al siguiente elemento de la lista */
		actual = obtener_next(actual);
		indice++;
	}
	/* El nombre no se encontr? en el conjunto */
	return -1;
}

/* ========== Conversi?n nombre -> ?ndice usando str ========== */
int estado_a_indice(AF aut, str nombre) {
	/* Convierte el nombre de un estado (str) a su ?ndice num?rico (-1 si no existe) */
	if (aut == NULL) {
		return -1;
	}
	return buscar_indice_en_conjunto(aut->Q, nombre);
}

int simbolo_a_indice(AF aut, str nombre) {
	/* Convierte el nombre de un s?mbolo (str) a su ?ndice num?rico (-1 si no existe) */
	if (aut == NULL) {
		return -1;
	}
	return buscar_indice_en_conjunto(aut->Sigma, nombre);
}

/* ========== Agregar transici?n (acceso O(1) mediante matriz) ========== */
void agregar_transicion(AF aut, str from, str symbol, str to) {
	int i, j, k;        /* ?ndices de origen, s?mbolo y destino */
	Tdata dest_node;    /* nodo para almacenar el destino */
	
	/* Convertir nombres a ?ndices */
	i = estado_a_indice(aut, from);
	j = simbolo_a_indice(aut, symbol);
	k = estado_a_indice(aut, to);
	if (i == -1 || j == -1 || k == -1) {
		return;
	}
	/* Asegurar que la matriz est? inicializada (por si se llama antes de terminar la carga) */
	if (aut->delta == NULL) {
		inicializar_matriz(aut);
	}
	dest_node = create_str();
	dest_node->string = copy_str(to);
	
	insert_set(&(aut->delta[i][j]), dest_node);
}

/* ========== Consulta de transici?n por nombre (usa str) ========== */
Tdata transicion_por_nombre(AF aut, str estado, str simbolo) {
	int i, j;   /* ?ndices del estado y s?mbolo */
	
	/* Devuelve el conjunto de estados destino para el par (estado, s?mbolo) dado como str */
	i = estado_a_indice(aut, estado);
	j = simbolo_a_indice(aut, simbolo);
	if (i == -1 || j == -1) {
		return create_set();
	}
	if (aut->delta == NULL) {
		return create_set();
	}
	return aut->delta[i][j];
}

/* ========== Consulta por ?ndice (acceso directo O(1)) ========== */
Tdata transicion_por_indice(AF aut, int from, int symbol) {
	/* Devuelve el conjunto de estados destino (como Tdata) para un par de ?ndices */
	if (aut == NULL || aut->delta == NULL) {
		return create_set();
	}
	if (from < 0 || from >= aut->num_estados || symbol < 0 || symbol >= aut->num_simbolos) {
		return create_set();
	}
	return aut->delta[from][symbol];
}

/* ========== Carga desde archivo ========== */
void leer_conjunto_desde_archivo(FILE* f, Tdata* conjunto) {
	char linea[1000];   /* buffer para la l?nea */
	char* token;        /* token actual */
	Tdata nodo;         /* nodo para insertar */
	
	/* Lee una l?nea del archivo, la divide por comas e inserta cada token en el conjunto */
	if (fgets(linea, sizeof(linea), f) == NULL) {
		return;
	}
	token = strtok(linea, ",\n");
	while (token != NULL) {
		while (*token == ' ') {
			token++;
		}
		nodo = create_str();
		nodo->string = load2(token);
		insert_set(conjunto, nodo);
		token = strtok(NULL, ",\n");
	}
}

void procesar_linea_transiciones(AF aut, char* linea) {
	char* token;                /* token de cada transici?n (separada por ';') */
	char origen[100], simbolo[100], destino[100];   /* campos de una transici?n */
	str from, sym, to;          /* strings temporales */
	
	/* Procesa la l?nea de transiciones separando por ';' y luego cada transici?n por coma */
	token = strtok(linea, ";\n");
	while (token != NULL) {
		if (sscanf(token, "%[^,],%[^,],%s", origen, simbolo, destino) == 3) {
			/* Convertir a str para llamar a agregar_transicion */
			from = load2(origen);
			sym = load2(simbolo);
			to = load2(destino);
			agregar_transicion(aut, from, sym, to);
			free_str(from);
			free_str(sym);
			free_str(to);
		} else {
			printf("Advertencia: transici?n mal formada: %s\n", token);
		}
		token = strtok(NULL, ";\n");
	}
}

int cargar_automata_desde_archivo(AF aut, const char* ruta) {
	FILE* f;                /* puntero al archivo */
	char linea[1000];       /* buffer para l?neas */
	char* p;                /* puntero auxiliar para estado inicial */
	str inicial_str;        /* string temporal para estado inicial */
	int resultado;          /* valor de retorno */
	
	/* Lee un archivo de texto y carga los datos en el aut?mata.
	Devuelve 1 si ?xito, 0 si error. */
	f = fopen(ruta, "r");
	if (f == NULL) {
		printf("Error al abrir el archivo %s\n", ruta);
		return 0;
	}
	printf("Archivo abierto correctamente.\n");
	
	/* ----- Estados Q ----- */
	leer_conjunto_desde_archivo(f, &(aut->Q));
	
	/* ----- Alfabeto Sigma ----- */
	leer_conjunto_desde_archivo(f, &(aut->Sigma));
	
	/* ----- Inicializar la matriz ahora que conocemos Q y Sigma ----- */
	inicializar_matriz(aut);
	
	/* ----- L?nea de transiciones (formato especial) ----- */
	if (fgets(linea, sizeof(linea), f) == NULL) {
		fclose(f);
		return 0;
	}
	printf("Linea transiciones: %s", linea);
	procesar_linea_transiciones(aut, linea);
	
	/* ----- Estado inicial ----- */
	if (fgets(linea, sizeof(linea), f) == NULL) {
		fclose(f);
		return 0;
	}
	printf("Linea estado inicial: %s", linea);
	linea[strcspn(linea, "\n")] = '\0';
	p = linea;
	while (*p == ' ') {  /* Ignorar espacios iniciales */
		p++;
	}
	inicial_str = load2(p);
	aut->q0 = estado_a_indice(aut, inicial_str);
	free_str(inicial_str);
	if (aut->q0 == -1) {
		aut->q0 = 0;
	}
	
	/* ----- Estados finales F ----- */
	leer_conjunto_desde_archivo(f, &(aut->F));
	
	fclose(f);
	printf("Carga completada exitosamente.\n");
	resultado = 1;
	return resultado;
}

/* ========== Determinismo (recorre la matriz) ========== */
int esDeterminista(AF aut) {
	int i, j;       /* ?ndices para recorrer la matriz */
	Tdata set;      /* conjunto de destinos en cada celda */
	
	/* Determina si el aut?mata es determinista (1) o no determinista (0) */
	if (aut == NULL || aut->delta == NULL) {
		return 0;
	}
	for (i = 0; i < aut->num_estados; i++) {
		for (j = 0; j < aut->num_simbolos; j++) {
			set = aut->delta[i][j];
			/* Si el conjunto tiene m?s de un elemento => no determinista */
			if (set != NULL && obtener_data(set) != NULL && obtener_next(obtener_data(set)) != NULL) {
				return 0;   /* m?ltiples destinos => no determinista */
			}
		}
	}
	return 1;
}

void mostrar_automata(AF aut) {
	int i, j;
	int determinista;
	str e, s;
	
	if (aut == NULL || aut->delta == NULL) {
		printf("Aut?mata nulo o sin inicializar\n");
		return;
	}
	determinista = esDeterminista(aut);
	if (determinista == 1) {
		printf("\n========== AUTOMATA AFD ==========");
	} else {
		printf("\n========== AUTOMATA AFND ==========");
	}
	printf("\nConjunto de estados Q:\n");
	mostrarArbol(aut->Q);
	printf("\nAlfabeto Sigma:\n");
	mostrarArbol(aut->Sigma);
	printf("\nMatriz de transiciones (delta[estado][s?mbolo]):\n\n");
	
	/* L?nea superior */
	printf("+-------+");
	for (j = 0; j < aut->num_simbolos; j++) {
		printf("-----------+");
	}
	printf("\n|       |");
	for (j = 0; j < aut->num_simbolos; j++) {
		s = indice_a_str(aut->Sigma, j);
		printf("    ");
		if (s != NULL) print(s); else printf("?");
		printf("    |");
	}
	printf("\n+-------+");
	for (j = 0; j < aut->num_simbolos; j++) {
		printf("-----------+");
	}
	printf("\n");
	
	for (i = 0; i < aut->num_estados; i++) {
		e = indice_a_str(aut->Q, i);
		printf("| ");
		if (e != NULL) print(e); else printf("?");
		printf("     |");
		for (j = 0; j < aut->num_simbolos; j++) {
			printf(" ");
			mostrarArbol(aut->delta[i][j]);
			printf("  |");
		}
		printf("\n");
	}
	printf("+-------+");
	for (j = 0; j < aut->num_simbolos; j++) {
		printf("-----------+");
	}
	printf("\n");
	
	printf("\nEstado inicial: ");
	e = indice_a_str(aut->Q, aut->q0);
	if (e != NULL) print(e); else printf("?");
	printf("\nEstados finales:\n");
	mostrarArbol(aut->F);
	printf("\n");
}

int validar_cadena(AF aut, str w) {
    if (aut == NULL || w == NULL) return 0;

    // 1. Inicializar el conjunto de estados actuales con el estado inicial q0
    Tdata estados_actuales = create_set();
    
    // Obtenemos el nombre (str) del estado inicial
    str str_q0 = indice_a_str(aut->Q, aut->q0); 
    if (str_q0 != NULL) {
        // Envolvemos el str en un nodo Tdata para poder insertarlo en el SET
        Tdata nodo_q0 = create_str();
        nodo_q0->string = copy_str(str_q0); 
        insert_set(&estados_actuales, nodo_q0);
        free_ast(nodo_q0); // Liberamos el envoltorio temporal (insert_set hace su propia copia)
    }

    str aux_cadena = w; // Puntero para recorrer la cadena de entrada

    // 2. Procesar la cadena w símbolo por símbolo
    while (aux_cadena != NULL) {
        char c = aux_cadena->data;

        // Convertimos char a str temporal
        str temp_sym = create_nodo(c);
        int sym_idx = simbolo_a_indice(aut, temp_sym);
        free_str(temp_sym); // Limpieza inmediata

        if (sym_idx == -1) {
            printf("\nSímbolo '%c' no pertenece a Sigma. Cadena rechazada.\n", c);
            free_ast(estados_actuales);
            return 0; // Rechazo inmediato
        }

        Tdata proximos_estados = create_set();
        Tdata iterador = obtener_data(estados_actuales);

        // 3. Evaluar transiciones
        while (iterador != NULL) {
            Tdata estado_nodo = obtener_data(iterador);

            if (estado_nodo != NULL && return_type(estado_nodo) == STR) {
                // Sacamos el string y buscamos su índice
                str nombre_estado = obtener_string(estado_nodo);
                int q_actual = estado_a_indice(aut, nombre_estado);

                if (q_actual != -1) {
                    Tdata destino = transicion_por_indice(aut, q_actual, sym_idx);

                    // Si hay estados de destino, iteramos sobre ellos para insertarlos
                    if (destino != NULL && obtener_data(destino) != NULL) {
                        Tdata it_dest = obtener_data(destino);
                        while (it_dest != NULL) {
                            if (obtener_data(it_dest) != NULL) {
                                insert_set(&proximos_estados, obtener_data(it_dest));
                            }
                            it_dest = obtener_next(it_dest);
                        }
                    }
                }
            }
            iterador = obtener_next(iterador);
        }

        // Actualizar estados actuales y limpiar memoria del paso anterior
        free_ast(estados_actuales);
        estados_actuales = proximos_estados;

        // Optimización: Si el conjunto quedó vacío, rechazo temprano
        if (esvacio(estados_actuales) == 1) {
            free_ast(estados_actuales);
            return 0;
        }

        aux_cadena = aux_cadena->next;
    }

    // 4. Verificación final: ¿Hay algún estado actual dentro del conjunto Final F?
    int aceptada = 0;
    Tdata iterador_final = obtener_data(estados_actuales);

    while (iterador_final != NULL) {
        Tdata estado_nodo = obtener_data(iterador_final);
        if (estado_nodo != NULL && belongs(aut->F, estado_nodo) == 1) {
            aceptada = 1; // Un solo camino exitoso
            break;
        }
        iterador_final = obtener_next(iterador_final);
    }

    free_ast(estados_actuales); // Limpieza final de memoria
    return aceptada;
}
