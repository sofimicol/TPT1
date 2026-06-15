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
int obtener_num_simbolos(AF aut);
int obtener_num_estados(AF aut);
void actualizar_contadores(AF aut);
AF create_automata(void) {
	AF nuevo;   /* declaracion al inicio */
	/* Crea y devuelve un automata vacio */
	nuevo = (AF)malloc(sizeof(Automata));
	if (nuevo != NULL) {
		nuevo->Q = create_set();
		nuevo->Sigma = create_set();
		nuevo->delta = NULL;
		nuevo->num_estados = 0;
		nuevo->num_simbolos = 0;
		nuevo->q0 = -1;
		nuevo->F = create_set();
	}
	return nuevo;
}

void inicializar_matriz(AF aut) {
	int i, j;
	/*Matriz no inicicializada */
	if (aut->delta == NULL){
		if (aut->num_estados != 0 || aut->num_simbolos != 0){ 
			/*Reservar array de punteros a filas (punteros a Tdata)*/
			aut->delta = (Tdata**)malloc(aut->num_estados * sizeof(Tdata*)); /*Reserva espacio para un array de num_estados punteros a Tdata. Cada puntero ocupará sizeof(Tdata*). Esto crea el array de filas (cada fila es un puntero).*/
			for (i = 0; i < aut->num_estados; i++) {
				/*Reservar cada fila: array de Tdata (punteros a conjuntos)*/
				aut->delta[i] = (Tdata*)malloc(aut->num_simbolos * sizeof(Tdata)); /*reserva espacio para un array de num_simbolos elementos de tipo Tdata. Esto crea una fila con tantas columnas como símbolos.*/
				for (j = 0; j < aut->num_simbolos; j++) {
					aut->delta[i][j] = create_set();   // conjunto vacío
				}
			}
		}
	}
}/*Se reservo memoria de esta manera porque malloc pide el número total de bytes. Para un array de N elementos, necesitamos N * sizeof(elemento).*/

/* ========== Destruccion de la matriz ========== */
void destruir_matriz(AF aut) {
	int i, j;      
	if (aut->delta != NULL) {
	
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
}
/* ========== Liberacion completa del automata ========== */
void free_automata(AF aut) {
	if (aut != NULL) {
		destruir_matriz(aut);
		free_ast(aut->Q);
		free_ast(aut->Sigma);
		free_ast(aut->F);
		free(aut);
	}
}

/* ========== Funcion auxiliar: busca un nombre (str) en un conjunto de Tdata ========== */
int buscar_indice_en_conjunto(Tdata conjunto, str nombre) {
	Tdata actual;   /* puntero para recorrer la lista de elementos */
	int indice;      /* indice asociado al elemento actual */
	
	/* Verifica que el conjunto exista y no esta vacio */
	if (conjunto == NULL || obtener_data(conjunto) == NULL) {
		return -1;  /* Si no existe o esta vacio, el nombre no puede encontrarse */
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
				/* Devuelve la posicion donde se encontro */
				return indice;
			}
		}
		/* Avanza al siguiente elemento de la lista */
		actual = obtener_next(actual);
		indice++;
	}
	/* El nombre no se encontro en el conjunto */
	return -1;
}


int estado_a_indice(AF aut, str nombre) {
	/* Convierte el nombre de un estado (str) a su indice numerico (-1 si no existe) */
	if (aut == NULL) {
		return -1;
	}
	return buscar_indice_en_conjunto(aut->Q, nombre);
}

int simbolo_a_indice(AF aut, str nombre) {
	if (aut == NULL) {
		return -1;
	}
	return buscar_indice_en_conjunto(aut->Sigma, nombre);
}

/* ========== Agregar transicion (acceso O(1) mediante matriz) ========== */
void agregar_transicion(AF aut, str from, str symbol, str to) {
	int i, j, k;        /* indices de origen, simbolo y destino */
	Tdata dest_node;    /* nodo para almacenar el destino */
	
	/* Convertir nombres a indices */
	i = estado_a_indice(aut, from);
	j = simbolo_a_indice(aut, symbol);
	k = estado_a_indice(aut, to);
	if (i != -1 || j != -1 || k != -1) {
		if (aut->delta == NULL) {
			inicializar_matriz(aut);
		}
		dest_node = create_str();
		dest_node->string = copy_str(to);
		
		insert_set(&(aut->delta[i][j]), dest_node);
	}
}

Tdata transicion_por_nombre(AF aut, str estado, str simbolo) {
	int i, j;   /* indices del estado y simbolo */
	/* Devuelve el conjunto de estados destino para el par (estado, simbolo) dado como str */
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

/* ========== Consulta por indice (acceso directo O(1)) ========== */
Tdata transicion_por_indice(AF aut, int from, int symbol) {
	/* Devuelve el conjunto de estados destino (como Tdata) para un par de indices */
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
	
	/* Lee una linea del archivo, la divide por comas e inserta cada token en el conjunto */
	if (fgets(linea, sizeof(linea), f) != NULL) {
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
}

void procesar_linea_transiciones(AF aut, char* linea) {
	char* token;                /* token de cada transici?n (separada por ';') */
	char origen[100], simbolo[100], destino[100];   /* campos de una transicion */
	str from, sym, to;          /* strings temporales */
	
	/* Procesa la linea de transiciones separando por ';' y luego cada transicion por coma */
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
	actualizar_contadores(aut); 
	/* ----- Inicializar la matriz ahora que conocemos Q y Sigma ----- */
	inicializar_matriz(aut);
	
	/* ----- Linea de transiciones (formato especial) ----- */
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
	int i, j;    
	Tdata set;     
	/* Determina si el automata es determinista (1) o no determinista (0) */
	if (aut == NULL || aut->delta == NULL) {
		return 0;
	}
	for (i = 0; i < aut->num_estados; i++) {
		for (j = 0; j < aut->num_simbolos; j++) {
			set = aut->delta[i][j];
			/* Si el conjunto tiene mas de un elemento => no determinista */
			if (set != NULL && obtener_data(set) != NULL && obtener_next(obtener_data(set)) != NULL) {
				return 0;   /* multiples destinos => no determinista */
			}
		}
	}
	return 1;
}

void mostrar_automata(AF aut) {
	int i, j;
	int determinista;
	str e, s;
	
	if (aut != NULL || aut->delta != NULL) {
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
		printf("\nMatriz de transiciones (delta[estado][simbolo]):\n\n");
		printf("+-------+");
		for (j = 0; j < aut->num_simbolos; j++) {
			printf("-----------+");
		}
		printf("\n|       |");
		for (j = 0; j < aut->num_simbolos; j++) {
			s = indice_a_str(aut->Sigma, j);
			printf("    ");
			if (s != NULL) {
				print(s);
			} else {
				printf("?");
			}
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
			if (e != NULL) {
				print(e);
			} else {
				printf("?");
			}
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
		if (e != NULL) {
			print(e);
		} else {
			printf("?");
		}
		printf("\nEstados finales:\n");
		mostrarArbol(aut->F);
		printf("\n");
	}else{
		printf("Aut?mata nulo o sin inicializar\n");}
}

/* ========== Actualizar contadores a partir de Q y Sigma ========== */
void actualizar_contadores(AF aut) {
	Tdata aux;
	aux = obtener_data(aut->Q);
	while (aux != NULL) {
		aut->num_estados++;
		aux = obtener_next(aux);
	}
	aux = obtener_data(aut->Sigma);
	while (aux != NULL) {
		aut->num_simbolos++;
		aux = obtener_next(aux);
	}
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

/* ==================================================================== */
/* ========== CONVERSIÓN DE AFND A AFD (MÉTODO DE SUBCONJUNTOS) ======= */
/* ==================================================================== */

#define MAX_DFA_STATES 100

// Estructuras de apoyo para el algoritmo
typedef struct {
    Tdata conjunto_nfa;  // Conjunto de estados del NFA que forman este estado del DFA
    char nombre[10];     // Nombre asignado (A, B, C...)
    int procesado;       // Bandera: 0 = sin procesar, 1 = procesado
} EstadoDFA;

typedef struct {
    char origen[10];
    char simbolo[100];
    char destino[10];
} TransicionDFA;

/* Función auxiliar para contar elementos de un conjunto (para comparar tamaños) */
int contar_elementos(Tdata conjunto) {
    if (conjunto == NULL || obtener_data(conjunto) == NULL) return 0;
    int c = 0;
    Tdata it = obtener_data(conjunto);
    while (it != NULL) {
        c++;
        it = obtener_next(it);
    }
    return c;
}

/* Función auxiliar para comparar si dos conjuntos de estados son exactamente iguales */
int conjuntos_iguales(Tdata c1, Tdata c2) {
    if (contar_elementos(c1) != contar_elementos(c2)) return 0;
    Tdata it1 = obtener_data(c1);
    while (it1 != NULL) {
        Tdata nodo1 = obtener_data(it1);
        // belongs() ya existe en tu código (usada en validar_cadena)
        if (nodo1 != NULL && belongs(c2, nodo1) == 0) {
            return 0; // Un elemento de c1 no está en c2
        }
        it1 = obtener_next(it1);
    }
    return 1;
}

/* Función auxiliar para extraer el texto de tu TAD 'str' a un char[] estándar de C */
void extraer_cadena(str s, char* buffer) {
    int i = 0;
    str aux = s;
    while (aux != NULL) {
        buffer[i++] = aux->data; // Extraemos cada caracter de la lista enlazada
        aux = aux->next;
    }
    buffer[i] = '\0';
}

/* Calcula la UNION de destinos a partir de un conjunto de estados origen */
Tdata obtener_destinos_dfa(AF afnd, Tdata conjunto_origen, str simbolo) {
    Tdata resultado = create_set();
    Tdata iterador = obtener_data(conjunto_origen);

    while (iterador != NULL) {
        Tdata estado_nodo = obtener_data(iterador);
        if (estado_nodo != NULL && return_type(estado_nodo) == STR) {
            str nombre_estado = obtener_string(estado_nodo);
            int q_actual = estado_a_indice(afnd, nombre_estado);
            int sym_idx = simbolo_a_indice(afnd, simbolo);

            if (q_actual != -1 && sym_idx != -1) {
                Tdata destino = transicion_por_indice(afnd, q_actual, sym_idx);
                if (destino != NULL && obtener_data(destino) != NULL) {
                    Tdata it_dest = obtener_data(destino);
                    while (it_dest != NULL) {
                        if (obtener_data(it_dest) != NULL) {
                            // insert_set se encarga de no duplicar
                            insert_set(&resultado, obtener_data(it_dest));
                        }
                        it_dest = obtener_next(it_dest);
                    }
                }
            }
        }
        iterador = obtener_next(iterador);
    }
    return resultado;
}

/* FUNCIÓN PRINCIPAL DE CONVERSIÓN */
AF convertir_AFND_a_AFD(AF afnd) {
    if (afnd == NULL) return NULL;

    EstadoDFA estados[MAX_DFA_STATES];
    int num_estados = 0;
    TransicionDFA trans[1000];
    int num_trans = 0;

    // 1. Crear el primer estado del DFA: {q0}
    Tdata q0_set = create_set();
    str str_q0 = indice_a_str(afnd->Q, afnd->q0);
    if (str_q0 != NULL) {
        Tdata nodo_q0 = create_str();
        nodo_q0->string = copy_str(str_q0);
        insert_set(&q0_set, nodo_q0);
        free_ast(nodo_q0);
    }

    estados[0].conjunto_nfa = q0_set;
    sprintf(estados[0].nombre, "A"); // Renombramos al Estado Inicial como "A"
    estados[0].procesado = 0;
    num_estados++;

    // 2. Procesamiento de subconjuntos (El corazón del algoritmo)
    int procesando = 1;
    while (procesando) {
        procesando = 0;
        int act = -1;
        // Buscar el primer estado sin procesar
        for (int i = 0; i < num_estados; i++) {
            if (estados[i].procesado == 0) {
                act = i;
                procesando = 1;
                break;
            }
        }
        if (!procesando) break;

        estados[act].procesado = 1;

        // Evaluar transiciones con cada símbolo del alfabeto
        Tdata it_sym = obtener_data(afnd->Sigma);
        while (it_sym != NULL) {
            Tdata nodo_sym = obtener_data(it_sym);
            str sym_str = obtener_string(nodo_sym);

            // Obtener la unión de todos los destinos para este símbolo
            Tdata destinos = obtener_destinos_dfa(afnd, estados[act].conjunto_nfa, sym_str);

            if (esvacio(destinos) == 0) {
                // Verificar si ya descubrimos este conjunto antes
                int idx_existente = -1;
                for (int k = 0; k < num_estados; k++) {
                    if (conjuntos_iguales(estados[k].conjunto_nfa, destinos)) {
                        idx_existente = k;
                        break;
                    }
                }

                int idx_dest;
                if (idx_existente == -1) {
                    // Descubrimos un nuevo estado!
                    estados[num_estados].conjunto_nfa = destinos;
                    // Generar nombre: B, C, D... Si pasamos Z, usamos S26, S27...
                    if (num_estados < 26) {
                        sprintf(estados[num_estados].nombre, "%c", 'A' + num_estados);
                    } else {
                        sprintf(estados[num_estados].nombre, "S%d", num_estados);
                    }
                    estados[num_estados].procesado = 0;
                    idx_dest = num_estados;
                    num_estados++;
                } else {
                    idx_dest = idx_existente;
                    free_ast(destinos); // Ya existía, liberamos la memoria
                }

                // Guardar la transición para construir la matriz luego
                strcpy(trans[num_trans].origen, estados[act].nombre);
                strcpy(trans[num_trans].destino, estados[idx_dest].nombre);
                extraer_cadena(sym_str, trans[num_trans].simbolo);
                num_trans++;
            } else {
                free_ast(destinos); // Transición vacía (sumidero implícito)
            }

            it_sym = obtener_next(it_sym);
        }
    }

    // 3. Ensamblar el nuevo Automata (AFD)
    AF afd = create_automata();

    // 3.1 Copiar alfabeto
    Tdata it_sym = obtener_data(afnd->Sigma);
    while (it_sym != NULL) {
        Tdata copia_sym = create_str();
        copia_sym->string = copy_str(obtener_string(obtener_data(it_sym)));
        insert_set(&(afd->Sigma), copia_sym);
        free_ast(copia_sym);
        it_sym = obtener_next(it_sym);
    }

    // 3.2 Insertar Estados Q y determinar Estados Finales F
    for (int i = 0; i < num_estados; i++) {
        str nombre_str = load2(estados[i].nombre);
        Tdata nodo_estado = create_str();
        nodo_estado->string = nombre_str;
        insert_set(&(afd->Q), nodo_estado);

        // Si el subconjunto contiene algún estado final del AFND, entonces este nuevo estado es FINAL
        int es_final = 0;
        Tdata it_nfa = obtener_data(estados[i].conjunto_nfa);
        while (it_nfa != NULL) {
            Tdata estado_nfa = obtener_data(it_nfa);
            if (estado_nfa != NULL && belongs(afnd->F, estado_nfa)) {
                es_final = 1;
                break; // Un solo estado final es suficiente
            }
            it_nfa = obtener_next(it_nfa);
        }

        if (es_final) {
            Tdata nodo_final = create_str();
            nodo_final->string = copy_str(nombre_str);
            insert_set(&(afd->F), nodo_final);
            free_ast(nodo_final);
        }
        free_ast(nodo_estado);
    }

    // Asignar el estado inicial explícitamente (siempre es "A")
    str str_A = load2("A");
    afd->q0 = estado_a_indice(afd, str_A);
    free_str(str_A);

    // 3.3 Inicializar matriz e inyectar todas las transiciones recopiladas
    inicializar_matriz(afd);
    for (int i = 0; i < num_trans; i++) {
        str o = load2(trans[i].origen);
        str s = load2(trans[i].simbolo);
        str d = load2(trans[i].destino);
        agregar_transicion(afd, o, s, d);
        free_str(o);
        free_str(s);
        free_str(d);
    }

    // Limpieza de memoria temporal
    for (int i = 0; i < num_estados; i++) {
        free_ast(estados[i].conjunto_nfa);
    }

    return afd;
}
