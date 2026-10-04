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
%}

    /* Extencinones Regilares */
DIGITO  [0-9]
LETRA_MIN [a-z]
LETRA_MAY [A-Z]
ID  {LETRA_MAY}({LETRA_MAY}|{LETRA_MIN}|{DIGITO}|_)*

%%

    /* TOKENS */
"set"       { printf("TOKEN_SET\n"); }
"list"      { printf("TOKEN_LIST\n"); }
"boolean"   { printf("TOKEN_BOOLEAN\n"); }
"while"     { printf("TOKEN_WHILE\n"); }
"for"       { printf("TOKEN_FOR\n"); }
"if"        { printf("TOKEN_IF\n"); }
"else"      { printf("TOKEN_ELSE\n"); }
"do"        { printf("TOKEN_DO\n"); }
"end"       { printf("TOKEN_END\n"); }
"break"     { printf("TOKEN_BREAK\n"); }
"return"    { printf("TOKEN_RETURN\n"); }
"let"       { printf("TOKEN_LET\n"); }
"in"        { printf("TOKEN_IN\n"); }
"where"     { printf("TOKEN_WHERE\n"); }
"empty"     { printf("TOKEN_EMPTY\n"); }
"union"     { printf("TOKEN_UNION\n"); }
"any"       { printf("TOKEN_ANY\n"); }
"append"    { printf("TOKEN_APPEND\n"); }
"insert"    { printf("TOKEN_INSERT\n"); }
"true"      { printf("TOKEN_TRUE\n"); }
"false"     { printf("TOKEN_FALSE\n"); }

    /*OPERADORES*/
"=="        { printf("TOKEN_EQ\n"); }
"!="        { printf("TOKEN_NEQ\n"); }
"="         { printf("TOKEN_ASSIGN\n"); }

    /*SIMBOLOS DE PUNTUACINO*/
":"         { printf("TOKEN_COLON\n"); }
";"         { printf("TOKEN_SEMICOLON\n"); }
","         { printf("TOKEN_COMMA\n"); }
"("         { printf("TOKEN_LPAREN\n"); }
")"         { printf("TOKEN_RPAREN\n"); }
"{"         { printf("TOKEN_LBRACE\n"); }
"}"         { printf("TOKEN_RBRACE\n"); }
"["         { printf("TOKEN_LBRACKET\n"); }
"]"         { printf("TOKEN_RBRACKET\n"); }

\'[^\']\'   { printf("TOKEN_CHAR_LITERAL: %s\n", yytext); }
{ID}        { printf("TOKEN_IDENTIFIER: %s\n", yytext); }
[ \t\n\r]+  { /* ignorar espacios en blanco */ }
.           { printf("Error lexico: Caracter no reconocido %s\n", yytext); }

%%

int main(int argc, char **argv) {
    printf("Iniciando escaner de Aleph. Escriba codigo y presione Ctrl+Z para salir.\n");
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

enum yytokentype {
    TOKEN_SET = 258,
    TOKEN_LIST = 259,
    TOKEN_BOOLEAN = 260,
    TOKEN_WHILE = 261,
    TOKEN_FOR = 262,
    TOKEN_IF = 263,
    TOKEN_ELSE = 264,
    TOKEN_DO = 265,
    TOKEN_END = 266,
    TOKEN_BREAK = 267,
    TOKEN_RETURN = 268,
    TOKEN_LET = 269,
    TOKEN_IN = 270,
    TOKEN_WHERE = 271,
    TOKEN_EMPTY = 272,
    TOKEN_UNION = 273,
    TOKEN_ANY = 274,
    TOKEN_APPEND = 275,
    TOKEN_INSERT = 276,
    TOKEN_TRUE = 277,
    TOKEN_FALSE = 278,
    TOKEN_COLON = 279,
    TOKEN_EQ = 280,
    TOKEN_NEQ = 281,
    TOKEN_ASSIGN = 282,
    TOKEN_SEMICOLON = 283,
    TOKEN_COMMA = 284,
    TOKEN_LPAREN = 285,
    TOKEN_RPAREN = 286,
    TOKEN_LBRACE = 287,
    TOKEN_RBRACE = 288,
    TOKEN_LBRACKET = 289,
    TOKEN_RBRACKET = 290,
    TOKEN_CHAR_LITERAL = 291,
    TOKEN_IDENTIFIER = 292
};
%}

%%
"set"       { return TOKEN_SET; }
"list"      { return TOKEN_LIST; }
"boolean"   { return TOKEN_BOOLEAN; }
"while"     { return TOKEN_WHILE; }
"for"       { return TOKEN_FOR; }
"if"        { return TOKEN_IF; }
"else"      { return TOKEN_ELSE; }
"do"        { return TOKEN_DO; }
"end"       { return TOKEN_END; }
"break"     { return TOKEN_BREAK; }
"return"    { return TOKEN_RETURN; }
"let"       { return TOKEN_LET; }
"in"        { return TOKEN_IN; }
"where"     { return TOKEN_WHERE; }
"empty"     { return TOKEN_EMPTY; }
"union"     { return TOKEN_UNION; }
"any"       { return TOKEN_ANY; }
"append"    { return TOKEN_APPEND; }
"insert"    { return TOKEN_INSERT; }
"true"      { return TOKEN_TRUE; }
"false"     { return TOKEN_FALSE; }

    /* OPERADORES Y PUNTUACION */
":"         { return TOKEN_COLON; }
"=="        { return TOKEN_EQ; }
"!="        { return TOKEN_NEQ; }
"="         { return TOKEN_ASSIGN; }
";"         { return TOKEN_SEMICOLON; }
","         { return TOKEN_COMMA; }
"("         { return TOKEN_LPAREN; }
")"         { return TOKEN_RPAREN; }
"{"         { return TOKEN_LBRACE; }
"}"         { return TOKEN_RBRACE; }
"["         { return TOKEN_LBRACKET; }
"]"         { return TOKEN_RBRACKET; }

\'[^\']\'             { return TOKEN_CHAR_LITERAL; }
[A-Z][a-zA-Z0-9_]* { return TOKEN_IDENTIFIER; }

[ \t\n\r]+  { /* ignorar espacios en blanco */ }
.           { printf("Error lexico: Caracter no reconocido %s\n", yytext); }
%%

int main(int argc, char **argv) {
    int tok;
    printf("Iniciando escanear con enumerados. Presione Ctrl+Z para salir.\n");
    
    while ((tok = yylex()) != 0) {
        printf("Token numerico devuelto: %d (Lexema: %s)\n", tok, yytext);
    }
    
    return 0;
}

int yywrap() {
    return 1;
}
```
## segunda opcion mas visual para scanner de enumerados
```c
%{
#include <stdio.h>

enum yytokentype {
    TOKEN_SET = 258,
    TOKEN_LIST = 259,
    TOKEN_BOOLEAN = 260,
    TOKEN_WHILE = 261,
    TOKEN_FOR = 262,
    TOKEN_IF = 263,
    TOKEN_ELSE = 264,
    TOKEN_DO = 265,
    TOKEN_END = 266,
    TOKEN_BREAK = 267,
    TOKEN_RETURN = 268,
    TOKEN_LET = 269,
    TOKEN_IN = 270,
    TOKEN_WHERE = 271,
    TOKEN_EMPTY = 272,
    TOKEN_UNION = 273,
    TOKEN_ANY = 274,
    TOKEN_APPEND = 275,
    TOKEN_INSERT = 276,
    TOKEN_TRUE = 277,
    TOKEN_FALSE = 278,
    TOKEN_COLON = 279,
    TOKEN_EQ = 280,
    TOKEN_NEQ = 281,
    TOKEN_ASSIGN = 282,
    TOKEN_SEMICOLON = 283,
    TOKEN_COMMA = 284,
    TOKEN_LPAREN = 285,
    TOKEN_RPAREN = 286,
    TOKEN_LBRACE = 287,
    TOKEN_RBRACE = 288,
    TOKEN_LBRACKET = 289,
    TOKEN_RBRACKET = 290,
    TOKEN_CHAR_LITERAL = 291,
    TOKEN_IDENTIFIER = 292
};
%}

%%
"set"       { return TOKEN_SET; }
"list"      { return TOKEN_LIST; }
"boolean"   { return TOKEN_BOOLEAN; }
"while"     { return TOKEN_WHILE; }
"for"       { return TOKEN_FOR; }
"if"        { return TOKEN_IF; }
"else"      { return TOKEN_ELSE; }
"do"        { return TOKEN_DO; }
"end"       { return TOKEN_END; }
"break"     { return TOKEN_BREAK; }
"return"    { return TOKEN_RETURN; }
"let"       { return TOKEN_LET; }
"in"        { return TOKEN_IN; }
"where"     { return TOKEN_WHERE; }
"empty"     { return TOKEN_EMPTY; }
"union"     { return TOKEN_UNION; }
"any"       { return TOKEN_ANY; }
"append"    { return TOKEN_APPEND; }
"insert"    { return TOKEN_INSERT; }
"true"      { return TOKEN_TRUE; }
"false"     { return TOKEN_FALSE; }

    /* OPERADORES Y PUNTUACION */
":"         { return TOKEN_COLON; }
"=="        { return TOKEN_EQ; }
"!="        { return TOKEN_NEQ; }
"="         { return TOKEN_ASSIGN; }
";"         { return TOKEN_SEMICOLON; }
","         { return TOKEN_COMMA; }
"("         { return TOKEN_LPAREN; }
")"         { return TOKEN_RPAREN; }
"{"         { return TOKEN_LBRACE; }
"}"         { return TOKEN_RBRACE; }
"["         { return TOKEN_LBRACKET; }
"]"         { return TOKEN_RBRACKET; }

\'[^\']\'             { return TOKEN_CHAR_LITERAL; }
[A-Z][a-zA-Z0-9_]* { return TOKEN_IDENTIFIER; }

[ \t\n\r]+  { /* ignorar espacios en blanco */ }
.           { printf("Error lexico: Caracter no reconocido %s\n", yytext); }
%%

const char* obtener_nombre_token(int token) {
    switch(token) {
        case 258: return "TOKEN_SET";
        case 259: return "TOKEN_LIST";
        case 260: return "TOKEN_BOOLEAN";
        case 261: return "TOKEN_WHILE";
        case 262: return "TOKEN_FOR";
        case 263: return "TOKEN_IF";
        case 264: return "TOKEN_ELSE";
        case 265: return "TOKEN_DO";
        case 266: return "TOKEN_END";
        case 267: return "TOKEN_BREAK";
        case 268: return "TOKEN_RETURN";
        case 269: return "TOKEN_LET";
        case 270: return "TOKEN_IN";
        case 271: return "TOKEN_WHERE";
        case 272: return "TOKEN_EMPTY";
        case 273: return "TOKEN_UNION";
        case 274: return "TOKEN_ANY";
        case 275: return "TOKEN_APPEND";
        case 276: return "TOKEN_INSERT";
        case 277: return "TOKEN_TRUE";
        case 278: return "TOKEN_FALSE";
        case 279: return "TOKEN_COLON";
        case 280: return "TOKEN_EQ";
        case 281: return "TOKEN_NEQ";
        case 282: return "TOKEN_ASSIGN";
        case 283: return "TOKEN_SEMICOLON";
        case 284: return "TOKEN_COMMA";
        case 285: return "TOKEN_LPAREN";
        case 286: return "TOKEN_RPAREN";
        case 287: return "TOKEN_LBRACE";
        case 288: return "TOKEN_RBRACE";
        case 289: return "TOKEN_LBRACKET";
        case 290: return "TOKEN_RBRACKET";
        case 291: return "TOKEN_CHAR_LITERAL";
        case 292: return "TOKEN_IDENTIFIER";
        default: return "DESCONOCIDO";
    }
}

int main(int argc, char **argv) {
    int token;

    printf("\nIniciando analizador lexico. Ingrese codigo y presione Ctrl+Z para finalizar.\n");
    printf("\n==============================================================\n");
    printf(" %-22s | %-12s | %s\n", "NOMBRE DEL TOKEN", "ID NUMERICO", "LEXEMA RECONOCIDO");
    printf("==============================================================\n");

    while ((token = yylex()) != 0) {
        printf(" %-22s | %-12d | '%s'\n", obtener_nombre_token(token), token, yytext);
    }

    printf("==============================================================\n\n");
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
<programa>      ::= 

<funcion>       ::= 

<cuerpo>        ::=

<sentencia>     ::= <declaracion> ";" 
                  | <asignacion> ";" 
                  | <bucle_while> 
                  | <bucle_for> 
                  | <condicional_if>

<bucle_while>   ::=

<bucle_for>     ::= 
```
