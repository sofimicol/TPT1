## 3.5 Descripción formal de la sintaxis Aleph
Las instrucciones que conforman todo lenguaje se pueden agrupar en instrucciones de asignación y estructuras de control de flujo. Las instrucciones de asignación también se conocen como de transferencia de datos, pues asignan valores de una dirección a otra o de una expresión a una dirección. Las expresiones que son asignadas o evaluadas manipulan datos y retornan un valor determinado. La evaluación de expresiones puede servir entonces tanto para ser asignada a una dirección de memoria (nominada mediante un identificador) o para determinar bifurcaciones en el flujo de ejecución del programa.

Se presentan a continuación las BNF y EBNFs de las diferentes instrucciones que conforman Alpeh.

## 3.5.1 Asignación y Asignación Múltiple
Las asignaciones son parte fundamental de un lenguaje de programación imperativo, sin ellas no sería posible relacionar valores con variables para un uso posterior. Asignación

[poner aquí la BNF de la asignación y borrar esto]

## 3.5.2 Asignación Múltiple
[poner aquí la BNF de la asignación múltiple y borrar esto]

## 3.5.3 Asociatividad y Precendencia de operadores
Para transformar valores y observar relaciones entre ellos debemos contar con expresiones que puedan evaluarse. Se presentan a continuación las expresiones de Aleph.

[poner aquí la Def de asociatividad y precedencia de operadores, explicación de cómo se implementan estyos conceptos en las BNF]

[poner aquí la BNF de expresiones aritméticas (números ver en sebesta) con precedencia y asociatividad con uso de paréntesis]

[poner aquí la Árbol de derivación de una expresión aritmética que use varios operadores y paréntesis]

[poner aquí la Def árbol de sintaxis abstracta y derivación]

[Poner aquí el árbol de sintaxis abstracta del árbol de derivación antes descripto]

[poner aquí la Def operadores sobrecargados]

[Para cada operador de aleph indicar precedencia, asociatividad y si hay sobrecarga]

### Expresiones: Operaciones con Conjuntos y Listas
[poner aquí la BNF de Operaciones con Conjuntos y Listas]

### Expresiones: Operaciones Relacionales
[poner aquí la BNF de Operaciones Relacionales]

### Expresiones: Operaciones Lógicas
[Def evaluación perezosa e ingenuas]

[poner aquí la BNF de Operaciones Lógicas]

## 3.5.4 Estructuras de Control
### El control del flujo de ejecución de instrucciones permite alterar el orden en el que cada sentencia es evaluada.

### Alternativa
[Explique sobre la ambigüedad de la sentencia if]

[poner aquí la BNF y EBNF del if y explque sobre la ambiguedad o no de su propuesta]

### Ciclos
[poner aquí las BNF de las estructuras de control de ciclos que hayan definido]

## 3.5.5 Subrutinas
Las subrutinas permiten la modularización de un programa, de manera que no deba repetir código cada vez que deseo obtener un resultado aplicando argumentos a un algoritmo bien determinado, al que podemos llamar función o procedimiento, dependiendo de si el mismo devuelve o no algún dato simple o estructurado.

[poner la BNF de la subrutina (dejar para el final)]

## 3.5.6 Gramática completa de Aleph
[poner aquí la BNF completa]

## 3.6 Análisis Sintáctico
[Def de Análisis Sintáctico]

## 3.6.1 Metas del Análisis Sintáctico
[explciar]

## 3.6.2 Analizadores Top-Down
[explicar]

### Gramáticas LL
[explicar]

### Analizadores Descendentes Recursivos
[explicar]

[Ejemplo de módulos descendentes recursivos de las expresiones de Aleph]

## 3.6.3 Analizadores Bottom-Up
[explciar]

[Def Frase, Frase Simple, Manejador]

[Ventajas]

### Funcionamiento de los analizadores LR
[explciar]

### Funcionamiento del analizador generado por Bison
[explciar]

### Primer Analizador Sintáctico de Aleph
[explciar]

[Enterga de código (no poner el código aquí presentarlo en el repo correspondiente): analizar sintáctico completo sin semántica]

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
