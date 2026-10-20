## 3.5 Descripción formal de la sintaxis Aleph
Las instrucciones que conforman todo lenguaje se pueden agrupar en instrucciones de asignación y estructuras de control de flujo. Las instrucciones de asignación también se conocen como de transferencia de datos, pues asignan valores de una dirección a otra o de una expresión a una dirección. Las expresiones que son asignadas o evaluadas manipulan datos y retornan un valor determinado. La evaluación de expresiones puede servir entonces tanto para ser asignada a una dirección de memoria (nominada mediante un identificador) o para determinar bifurcaciones en el flujo de ejecución del programa.

Se presentan a continuación las BNF y EBNFs de las diferentes instrucciones que conforman Alpeh.

## 3.5.1 Asignación y Asignación Múltiple
Las asignaciones son parte fundamental de un lenguaje de programación imperativo, sin ellas no sería posible relacionar valores con variables para un uso posterior. Asignación

[poner aquí la BNF de la asignación y borrar esto]

```c
<asignacion> ::= INDETIFIER = <esprecion>
              | <asignacino_mult>
```

## 3.5.2 Asignación Múltiple
[poner aquí la BNF de la asignación múltiple y borrar esto]

```C
<asignacin_mult> ::= (funcion de multiples asignaciones ej. LET o "¤") IDENTIFIRE = <lts_id>
<lts_id> ::= INDETIFIRE
            | INDENTIFIRE "," <lts_id>
```

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
```C
<op_lts_cjt> ::= <lts_conjunto> "any" <lts_conjunto>
                | <lts_conjunto> "empty" <lts_conjunto>
                | <lit_lista> "union" <lit_lista>
                | <lit_lista> "append" <lit_lista>
                | <lit_lista> "diferencia" <lit_lista>
```

### Expresiones: Operaciones Relacionales
[poner aquí la BNF de Operaciones Relacionales]

### Expresiones: Operaciones Lógicas
[Def evaluación perezosa e ingenuas]
```c
<op_log> ::= <expresion> "==" <expresion> 
              | <expresion> "!=" <expresion>
```

[poner aquí la BNF de Operaciones Lógicas]

## 3.5.4 Estructuras de Control
### El control del flujo de ejecución de instrucciones permite alterar el orden en el que cada sentencia es evaluada.

### Alternativa
[Explique sobre la ambigüedad de la sentencia if]

[poner aquí la BNF y EBNF del if y explque sobre la ambiguedad o no de su propuesta]
```c
<sent_if> ::= "if" "(" <op_log> ")" "do" <lts_sentencias> "end"
            | "if" "(" <op_log> ")" "do" <lts_sentencias> "else" <lts_sentencias> "end"
```
### Ciclos
[poner aquí las BNF de las estructuras de control de ciclos que hayan definido]
```c
<sent_while> ::= "while" "(" <op_log> ")" "do" <lts_sentencias> "end"
<sent_for> ::= "for" "(" IDENTIFIER "in" IDENTIFIER ")" "do" <lts_sentencias> "end"
```

## 3.5.5 Subrutinas
Las subrutinas permiten la modularización de un programa, de manera que no deba repetir código cada vez que deseo obtener un resultado aplicando argumentos a un algoritmo bien determinado, al que podemos llamar función o procedimiento, dependiendo de si el mismo devuelve o no algún dato simple o estructurado.

[poner la BNF de la subrutina (dejar para el final)]

## 3.5.6 Gramática completa de Aleph
[poner aquí la BNF completa]

## 3.6 Análisis Sintáctico pagina 178 sebesta capitilo 4
El análisis sintáctico (syntax analysis o parsing) de un lenguaje de programación se define como en proceso encargado de analizar la sintaxis para un programa dado con entrada sintácticamente correcta, construyendo un árbol de análisis sintáctico (parse trees) que se construye con información necesaria obtenida durante el análisis. Estos árboles y sus derivaciones incluyen toda la información sintáctica que necesita un procesador de lenguaje.

## 3.6.1 Metas del Análisis Sintáctico
El analizador sintáctico posee como metas:
1. Revisar y verificar el programa de entrada (código de un determinado Lenguaje de Programación) sea correcto, en caso de encontrar un error, el analizador debe producir un mensaje de diagnóstico, recuperarse de este y continuar el análisis del programa de entrada. Esto se realiza para encontrar la mayor cantidad de errores posibles en un único análisis del programa de entrada.
2. Producir un árbol de análisis sintáctico completo para una entrada sintácticamente correcta. Este árbol (o su rastreo) se utilizará como base para la traducción.

## 3.6.2 Analizadores Top-Down

Constituye un árbol de análisis en preorden. Dado el árbol de análisis sintáctico, el recorrido preorden construyendo desde la raíz, luego su rama izquierda que corresponde con una derivación por izquierda y por último su rama derecha. Consiste en visitar cada nodo antes de seguir sus ramas. En términos de la derivación, se puede como: dado una forma sentencial que es parte de una derivación más a la izquierda, la tarea del analizador es encontrar la siguiente forma sentencial en esa derivación más a la izquierda. 
La forma general de una forma sentencial izquierda es wAα donde: w es una cadena de símbolos terminales, A es un no terminal, y α es una cadena mixta. Como w contiene solo terminales, A es el no terminal más a la izquierda en la forma sentencial, por lo que es el que debe expandirse para obtener la siguiente forma sentencial en una derivación más a la izquierda. El analizador deberá determinar desde la gramática, la producción correcta que contienen a A en su lado izquierdo y que será utilizada para determinar la siguiente forma sentencial, este será el problema de decisión de los analizadores top-down.

### Gramáticas LL
[explicar]

### Analizadores Descendentes Recursivos
Los analizadores descendientes recursivos constan de un conjunto de subprogrmas, mayormente recursivos. Cada uno de los subprogramas se asocia a una variable sintáctica de la Gramática Libre de Contexto. Por ejemplo, dada una gramática que posee las expresiones aritméticas, que en general tiene tres variables sintácticas: <expresión> <termino> <factor>, habrá un módulo asociado a cada de ellas. El código del subprograma, asociada a cada una de las variables, describe el RHS de la producción a la que esta asociado ese subprograma. 
Los analizadores descendentes más comunes eligen el RHS correcto para el no terminal más a la izquierda en la forma sentencial actual comparando el siguiente token de entrada con los primeros símbolos que pueden generarse por los RHS de esas reglas. El RHS que tenga ese token al inicio de la cadena que genera es el correcto. Así que, en la forma sentencial wAα, el analizador usaría cualquier token que sería el primero generado por A para determinar qué regla de A se debe usar para obtener la siguiente forma sentencial.
La elección de la producción no siempre es tan sencilla, ya sea porque se repiten los terminales al inicio de los RHS o los extremos izquierdos tienen una variable sintáctica, por esto, se debe aplicar la Prueba de pares disjuntos sobre la BNF para detectar el problema antes de implementar el analizador y verificar que siempre sea posible elegir la producción correcta. 

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
