%{
#include <stdio.h>

enum yytokentype {
    KW_SET = 258,
    KW_LIST = 259,
    KW_BOOLEAN = 260,
    KW_WHILE = 261,
    KW_FOR = 262,
    KW_IF = 263,
    KW_ELSE = 264,
    DELIM_DO = 265,
    DELIM_END = 266,
    KW_BREAK = 267,
    KW_RETURN = 268,
    KW_LET = 269,
    OP_IN = 270,
    KW_WHERE = 271,
    KW_EMPTY = 272,
    OP_UNION = 273,
    KW_ANY = 274,
    KW_APPEND = 275,
    KW_INSERT = 276,
    LIT_BOOLEAN = 277,
    DELIM_COLON = 278,
    OP_EQUAL = 279,
    OP_NOT_EQUAL = 280,
    OP_ASSIGN = 281,
    DELIM_SEMICOLON = 282,
    DELIM_COMMA = 283,
    DELIM_LPAREN = 284,
    DELIM_RPAREN = 285,
    DELIM_LBRACE = 286,
    DELIM_RBRACE = 287,
    DELIM_LBRACKET = 288,
    DELIM_RBRACKET = 289,
    LIT_CHAR = 290,
    IDENTIFIER = 291
};
%}

%%
  /* PALABRAS RESERVADAS (KW) */
"set"       { return KW_SET; }
"list"      { return KW_LIST; }
"boolean"   { return KW_BOOLEAN; }
"while"     { return KW_WHILE; }
"for"       { return KW_FOR; }
"if"        { return KW_IF; }
"else"      { return KW_ELSE; }
"break"     { return KW_BREAK; }
"return"    { return KW_RETURN; }
"let"       { return KW_LET; }
"where"     { return KW_WHERE; }
"empty"     { return KW_EMPTY; }
"any"       { return KW_ANY; }
"append"    { return KW_APPEND; }
"insert"    { return KW_INSERT; }

  /* OPERADORES (OP) */
"in"        { return OP_IN; }
"union"     { return OP_UNION; }
"=="        { return OP_EQUAL; }
"!="        { return OP_NOT_EQUAL; }
"="         { return OP_ASSIGN; }

  /* DELIMITADORES (DELIM) */
"do"        { return DELIM_DO; }
"end"       { return DELIM_END; }
":"         { return DELIM_COLON; }
";"         { return DELIM_SEMICOLON; }
","         { return DELIM_COMMA; }
"("         { return DELIM_LPAREN; }
")"         { return DELIM_RPAREN; }
"{"         { return DELIM_LBRACE; }
"}"         { return DELIM_RBRACE; }
"["         { return DELIM_LBRACKET; }
"]"         { return DELIM_RBRACKET; }

  /* LITERALES (LIT) E IDENTIFICADORES */
"true"|"false"     { return LIT_BOOLEAN; }
\'[^\']\'          { return LIT_CHAR; }
[A-Z][a-zA-Z0-9_]* { return IDENTIFIER; }

  /* ESPACIOS Y ERRORES */
[ \t\n\r]+  { /* ignorar espacios en blanco */ }
.           { printf("Error lexico: Caracter no reconocido %s\n", yytext); }
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
        printf("\nIniciando analizador lexico. Ingrese codigo y presione Ctrl+Z para finalizar.\n");
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

int yywrap() {
    return 1;
}
