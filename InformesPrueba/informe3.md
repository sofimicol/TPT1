```c
<programa> ::= <tipo> IDENTIFIER "(" <lts_parametros> ")" ":" <lts_sentencias> "end"

<lts_parametros> ::= <parametro> 
                | <parametro> "," <lts_parametros> 
                | ε

<paramtro> ::= <tipo> IDENTIFIER

<lts_sentencias> ::= <sent_simple> ";" 
               | <sent_simple> ";" <lts_sentencias> 
               | <estruc_control> 
               | <estruc_control> <lts_sentencias>

<sent_simple> ::= <declaracion> 
                | <asignacion> 
                | <llamada_rutina> 
                | "return" <expresion> 
                | "break"

<declaracion> ::= <tipo> IDENTIFIER "=" <expresion> 
                | "let" IDENTIFIER "=" <expresion>

<asignacion> ::= IDENTIFIER "=" <expresion>

<estruc_control> ::= <sent_if> 
                   | <sent_while> 
                   | <sent_for>

<sent_if> ::= "if" "(" <condicion> ")" "do" <lista_sent> "end"
            | "if" "(" <condicion> ")" "do" <lista_sent> "else" <lista_sent> "end"

<sent_while> ::= "while" "(" <condicion> ")" "do" <lista_sent> "end"

<sent_for> ::= "for" "(" IDENTIFIER "in" IDENTIFIER ")" "do" <lista_sent> "end"
             | "for" "(" IDENTIFIER "in" IDENTIFIER ")" "where" "(" <condicion> ")" "do" <lista_sent> "end"

<condicion> ::= <expresion> "==" <expresion> 
              | <expresion> "!=" <expresion> 
              | "any" "(" <lts_exprsiones> ")"

<expresion> ::= <termino> 
         | <termino> "union" <termino>

<termino> ::= IDENTIFIER 
            | LIT_BOOLEAN 
            | LIT_CHAR 
            | "empty" 
            | <lts_lista> 
            | <lts_conjunto>

<lts_lista> ::= "[" <lts_expresino> "]"

<lts_conjunto> ::= "{" <lts_expresion> "}"

<lts_expresiones> ::= <expresion> 
               | <expr> "," <lts_expresion>

<llamada_rutina> ::= "insert" "(" <lts_expresiones> ")" 
                   | "append" "(" <lts_expresiones> ")"

<tipo> ::= "set" | "list" | "boolean"
```
