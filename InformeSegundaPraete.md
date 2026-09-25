## 3.3 Análisis Léxico
### [PONER AQUÍ Definición de Análisis Léxico]

Un analizador léxico realiza, técnicamente, un análisis sintáctico al nivel más bajo de la estructura del programa. Su función principal consiste en reconocer agrupaciones lógicas de caracteres (al identificar subcadenas) mediante la coincidencia de patrones sobre el código de entrada. Dichas agrupaciones se denominan lexemas, mientras que las categorías sintácticas o códigos internos asignados a ellas corresponden a los tokens.

### [PONER AQUÍ Descripción de las tareas de un analizador léxico]

Las tareas fundamentales de un analizador léxico (scanner) incluyen:

* Identificar y agrupar los caracteres en lexemas válidos basándose en patrones regulares.

* Generar y entregar secuencialmente los tokens al analizador sintáctico (parser) cada vez que este los requiere para la construcción del árbol sintáctico.

* Filtrar y descartar elementos que carecen de significado estructural para el compilador o intérprete, tales como espacios en blanco, tabulaciones, saltos de línea y comentarios.

* Llevar el registro de la posición actual en el código fuente (número de línea y columna) para facilitar la emisión de mensajes de error precisos durante el análisis.

## 3.3.1 Primer analizador léxico de Aleph
### [PONER AQUI scanner que imprime por pantalla los tokens]

```c
%{
#include <stdio.h>
/* Archivo: scanner_impresion.l */
%}

/* Expresiones regulares básicas */
DIGITO      [0-9]
LETRA_MIN   [a-z]
LETRA_MAY   [A-Z]
ID          {LETRA_MAY}({LETRA_MAY}|{LETRA_MIN}|{DIGITO}|_)*

%%

"set"       { printf("Token: KW_SET, Lexema: %s\n", yytext); }
"list"      { printf("Token: KW_LIST, Lexema: %s\n", yytext); }
"while"     { printf("Token: KW_WHILE, Lexema: %s\n", yytext); }
"for"       { printf("Token: KW_FOR, Lexema: %s\n", yytext); }
"in"        { printf("Token: OP_IN, Lexema: %s\n", yytext); }
"union"     { printf("Token: OP_UNION, Lexema: %s\n", yytext); }
"empty"     { printf("Token: KW_EMPTY, Lexema: %s\n", yytext); }
"do"        { printf("Token: DELIM_DO, Lexema: %s\n", yytext); }
"end"       { printf("Token: DELIM_END, Lexema: %s\n", yytext); }
"="         { printf("Token: OP_ASSIGN, Lexema: %s\n", yytext); }

{ID}        { printf("Token: IDENTIFIER, Lexema: %s\n", yytext); }
[ \t\n\r]+  { /* Ignorar espacios en blanco y saltos de línea */ }
.           { printf("Token: ERROR_LEXICO, Lexema desconocido: %s\n", yytext); }

%%

int main(int argc, char **argv) {
    printf("Iniciando análisis léxico de prueba...\n");
    yylex();
    return 0;
}

int yywrap() {
    return 1;
}
```
### [PONER AQUI scanner que usa enumerados]

```c
%{
#include <stdio.h>
/* Archivo: scanner_enumerados.l */

/* Definición de los enumerados para los Tokens de Aleph */
typedef enum {
    KW_SET = 256,
    KW_LIST,
    KW_WHILE,
    KW_FOR,
    KW_EMPTY,
    OP_IN,
    OP_UNION,
    DELIM_DO,
    DELIM_END,
    OP_ASSIGN,
    IDENTIFIER,
    ERROR_LEXICO
} TokenType;
%}

DIGITO      [0-9]
LETRA_MIN   [a-z]
LETRA_MAY   [A-Z]
ID          {LETRA_MAY}({LETRA_MAY}|{LETRA_MIN}|{DIGITO}|_)*

%%

"set"       { return KW_SET; }
"list"      { return KW_LIST; }
"while"     { return KW_WHILE; }
"for"       { return KW_FOR; }
"in"        { return OP_IN; }
"union"     { return OP_UNION; }
"empty"     { return KW_EMPTY; }
"do"        { return DELIM_DO; }
"end"       { return DELIM_END; }
"="         { return OP_ASSIGN; }

{ID}        { return IDENTIFIER; }
[ \t\n\r]+  { /* Ignorar espacios */ }
.           { return ERROR_LEXICO; }

%%

int main(int argc, char **argv) {
    int token_actual;
    while ((token_actual = yylex()) != 0) {
        printf("Se reconoció el token con ID numérico: %d\n", token_actual);
    }
    return 0;
}

int yywrap() {
    return 1;
}
```

## 3.4. Descripción Formal de la Sintaxis
Los lenguajes de programación requieren una interpretación de sus sentencias sin ambigüedades, su descripción, a los fines de comunicar su funcionamiento, tanto a los usuarios de los mismos como a quienes realizan su implementación, requiere herramientas formales.

[PONER AQUÍ DEFINICIÓN DE SINTAXIS DE LOS LP]

La sintaxis de un lenguaje de programación establece el conjunto de reglas formales que determinan cómo deben combinarse los símbolos del alfabeto del lenguaje (tokens) para construir sentencias, expresiones y estructuras válidas. Se enfoca estrictamente en la forma y organización jerárquica del código, siendo independiente del significado computacional o los efectos de ejecución (semántica).

[PONER AQUÍ TEORÍA: LAS GLC Y LA DESCRIPCIÓN DE SINTAXIS]

Para modelar matemáticamente y especificar la sintaxis de la mayoría de los lenguajes de programación se utilizan las Gramáticas Libres de Contexto (GLC), introducidas por Noam Chomsky. Una GLC consta de cuatro elementos: un conjunto finito de símbolos terminales (los tokens indivisibles), un conjunto de símbolos no terminales (variables sintácticas que representan estructuras agrupadas, como una "sentencia"), un símbolo inicial desde el cual se deriva todo programa válido, y un conjunto de reglas de producción. Estas reglas indican cómo un no terminal puede reescribirse como una secuencia de terminales y no terminales. Esta capacidad recursiva permite a las GLC describir con precisión el anidamiento de estructuras de control (como bucles dentro de condicionales) que caracteriza a los lenguajes imperativos estructurados.

[PONER AQUÍ DEFINICIÓN DE BNF Y EBNF]

La Forma de Backus-Naur (BNF) es la notación metalingüística estándar empleada para escribir las reglas de producción de una GLC de manera legible y estandarizada. En BNF, los símbolos no terminales se encierran entre corchetes angulares (ej. `<bucle>`), la derivación se expresa mediante el símbolo `::=` o `->`, y las opciones alternativas se separan con una barra vertical `|`.

La EBNF (Forma de Backus-Naur Extendida) optimiza esta notación incorporando metacaracteres que evitan la sobrecarga de reglas recursivas simples: utiliza corchetes `[...]` para marcar elementos opcionales, llaves `{...}` para representar la repetición de un elemento (cero o más veces), y paréntesis `(...)` para la agrupación de símbolos lógicos.

## 3.3.1. Primera descripción formal de Aleph
[PONER AQUÍ EL PRIMERA BNF DE ALEPH (ES A NIVEL GENERAL NO MÁS DE 5/6 PRODUCCIONES QUE ABARQUEN LAS ESTRUCTURAS IMPORTANTES]

A continuación, presentamos un subconjunto general en BNF que abarca las estructuras de control principales y la definición de funciones diseñadas en nuestro primer prototipo algorítmico de Aleph:

```c
<programa>      ::= <funcion> | <funcion> <programa>

<funcion>       ::= <tipo_dato> <id> "(" <parametros> ")" ":" <cuerpo> "return" <expresion> ";" "end"

<cuerpo>        ::= <sentencia> | <sentencia> <cuerpo>

<sentencia>     ::= <declaracion> ";" 
                  | <asignacion> ";" 
                  | <bucle_while> 
                  | <bucle_for> 
                  | <condicional_if>

<bucle_while>   ::= "while" "(" <expresion> ")" "do" <cuerpo> "end"

<bucle_for>     ::= "for" "(" <id> "in" <expresion> ")" "do" <cuerpo> "end"
```
