/* reconice tokens para la calculadoray los imprime por pantalla */
%%
"+" { printf("SUMA\n"); }
"-" { printf("RESTA\n"); }
"*" { printf("MULTIPLICACION\n"); }
"/" { printf("DIVICION\n"); }
"|" { printf("ABS\n"); }
[0-9]+ { printf("NUMERO %s\n", yytext); }
\n { printf("LINEA\n"); }
[ \t] { }
. { printf("Carcater desconocido %s\n", yytext); }
%%