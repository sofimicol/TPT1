#include <stdio.h>
#include <stdlib.h>
#include "TAD_ast.h"
#include "TAD_set.h"
#include "TAD_list.h"
#include "TAD_str.h"
#include "TAD_afnd.h" 

void agregar_elemento(Tdata* conjunto, const char* valor) { /* Facilita la carga estática de estados y símbolos. */
    str cadena = load2(valor);         
    Tdata nodo = create_str();         
    nodo->string = cadena;
    insert_set(conjunto, nodo);        
    free_ast(nodo);                   
}

int main() {
    printf("=== INICIANDO PRUEBA DEL CONVERSOR AFND A AFD ===\n\n");

    // 1. Crear el autómata vacío
    AF nfa = create_automata();

    // 2. Cargar conjunto de estados Q = {q0, q1, q2}
    agregar_elemento(&(nfa->Q), "q0");
    agregar_elemento(&(nfa->Q), "q1");
    agregar_elemento(&(nfa->Q), "q2");

    // 3. Cargar alfabeto Sigma = {0, 1}
    agregar_elemento(&(nfa->Sigma), "0");
    agregar_elemento(&(nfa->Sigma), "1");

    // Actualizamos los contadores antes de usar la matriz
    actualizar_contadores(nfa);
    
    // 4. Configurar estado inicial (q0) y estado final F = {q2}
    str inicio = load2("q0");
    nfa->q0 = estado_a_indice(nfa, inicio);
    free_str(inicio);
    
    agregar_elemento(&(nfa->F), "q2");

    // 5. Inicializar la matriz delta ahora que sabemos las dimensiones
    inicializar_matriz(nfa);

    // 6. Cargar transiciones
    str q0 = load2("q0");
    str q1 = load2("q1");
    str q2 = load2("q2");
    str sym_0 = load2("0");
    str sym_1 = load2("1");

    // delta(q0, 0) = {q0, q1} 
    agregar_transicion(nfa, q0, sym_0, q0);
    agregar_transicion(nfa, q0, sym_0, q1);
    
    // delta(q0, 1) = {q0}
    agregar_transicion(nfa, q0, sym_1, q0);
    
    // delta(q1, 1) = {q2}
    agregar_transicion(nfa, q1, sym_1, q2);

    // Liberar los strings temporales que usamos para cargar
    free_str(q0); free_str(q1); free_str(q2);
    free_str(sym_0); free_str(sym_1);

    /* ========================================================================= */
    /* FASE DE IMPRESIÓN Y CONVERSIÓN                                            */
    /* ========================================================================= */
    
    printf("--- ESTRUCTURA DEL AFND ORIGINAL ---\n");
    mostrar_automata(nfa);

    printf("\n\n=== EJECUTANDO ALGORITMO DE SUBCONJUNTOS ===\n");
    AF dfa = convertir_AFND_a_AFD(nfa);

    if (dfa != NULL) {
        printf("\n--- ESTRUCTURA DEL AFD RESULTANTE ---\n");
        mostrar_automata(dfa);
        
    } else {
        printf("\nError: La conversión falló y devolvió NULL.\n");
    }

    // Limpieza de memoria general al cerrar el programa
    free_automata(nfa);
    if (dfa != NULL) {
        free_automata(dfa);
    }

    printf("\n=== PRUEBA FINALIZADA CON ÉXITO ===\n");
    return 0;
}
