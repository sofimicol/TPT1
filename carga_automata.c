#include "TAD_afnd.h"


AF automaton_from_string(str);
Tdata build_set(str *);
Tdata build_list(str *);
void load_transitions_from_digested_delta(AF, Tdata);



int main() {
  automaton_from_string(load2("{{q0 q1 q2 q3 q4} {a b} {[q0 a {q1 q2 q3}] [q1 a {q2}] [q2 a {q3}] [q3 a {q3}] [q0 b {q4}] [q1 b {q4}] [q2 b {q4}] [q3 b {q4}] [q4 b {q4}] [q4 a {q4}]} q0 {q3}}"));
  return 0;
}

Tdata build_set(str * entrada){
  Tdata set, elemento;
  str str_caracter, aux;
  int processing_set;

  set = create_set();
  processing_set = 1;

  while(processing_set) {
    str_caracter = pop(entrada);
    if(str_caracter->data == '{'){
      elemento = build_set(entrada);
      insert_set(&set, copy_ast(elemento));

    } else if (str_caracter->data == '['){
      elemento = build_list(entrada);
      insert_set(&set, copy_ast(elemento));

    } else {
      elemento = create_str();
      while(str_caracter->data != ' ' && str_caracter->data != '}'){
        aux = copy_str(elemento->string);
        elemento -> string = concat(aux, str_caracter);
        free_str(aux);
        free_str(str_caracter);
        str_caracter = pop(entrada);
      }

      if(str_caracter->data == '}'){
        processing_set = 0;
      }

      if(elemento->string != NULL){
        insert_set(&set, copy_ast(elemento));
      }
    }
    free_ast(elemento);
  }

  return set;
}

Tdata build_list(str * entrada){
  Tdata list, elemento;
  str str_caracter, aux;
  int processing_list;

  processing_list = 1;
  list = create_list();

  while(processing_list) {
    str_caracter = pop(entrada);
    if(str_caracter->data == '{'){
      elemento = build_set(entrada);
      append(&list, copy_ast(elemento));

    } else if (str_caracter->data == '['){
      elemento = build_list(entrada);
      append(&list, copy_ast(elemento));
    } else {
      elemento = create_str();
      while(str_caracter->data != ' ' && str_caracter->data != ']'){
        aux = copy_str(elemento->string);
        elemento -> string = concat(aux, str_caracter);
        free_str(aux);
        free_str(str_caracter);
        str_caracter = pop(entrada);
      }
      if(str_caracter->data == ']'){
        processing_list = 0;
      }
      if(elemento->string != NULL){
        append(&list, copy_ast(elemento));
      }
    }
    free_ast(elemento);
  }

  return list;
}

void load_transitions_from_digested_delta(AF automata, Tdata digested_delta){
  Tdata from, symbol, to, aux1, aux2;

  aux1 = obtener_data(digested_delta);

  while(aux1 != NULL){

    from = obtener_data(obtener_data(obtener_data(aux1)));
    symbol = obtener_data(obtener_next(obtener_data(obtener_data(aux1))));
    aux2 = obtener_data(obtener_data(obtener_next(obtener_next(obtener_data(obtener_data(aux1))))));

    while(aux2 != NULL){
      to = obtener_data(aux2);
      agregar_transicion(automata, copy_str(obtener_string(from)), copy_str(obtener_string(symbol)), copy_str(obtener_string(to)));
      aux2 = obtener_next(aux2);
    }

    aux1 = obtener_next(aux1);
  }
}

AF automaton_from_string(str entrada){
  AF automata;
  str cadena, str_caracter;
  Tdata set, Q, sigma, delta, q0, F;
  cadena = copy_str(entrada);
  str_caracter = pop(&cadena);

  if(str_caracter == NULL) {
    printf("AF Inválido: Cadena Vacía;");
    return NULL;
  } else if (str_caracter->data != '{'){
    printf("AF Inválido: Entrada debe ser un set;");
    return NULL;
  } else {
    set = build_set(&cadena);
  }

  free_str(cadena);

  Q = obtener_data(obtener_data(set));
  sigma = obtener_data(obtener_next(obtener_data(set)));
  delta = obtener_data(obtener_next(obtener_next(obtener_data(set))));
  q0 = obtener_data(obtener_next(obtener_next(obtener_next(obtener_data(set)))));
  F = obtener_data(obtener_next(obtener_next(obtener_next(obtener_next(obtener_data(set))))));

  automata = create_automata();

  automata->Q = Q;
  automata->Sigma = sigma;
  automata->q0 = estado_a_indice(automata, obtener_string(q0));
  automata->F = F;

  load_transitions_from_digested_delta(automata, delta);

  mostrar_automata(automata);

  free_ast(q0);
  free_ast(delta);
  return automata;
}

