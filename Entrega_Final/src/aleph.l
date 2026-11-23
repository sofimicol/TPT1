%option noyywrap
%option yylineno
%{
#include <stdio.h>
#include <stdlib.h>
#include "TAD_eval.h"
#include "aleph.tab.h"
%}

DIGITO      [0-9]
LETRA_MIN   [a-z]
LETRA_MAY   [A-Z]
LETRA       {LETRA_MIN}|{LETRA_MAY}
ID          {LETRA_MAY}({LETRA}|{DIGITO}|_)*
COMENT_LIN  \/\/[^\n]*
COMENT_BLO  \/\*([^*]|\*+[^*/])*\*+\/

%%

{COMENT_LIN}            { /* Ignorar comentario de una linea */ }
{COMENT_BLO}            { /* Ignorar comentario multilinea */ }
[ \t\n\r]+              { /* Ignorar espacios en blanco */ }

"set"                   { return KW_SET; }
"list"                  { return KW_LIST; }
"let"                   { return KW_LET; }
"empty"                 { return KW_EMPTY; }

"union"                 { return OP_UNION; }
"inter"|"intersection"  { return OP_INTERSECTION; }
"diff"|"difference"     { return OP_DIFFERENCE; }
"append"                { return KW_APPEND; }
"insert"                { return KW_INSERT; }

[=;:,(){}\[\]]          { return yytext[0]; }

\'[^\']\'               {
                            yylval.caracter = yytext[1];
                            return LIT_ELEMENT;
                        }

{ID}                    {
                            yylval.cadena = load2(yytext);
                            return ID;
                        }

.                       {
                            fprintf(stderr, "Linea %d - Error lexico: caracter no reconocido '%s'\n",
                                    yylineno, yytext);
                        }

%%