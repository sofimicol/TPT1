%{
#include <stdio.h>
%}

    /* Extencinones Regilares */
DIGITO  [0-9]
LETRA_MIN [a-z]
LETRA_MAY [A-Z]
ID  {LETRA_MAY}({LETRA_MAY}|{LETRA_MIN}|{DIGITO}|_)*

%%
  /* PALABRAS RESERVADAS (KW) */
"set"       { printf("KW_SET: %s\n", yytext); }
"list"      { printf("KW_LIST: %s\n", yytext); }
"boolean"   { printf("KW_BOOLEAN: %s\n", yytext); }
"while"     { printf("KW_WHILE: %s\n", yytext); }
"for"       { printf("KW_FOR: %s\n", yytext); }
"if"        { printf("KW_IF: %s\n", yytext); }
"else"      { printf("KW_ELSE: %s\n", yytext); }
"break"     { printf("KW_BREAK: %s\n", yytext); }
"return"    { printf("KW_RETURN: %s\n", yytext); }
"let"       { printf("KW_LET: %s\n", yytext); }
"where"     { printf("KW_WHERE: %s\n", yytext); }
"empty"     { printf("KW_EMPTY: %s\n", yytext); }
"any"       { printf("KW_ANY: %s\n", yytext); }
"append"    { printf("KW_APPEND: %s\n", yytext); }
"insert"    { printf("KW_INSERT: %s\n", yytext); }

  /* OPERADORES (OP) */
"in"        { printf("OP_IN: %s\n", yytext); }
"union"     { printf("OP_UNION: %s\n", yytext); }
"=="        { printf("OP_EQUAL: %s\n", yytext); }
"!="        { printf("OP_NOT_EQUAL: %s\n", yytext); }
"="         { printf("OP_ASSIGN: %s\n", yytext); }

  /* DELIMITADORES (DELIM) */
"do"        { printf("DELIM_DO: %s\n", yytext); }
"end"       { printf("DELIM_END: %s\n", yytext); }
":"         { printf("DELIM_COLON: %s\n", yytext); }
";"         { printf("DELIM_SEMICOLON: %s\n", yytext); }
","         { printf("DELIM_COMMA: %s\n", yytext); }
"("         { printf("DELIM_LPAREN: %s\n", yytext); }
")"         { printf("DELIM_RPAREN: %s\n", yytext); }
"{"         { printf("DELIM_LBRACE: %s\n", yytext); }
"}"         { printf("DELIM_RBRACE: %s\n", yytext); }
"["         { printf("DELIM_LBRACKET: %s\n", yytext); }
"]"         { printf("DELIM_RBRACKET: %s\n", yytext); }

  /* LITERALES (LIT) E IDENTIFICADORES */
"true"|"false" { printf("LIT_BOOLEAN: %s\n", yytext); }
\'[^\']\'      { printf("LIT_CHAR: %s\n", yytext); }
[A-Z][a-zA-Z0-9_]* { printf("IDENTIFIER: %s\n", yytext); }

  /* ESPACIOS Y ERRORES */
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
