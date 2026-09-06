#include "TAD_afnd.h"


AF automaton_from_string(str);
Tdata build_set(str *);
Tdata build_list(str *);


int main() {
  // automaton_from_string(load2("{{q0 q1 q2 q3}{a b}{[q0 a q1][q1 a q2][q2 a q3][q0 b q4][q1 b q4][q2 b q4][q3 b q4][q4 b q4][q4 a q4]}q0{q3}}"));
  automaton_from_string(load2("{[[q0 a q4]]}"));
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
    } else if (str_caracter->data == '['){
      elemento = build_list(entrada);
    } else {
      elemento = create_str();
      while(str_caracter->data != ' ' && str_caracter->data != '}'){
        aux = copy_str(elemento->string);
        elemento -> string = concat(aux, str_caracter);
        free_str(aux);
        free_str(str_caracter);
        str_caracter = pop(entrada);
      }

      if(str_caracter->data == '}'){ processing_set = 0; }
    }
    insert_set(&set, copy_ast(elemento));
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
    } else if (str_caracter->data == '['){
      elemento = build_list(entrada);
    } else {
      elemento = create_str();
      while(str_caracter->data != ' ' && str_caracter->data != ']'){
        aux = copy_str(elemento->string);
        elemento -> string = concat(aux, str_caracter);
        free_str(aux);
        free_str(str_caracter);
        str_caracter = pop(entrada);
      }
      if(str_caracter->data == ']'){ processing_list = 0; }
    }
    append(&list, copy_ast(elemento));
    free_ast(elemento);
  }

  return list;
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
  mostrarArbol(set);

  // Q = obtener_data(set);
  // sigma = obtener_data(obtener_next(set));
  // delta = obtener_data(obtener_next(obtener_next(set)));
  // q0 = obtener_data(obtener_next(obtener_next(obtener_next(set))));
  // F = obtener_data(obtener_next(obtener_next(obtener_next(obtener_next(set)))));

  automata = create_automata();

  return automata;
}
