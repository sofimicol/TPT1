## 3.3 Análisis Léxico

Un analizador léxico realiza, técnicamente, un análisis sintáctico al nivel más bajo de la estructura del programa. Su función principal consiste en reconocer agrupaciones lógicas de caracteres (al identificar subcadenas) mediante la coincidencia de patrones sobre el código de entrada. Dichas agrupaciones se denominan lexemas, mientras que las categorías sintácticas o códigos internos asignados a ellas corresponden a los tokens.


### Descripción de las tareas de un analizador léxico

Las tareas fundamentales de un analizador léxico (scanner) incluyen:

* Identificar y agrupar los caracteres en lexemas válidos basándose en patrones regulares.

* Generar y entregar secuencialmente los tokens al analizador sintáctico (parser) cada vez que este los requiere para la construcción del árbol sintáctico.

* Filtrar y descartar elementos que carecen de significado estructural para el compilador o intérprete, tales como espacios en blanco, tabulaciones, saltos de línea y comentarios.

* Llevar el registro de la posición actual en el código fuente (número de línea y columna) para facilitar la emisión de mensajes de error precisos durante el análisis.

## 3.3.1 Primer analizador léxico de Aleph
### Scanner que imprime por pantalla los tokens

```c
%{
#include <stdio.h>
%}

%option noyywrap

%%
  /* PALABRAS RESERVADAS Y FUNCIONES PRIMITIVAS */
"set"          { printf("KW_SET: %s\n", yytext); }
"list"         { printf("KW_LIST: %s\n", yytext); }
"while"        { printf("KW_WHILE: %s\n", yytext); }
"for"          { printf("KW_FOR: %s\n", yytext); }
"if"           { printf("KW_IF: %s\n", yytext); }
"else"         { printf("KW_ELSE: %s\n", yytext); }
"break"        { printf("KW_BREAK: %s\n", yytext); }
"return"       { printf("KW_RETURN: %s\n", yytext); }
"let"          { printf("KW_LET: %s\n", yytext); }
"where"        { printf("KW_WHERE: %s\n", yytext); }
"empty"        { printf("KW_EMPTY: %s\n", yytext); }
"any"          { printf("KW_ANY: %s\n", yytext); }
"append"       { printf("KW_APPEND: %s\n", yytext); }
"insert"       { printf("KW_INSERT: %s\n", yytext); }

  /* OPERADORES DE COLECCIÓN, RELACIONALES Y LÓGICOS */
"in"           { printf("OP_IN: %s\n", yytext); }
"union"        { printf("OP_UNION: %s\n", yytext); }
"intersection" { printf("OP_INTERSECTION: %s\n", yytext); }
"contain"      { printf("OP_CONTAIN: %s\n", yytext); }
"difference"   { printf("OP_DIFFERENCE: %s\n", yytext); }
"&&"           { printf("KW_AND: %s\n", yytext); }
"!"            { printf("KW_NOT: %s\n", yytext); }
"||"           { printf("KW_OR: %s\n", yytext); }
"=="           { printf("OP_EQUAL: %s\n", yytext); }
"!="           { printf("OP_NOT_EQUAL: %s\n", yytext); }
"="            { printf("OP_ASSIGN: %s\n", yytext); }

  /* DELIMITADORES Y SÍMBOLOS ESPECIALES */
"do"           { printf("DELIM_DO: %s\n", yytext); }
"end"          { printf("DELIM_END: %s\n", yytext); }
":"            { printf("DELIM_COLON: %s\n", yytext); }
";"            { printf("DELIM_SEMICOLON: %s\n", yytext); }
","            { printf("DELIM_COMMA: %s\n", yytext); }
"("            { printf("DELIM_LPAREN: %s\n", yytext); }
")"            { printf("DELIM_RPAREN: %s\n", yytext); }
"{"            { printf("DELIM_LBRACE: %s\n", yytext); }
"}"            { printf("DELIM_RBRACE: %s\n", yytext); }
"["            { printf("DELIM_LBRACKET: %s\n", yytext); }
"]"            { printf("DELIM_RBRACKET: %s\n", yytext); }

  /* LITERALES E IDENTIFICADORES */
\'[^\']\'      { printf("LIT_ELEMENT: %s\n", yytext); }
[A-Z][a-zA-Z0-9_]* { printf("ID: %s\n", yytext); }

  /* ESPACIOS Y ERRORES */
[ \t\n\r]+     { /* ignorar espacios en blanco */ }
.              { printf("Error lexico: Caracter no reconocido %s\n", yytext); }
%%

int main(int argc, char **argv) {
    printf("Iniciando escaner de Aleph. Escriba codigo y presione Ctrl+Z (Windows) o Ctrl+D (Linux) para salir.\n");
    yylex();
    return 0;
}
```
### Scanner que usa enumerados

```c
%{
#include <stdio.h>

enum yytokentype {
    KW_SET = 258,
    KW_LIST = 259,
    KW_WHILE = 260,
    KW_FOR = 261,
    KW_IF = 262,
    KW_ELSE = 263,
    DELIM_DO = 264,
    DELIM_END = 265,
    KW_BREAK = 266,
    KW_RETURN = 267,
    KW_LET = 268,
    KW_WHERE = 269,
    KW_EMPTY = 270,
    KW_ANY = 271,
    KW_APPEND = 272,
    KW_INSERT = 273,
    OP_IN = 274,
    OP_UNION = 275,
    OP_INTERSECTION = 276,
    OP_CONTAIN = 277,
    OP_RELATION = 278,
    OP_DIFFERENCE = 279,
    KW_AND = 280,
    KW_NOT = 281,
    KW_OR = 282,
    OP_EQUAL = 283,
    OP_NOT_EQUAL = 284,
    OP_ASSIGN = 285,
    DELIM_COLON = 286,
    DELIM_SEMICOLON = 287,
    DELIM_COMMA = 288,
    DELIM_LPAREN = 289,
    DELIM_RPAREN = 290,
    DELIM_LBRACE = 291,
    DELIM_RBRACE = 292,
    DELIM_LBRACKET = 293,
    DELIM_RBRACKET = 294,
    LIT_ELEMENT = 295,
    ID = 296
};
%}

%option noyywrap

%%
  /* PALABRAS RESERVADAS Y FUNCIONES PRIMITIVAS */
"set"          { return KW_SET; }
"list"         { return KW_LIST; }
"while"        { return KW_WHILE; }
"for"          { return KW_FOR; }
"if"           { return KW_IF; }
"else"         { return KW_ELSE; }
"break"        { return KW_BREAK; }
"return"       { return KW_RETURN; }
"let"          { return KW_LET; }
"where"        { return KW_WHERE; }
"empty"        { return KW_EMPTY; }
"any"          { return KW_ANY; }
"append"       { return KW_APPEND; }
"insert"       { return KW_INSERT; }

  /* OPERADORES DE COLECCIÓN, RELACIONALES Y LÓGICOS */
"in"           { return OP_IN; }
"union"        { return OP_UNION; }
"intersection" { return OP_INTERSECTION; }
"contain"      { return OP_CONTAIN; }
"difference"   { return OP_DIFFERENCE; }
"&&"           { return KW_AND; }
"!"            { return KW_NOT; }
"||"           { return KW_OR; }
"=="           { return OP_EQUAL; }
"!="           { return OP_NOT_EQUAL; }
"="            { return OP_ASSIGN; }

  /* DELIMITADORES Y SÍMBOLOS ESPECIALES */
"do"           { return DELIM_DO; }
"end"          { return DELIM_END; }
":"            { return DELIM_COLON; }
";"            { return DELIM_SEMICOLON; }
","            { return DELIM_COMMA; }
"("            { return DELIM_LPAREN; }
")"            { return DELIM_RPAREN; }
"{"            { return DELIM_LBRACE; }
"}"            { return DELIM_RBRACE; }
"["            { return DELIM_LBRACKET; }
"]"            { return DELIM_RBRACKET; }

  /* LITERALES E IDENTIFICADORES */
\'[^\']\'      { return LIT_ELEMENT; }
[A-Z][a-zA-Z0-9_]* { return ID; }

  /* ESPACIOS Y ERRORES */
[ \t\n\r]+     { /* ignorar espacios en blanco */ }
.              { printf("Error lexico: Caracter no reconocido %s\n", yytext); }
%%

int main(int argc, char **argv) {
    int token;
    
    if (argc > 1) {
        FILE *archivo = fopen(argv[1], "r");
        if (!archivo) {
            printf("Error: No se pudo abrir el archivo %s\n", argv[1]);
            return 1;
        }
        yyin = archivo;
    } else {
        printf("\nIniciando analizador lexico. Ingrese codigo y presione Ctrl+Z (Windows) o Ctrl+D (Linux) para finalizar.\n");
    }

    printf("\n=====================================\n");
    printf(" %-15s | %s\n", "ID NUMERICO", "LEXEMA RECONOCIDO");
    printf("=====================================\n");

    while ((token = yylex()) != 0) {
        printf(" %-15d | '%s'\n", token, yytext);
    }
    
    printf("=====================================\n\n");
    return 0;
}
```
***Prefijos***:
KW (Keyword): identifica palabras reservadas que tienen un significado especial en el lenguaje.
DELIM (Delimiter): identifica signos que delimitan, separan o estructuran las construcciones sintácticas.
OP (Operator): identifica operadores que realizan comparaciones, asignaciones u otras operaciones.
LIT (Literal): identifica valores constantes escritos directamente en el código fuente.

## 3.4. Descripción Formal de la Sintaxis
Los lenguajes de programación requieren una interpretación de sus sentencias sin ambigüedades, su descripción, a los fines de comunicar su funcionamiento, tanto a los usuarios de los mismos como a quienes realizan su implementación, requiere herramientas formales.

## SINTAXIS DE LOS LP

La sintaxis de un lenguaje de programación establece el conjunto de reglas formales que determinan cómo deben combinarse los símbolos del alfabeto del lenguaje (tokens) para construir sentencias, expresiones y estructuras válidas. Se enfoca estrictamente en la forma y organización jerárquica del código, siendo independiente del significado computacional o los efectos de ejecución (semántica).

## LAS GLC Y LA DESCRIPCIÓN DE SINTAXIS

Para modelar matemáticamente y especificar la sintaxis de la mayoría de los lenguajes de programación se utilizan las Gramáticas Libres de Contexto (GLC), introducidas por Noam Chomsky. Una GLC consta de cuatro elementos: un conjunto finito de símbolos terminales (los tokens indivisibles), un conjunto de símbolos no terminales (variables sintácticas que representan estructuras agrupadas, como una "sentencia"), un símbolo inicial desde el cual se deriva todo programa válido, y un conjunto de reglas de producción. Estas reglas indican cómo un no terminal puede reescribirse como una secuencia de terminales y no terminales. Esta capacidad recursiva permite a las GLC describir con precisión el anidamiento de estructuras de control (como bucles dentro de condicionales) que caracteriza a los lenguajes imperativos estructurados.

## DEFINICIÓN DE BNF Y EBNF

La Forma de Backus-Naur (BNF) es la notación metalingüística estándar empleada para escribir las reglas de producción de una GLC de manera legible y estandarizada. En BNF, los símbolos no terminales se encierran entre corchetes angulares (ej. `<bucle>`), la derivación se expresa mediante el símbolo `::=` o `->`, y las opciones alternativas se separan con una barra vertical `|`.

La EBNF (Forma de Backus-Naur Extendida) optimiza esta notación incorporando metacaracteres que evitan la sobrecarga de reglas recursivas simples: utiliza corchetes `[...]` para marcar elementos opcionales, llaves `{...}` para representar la repetición de un elemento (cero o más veces), y paréntesis `(...)` para la agrupación de símbolos lógicos.

## 3.3.1. Primera descripción formal de Aleph

A continuación, presentamos un subconjunto general en BNF que abarca las estructuras de control principales y la definición de funciones diseñadas en nuestro primer prototipo algorítmico de Aleph:

```c
<Aleph>            ::= <lts_sent>

<lts_sent>         ::= <sent> ';'
                     | <lts_sent> <sent> ';'
  
<bloque>           ::= DELIM_DO <lts_sent> DELIM_END
                     | DELIM_DO DELIM_END

<sent>             ::= <asignacion>
                     | <expresion> 
                     | <sent_while> 
                     | <sent_if> 
                     | <sent_for>
                     | KW_RETURN <expresion>
                     | KW_BREAK

<asignacion>       ::= ID '=' <expresion>
                     | KW_LET <lts_id> '=' <lts_expresiones>

<lts_id>           ::= ID 
                     | <lts_id> ',' ID

<sent_if>          ::= KW_IF '(' <op_log> ')' <bloque>
                     | KW_IF '(' <op_log> ')' <bloque> KW_ELSE <bloque>

<sent_while>       ::= KW_WHILE '(' <op_log> ')' <bloque>

<sent_for>         ::= KW_FOR '(' ID OP_IN <expresion> ')' <bloque>

<condicion>        ::= <expresion> "==" <expresion> 
                     | <expresion> "!=" <expresion> 

<expresion>        ::= IDE
                     | LIT_ELEMENT
                     | <lts_lista> 
                     | <lts_conjunto>
                     | <expresion> OP_UNION <termino>
                     | <expresion> OP_INTERSECTION <termino>
                     | <expresion> OP_DIFFERENCE <termino>
                     | <expresion> KW_APPEND <termino>
                     | <expresion> KW_INSERT <termino>

<lts_lista>        ::= '[' <lts_expresiones> ']'
                     | '[' ']'

<lts_conjunto>     ::= '{' <lts_expresiones> '}'
                     | '{' '}'

<lts_expresiones> ::= <expresion> 
                    | <expresion> "," <lts_expresiones>
```
