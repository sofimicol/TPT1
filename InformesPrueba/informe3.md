## 3.5 Descripción formal de la sintaxis Aleph
Las instrucciones que conforman todo lenguaje se pueden agrupar en instrucciones de asignación y estructuras de control de flujo. Las instrucciones de asignación también se conocen como de transferencia de datos, pues asignan valores de una dirección a otra o de una expresión a una dirección. Las expresiones que son asignadas o evaluadas manipulan datos y retornan un valor determinado. La evaluación de expresiones puede servir entonces tanto para ser asignada a una dirección de memoria (nominada mediante un identificador) o para determinar bifurcaciones en el flujo de ejecución del programa.

Se presentan a continuación las BNF y EBNFs de las diferentes instrucciones que conforman Alpeh.

## 3.5.1 Asignación y Asignación Múltiple
Las asignaciones son parte fundamental de un lenguaje de programación imperativo, sin ellas no sería posible relacionar valores con variables para un uso posterior. Asignación

[poner aquí la BNF de la asignación y borrar esto]

```c
<asignacion> ::= ID '=' <esprecion> | <asignacino_mult>
```

## 3.5.2 Asignación Múltiple
[poner aquí la BNF de la asignación múltiple y borrar esto]

```C
<asignacin_mult>   ::= KW_LET <lts_id> '=' <lts_expresiones>

<lts_id>           ::= ID | ID ',' <lts_id>

<lts_expresiones>  ::= <expresion> | <expresion> ',' <lts_espresiones>
```

## 3.5.3 Asociatividad y Precendencia de operadores
Para transformar valores y observar relaciones entre ellos debemos contar con expresiones que puedan evaluarse. Se presentan a continuación las expresiones de Aleph.

[poner aquí la Def de asociatividad y precedencia de operadores, explicación de cómo se implementan estyos conceptos en las BNF]

Asociatividad y Precedencia:

La precedencia de operadores responde a la pregunta semántica sobre el orden de evaluación cuando una expresión incluye dos operadores diferentes. En una gramática BNF, la precedencia se especifica forzando a que el operador con mayor prioridad se genere en un nivel más bajo (más alejado del símbolo inicial) dentro del árbol de análisis sintáctico.
La asociatividad define qué operador se evalúa primero cuando una expresión contiene dos operadores adyacentes con el mismo nivel de precedencia. En las gramáticas, la asociatividad por la izquierda se indica mediante una regla recursiva por la izquierda (donde el lado izquierdo de la regla aparece al principio del lado derecho), mientras que la recursividad por la derecha indica asociatividad por la derecha.

[poner aquí la BNF de expresiones aritméticas (números ver en sebesta) con precedencia y asociatividad con uso de paréntesis]
```c
<expresion>  ::= <expresion> "+" <termino> 
               | <expresion> "-" <termino> 
               | <termino>

<termino>    ::= <termino> "*" <factor> 
               | <termino> "/" <factor> 
               | <factor>

<factor>     ::= "(" <expresion> ")" 
               | ID 
               | LIT_NUM
```
[poner aquí la Árbol de derivación de una expresión aritmética que use varios operadores y paréntesis]

Arbol de Derivacion para la expresion para `A = B + C * A`:

                  <asignacion>
                 /      |     \
               ID      "="   <expresion>
               |             /    |    \
               A   <expresion>   "+"  <termino>
                       |              /   |   \
                   <termino>   <termino> "*" <factor>
                       |           |            |
                   <factor>    <factor>         ID
                       |           |            |
                       ID          ID           A
                       |           |
                       B           C

[poner aquí la Def árbol de sintaxis abstracta y derivación]

Derivación y Árbol de Sintaxis Abstracta (AST):

Una derivación es una secuencia de aplicaciones de reglas gramaticales que comienza con un símbolo inicial especial y termina en una oración generada sin símbolos no terminales. El árbol de análisis sintáctico (Parse Tree) describe de forma natural la estructura sintáctica jerárquica de las oraciones del lenguaje. Un Árbol de Sintaxis Abstracta (AST) es una representación condensada de este árbol en donde los símbolos no terminales sintácticos innecesarios se eliminan y los operadores pasan a ser los nodos raíz de sus respectivos operandos.

[Poner aquí el árbol de sintaxis abstracta del árbol de derivación antes descripto]

Árbol de Sintaxis Abstracto (AST) para `A = B + C * A`:

                           "="
                          /   \
                         A    "+"
                             /   \
                            B    "*"
                                /   \
                               C     A

[poner aquí la Def operadores sobrecargados]

Operadores Sobrecargados:

El uso múltiple de un mismo símbolo de operador para diferentes propósitos es lo que se conoce formalmente como sobrecarga de operadores. Al estructurar la gramática de Aleph, asignamos lexemas a delimitadores sintácticos (`), ], }`), separadores sintácticos (`.`) y símbolos especiales (`:`). Por su parte, a los operadores de palabra como la unión los identificamos directamente por su token (`OP_UNION`). De esta forma evitamos la sobrecarga, logrando que el árbol de análisis sintáctico determine de forma única la operación. Esto significa que las herramientas de análisis léxico y sintáctico (`como Flex y Bison`) pueden reconocer y procesar las instrucciones directamente desde la estructura de la gramática, sin requerir reglas semánticas complejas para desambiguar el propósito de un símbolo.

[Para cada operador de aleph indicar precedencia, asociatividad y si hay sobrecarga]

* Relacionales (==, !=, KW_IN): Precedencia Baja, Asociatividad Ninguna, Sobrecargados (comparación de distintos tipos).

* Conjuntos/Listas (union, insert, append): Precedencia Media (igual a suma), Asociatividad Izquierda, No sobrecargados.

* Lógicos (KW_AND, KW_OR): Precedencia Muy Baja, Asociatividad Izquierda, No sobrecargados.

### Expresiones: Operaciones con Conjuntos y Listas
[poner aquí la BNF de Operaciones con Conjuntos y Listas]
```C
<expreciones>  ::= ID
                 | LIT_ELEM
                 | <lts_lista>
                 | <lts_conjunto>
                 | <expresion> OP_UNION <expresion>
                 | <expresión> OP_INTERSECTION <expresion>
                 | <expresión> OP_RELATION <exprecion>
                 | <expresion> OP_DIFFERENCE <expresion>
                 | <expresión> KW_APPEND <exprecion>
                 | <expresión> KW_INSERT <exprecion>
```

### Expresiones: Operaciones Relacionales
[poner aquí la BNF de Operaciones Relacionales]
```c
<op_rel>  ::= <expresion> KW_IN <expresion>
            | <expresion> OP_EQUAL <expresion>
            | <expresion> OP_NOT_EQUAL <expresion> 
            | KW_EMPTY <espresion>
            | <expresión> KW_ANY <exprecion>
            | <expresion> "==" <expresion>
            | <expresion> "!=" <expresion>
```

### Expresiones: Operaciones Lógicas
[Def evaluación perezosa e ingenuas]

La evaluación en cortocircuito (perezosa) determina el resultado de una expresión sin necesidad de evaluar la totalidad de los operandos o los operadores. Por el contrario, la evaluación ingenua evalúa sistemáticamente todas las subexpresiones y operandos antes de aplicar el operador lógico final, incluso si el primer operando ya determinó de forma concluyente el valor de la expresión.

[poner aquí la BNF de Operaciones Lógicas]

```c

<op_log>  ::= <OP_rel>
            | KW_NOT <op_log>
            | <op_log> KW_AND <op_rel>
            | <op_log> KW_OR <OP_rel>
```

## 3.5.4 Estructuras de Control
### El control del flujo de ejecución de instrucciones permite alterar el orden en el que cada sentencia es evaluada.

### Alternativa
[Explique sobre la ambigüedad de la sentencia if]

La ambigüedad en el diseño clásico de la sentencia `if` (dangling-else) ocurre porque una gramática genera una forma sentencial para la cual existen dos o más árboles de análisis sintáctico distintos. Esto ocurre cuando un `if` sin `else` está anidado dentro del bloque de otro `if` externo; el compilador no puede determinar de manera unívoca a cuál de los dos `if` pertenece el else subsecuente.

[poner aquí la BNF y EBNF del if y explque sobre la ambiguedad o no de su propuesta]

```c
<sent_if>  ::= KW_IF  '(' <op_log> ')' <bloque> KW_ELSE <bloque>
```

El uso estricto del no terminal `<bloque>` definido con los marcadores explícitos DELIM_DO y DELIM_END soluciona formalmente la ambigüedad. Al exigir un final de bloque explícito, el árbol de análisis sintáctico se cierra forzosamente, por lo que un else posterior solo puede enlazarse a la estructura if que actualmente permanezca abierta.

```c
<bloque>  ::= DELIM_DO <lista_sent> DELIM_END
```

### Ciclos
[poner aquí las BNF de las estructuras de control de ciclos que hayan definido]

```c
<sent_while>  ::= KW_WHILE '(' <op_log> ')' <bloque>

<sent_for>    ::= KW_FOR '(' <op_log> ')' <bloque>
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
Un analizador sintáctico ascendente (bottom-up parser) construye un árbol de análisis sintáctico comenzando en las hojas y avanzando hacia la raíz. Este orden de análisis corresponde al inverso de una derivación por la extrema derecha. Es decir, las formas sentenciales de la derivación se producen en orden desde la última hasta la primera.
En términos de la derivación, un analizador ascendente se puede describir de la siguiente manera: Dada una forma sentencial derecha, el analizador debe determinar qué subcadena es el lado derecho (RHS) de la regla de la gramática que debe reducirse a su lado izquierdo (LHS) para producir la forma sentencial anterior en la derivación por la derecha. Por ejemplo, el primer paso para un analizador ascendente es determinar qué subcadena de la oración inicial dada es el RHS que debe reducirse a su LHS correspondiente para obtener la penúltima forma sentencial de la derivación. 
Elegir el RHS correcto a reducir no es sencillo, pues una forma sentencial derecha dada puede incluir más de un RHS de la gramática del lenguaje que se está analizando. El RHS correcto se denomina manejador (handle).
1. Definición de manejador (handle):
γ
γ es el manejador (handle) de la forma sentencial derecha 
β
β si y solo si:

S
⇒
∗
r
m
A
w
⇒
d
e
r
r
m
α
w
S⇒ 
∗
 rmAw⇒ 
der
​
 rmαw
La idea es identificar la producción que se utiliza en el último paso de una derivación por la derecha. Más precisamente, si la producción es 
A
→
α
A→α, entonces 
α
α es el manejador de la forma sentencial derecha 
r
m
α
w
rmαw.

Esta definición, por sí sola, no siempre facilita la identificación del manejador de una forma sentencial derecha. Para comprender mejor este concepto, se definen las frases que están relacionadas con los manejadores.

2. Definición de frase simple: 
γ
γ es una frase simple de la forma sentencial derecha 
β
β si y solo si:

S
⇒
∗
α
A
w
⇒
d
e
r
α
γ
w
S⇒ 
∗
 αAw⇒ 
der
​
 αγw
donde 
A
→
γ
A→γ es la producción aplicada en el último paso de la derivación por la derecha.

Aquí, 
⇒
∗
⇒ 
∗
  significa uno o más pasos de derivación según la convención de tu material; estrictamente, en la notación habitual, 
⇒
∗
⇒ 
∗
  permite cero o más pasos, mientras que 
⇒
+
⇒ 
+
  indica uno o más pasos.
### Funcionamiento de los analizadores LR
Los analizadores sintácticos LR constituyen el algoritmo ascendente (bottom-up) mas difundido y utilizado en la implementación de procesadores de lenguajes deprogramación. Dadas sus características fundamentales: 
- L (Left-to-right) que indica que el flujo de tokens de entrada se explora y analiza de izquierda a derecha. 
- R (Rightmost derivarion): Especifica que la secuencia de análisis construye la inversa de una derivación por extrema derecha.
Todo analizador LR es un reconocedor basado en un Autómata de Pila determinista (PDA). En esta arquitectura, la pila almacena una secuencia alternada de símbolos gramaticales (X) y símbolos de estado numéricos (S): **$$S_0 \, X_1 \, S_1 \, X_2 \, S_2 \dots X_m \, S_m \quad \text{(tope)}$$**
Donde S0 representa el estado inicial del analizador y el estado Sm sera la información relevante acumulada del análisis sintáctico procesado hasta el momento.
Una configuración completa del analizador LR se representa como el par ordenado compuesto por la pila y la cadena restante: **$$(S_0 \, X_1 \, S_1 \, X_2 \, S_2 \dots X_m \, S_m, \quad a_i \, a_{i+1} \dots a_n \$)$$**, donde $ se coloca allí durante la inicialización del analizador para la terminación normal del proceso.
Dada la configuración se define formalmente el proceso del analizador LR basado en la tabla de análisis:
Una tabla de análisis LR consta de dos partes, llamadas ACTION y GOTO. La parte ACTION tiene símbolos de estado como etiquetas de gilas y los símbolos terminales de la gramática como etiquetas de columnas. Dado el estado actual del analizador y el siguiente símbolo de entrada, la tabla especifica qué debe hacer el analizador.   
### Funcionamiento del analizador generado por Bison
El analizador generado por Bison es un parser ascendente (bottom-up) determinista de la familia LALR(1) (Look-Ahead Left-to-Right with 1 token lookahead). Su tarea principal es reconstruir la inversa de una derivación por la extrema derecha a partir del flujo lineal de tokens que le provee el analizador léxico Flex.

Internamente Bison mediante la función yyparse() dirige la ejecución.  Cada vez que la lógica del autómata necesita un nuevo token, invoca a yylex(), para escanear el código fuente en Aleph y le retorna el valor entero del token junto con sus atributos semánticos en yyval.

El parser LALR(1) utiliza un Autómata con Pila donde cada entrada alterna un símbolo gramatical y un símbolo de estado numérico34: $$(S_0, \, X_1 S_1 \, X_2 S_2 \, \dots \, X_m S_m, \quad a_i a_{i+1} \dots \$)$$. 

En cada paso, el parser consulta las tablas generadas durante la compilación de aleph.y mediante: 
- Acción Shift (Desplazamiento): Si la secuencia de tokens aún no completa el lado derecho (RHS) de una regla, el token $a_i$ se desplaza a la pila junto con un nuevo estado $S_n$, y se solicita el siguiente token a Flex.
- Acción Reduce (Reducción):Ocurre cuando en la cima de la pila se ha completado el lado derecho de una regla de producción. Bison retira de la pila los símbolos del manejador junto con sus estados, apila el símbolo no terminal del lado izquierdo (LHS) y consulta la tabla GOTO para determinar el nuevo estado de la cima. Se ejecutan las acciones en C asociadas a la regla.
- Aceptar (Accept) / Error: Se acepta si se reduce la regla inicial (`programa`) al llegar al fin de archivo (`$`). Si no hay transición válida, se invoca a `yyerror()`.

### Primer Analizador Sintáctico de Aleph

| Lexema               | Token             | Descripción                                                                              |
| -------------------- | ----------------- | ---------------------------------------------------------------------------------------- |
| `set`                | `KW_SET`          | Tipo de dato / Palabra reservada: conjunto no duplicado.                                 |
| `list`               | `KW_LIST`         | Tipo de dato / Palabra reservada: secuencia o lista ordenada.                            |
| `while`              | `KW_WHILE`        | Sentencia de control: inicio de iteración condicionada.                                  |
| `for`                | `KW_FOR`          | Sentencia de control: iteración sobre elementos de colección.                            |
| `if`                 | `KW_IF`           | Sentencia de control: bifurcación condicional de flujo.                                  |
| `else`               | `KW_ELSE`         | Sentencia de control: rama alternativa de selección.                                     |
| `do`                 | `DELIM_DO`        | Delimitador de bloque: apertura de cuerpo de control o bucle.                            |
| `end`                | `DELIM_END`       | Delimitador de bloque: cierre estructural de bloques o funciones.                        |
| `break`              | `KW_BREAK`        | Sentencia de control: interrupción forzada de iteración.                                 |
| `return`             | `KW_RETURN`       | Sentencia de control: retorno explícito de valor de función.                             |
| `let`                | `KW_LET`          | Declaración: desestructuración o nombramiento formal de componentes.                     |
| `in`                 | `OP_IN`           | Operador relacional: evaluación de pertenencia conjuntista o recorrido.                  |
| `where`              | `KW_WHERE`        | Sentencia de control: cláusula de filtrado condicional en bucles.                        |
| `empty`              | `KW_EMPTY`        | Literal / Constante: representa el conjunto o lista sin elementos.                       |
| `union`              | `OP_UNION`        | Operador de colección: cálculo de la unión algebraica entre conjuntos.                   |
| `intersection`       | `OP_INTERSECTION` | Operador de colección: cálculo de la intersección entre conjuntos.                       |
| `contain`            | `OP_CONTAIN`      | Operador de colección: verificación de pertenencia de un elemento en un conjunto o lista.|
| `relation`           | `OP_RELATION`     | Operador relacional: define o evalúa una relación entre expresiones.                     |
| `difference`         | `OP_DIFFERENCE`   | Operador de colección: cálculo de la diferencia algebraica entre conjuntos.              |
| `any`                | `KW_ANY`          | Función primitiva: verificación existencial en colecciones.                              |
| `append`             | `KW_APPEND`       | Función primitiva: inserción de elemento al final de una lista.                          |
| `insert`             | `KW_INSERT`       | Función primitiva: incorporación de un elemento a un conjunto.                           |
| `&&`                 | `KW_AND`          | Operador lógico: conjunción (Y lógico).                                                  |
| `!`                  | `KW_NOT`          | Operador lógico: negación unaria.                                                        |
| `\|\|`               | `KW_OR`           | Operador lógico: disyunción (O lógico).                                                  |
| `:`                  | `DELIM_COLON`     | Símbolo especial: delimitador de apertura del bloque de función.                         |
| `=`                  | `OP_ASSIGN`       | Operador básico: asignación destructiva de valor a identificador.                        |
| `==`                 | `OP_EQUAL`        | Operador relacional: prueba de igualdad estructural estricta.                            |
| `!=`                 | `OP_NOT_EQUAL`    | Operador relacional: prueba de desigualdad estructural estricta.                         |
| `;`                  | `DELIM_SEMICOLON` | Delimitador sintáctico: terminación explícita de sentencia.                              |
| `,`                  | `DELIM_COMMA`     | Separador sintáctico: división posicional en listas, conjuntos o parámetros.             |
| `(`                  | `DELIM_LPAREN`    | Delimitador sintáctico: apertura de tuplas, condiciones o llamadas.                      |
| `)`                  | `DELIM_RPAREN`    | Delimitador sintáctico: cierre de tuplas, condiciones o llamadas.                        |
| `{`                  | `DELIM_LBRACE`    | Delimitador sintáctico: apertura de especificación literal de conjunto.                  |
| `}`                  | `DELIM_RBRACE`    | Delimitador sintáctico: cierre de especificación literal de conjunto.                    |
| `[`                  | `DELIM_LBRACKET`  | Delimitador sintáctico: apertura de especificación literal de lista.                     |
| `]`                  | `DELIM_RBRACKET`  | Delimitador sintáctico: cierre de especificación literal de lista.                       |
| `'0'`,`'1'`,`'a'`    | `LIT_ELEMENT`     | Literales: representación atómica de un elemento base para conformar colecciones.        |
| `[A-Z][a-zA-Z0-9_]*` | `ID`              | Identificadores: nombres de variables definidos por el usuario iniciados en mayúscula.   |

```c
<Aleph>  ::= <lts_sent>

<lts_sent>  ::= <sent> ';' | <sent> ';' <lts_sent>

<sent>            ::= <asignacion>
                    | <asignacion_mul>
                    | <expresion> 
                    | <sent_while> 
                    | <sent_if> 
                    | <sent_for>

<asignacion> ::= ID '=' <esprecion> | <asignacino_mult>

asignacin_mult>   ::= KW_LET <lts_id> '=' <lts_expresiones>

<lts_id>           ::= ID | ID ',' <lts_id>

<lts_expresiones>  ::= <expresion> | <expresion> ',' <lts_espresiones>

<expreciones>  ::= ID
                 | LIT_ELEM
                 | <lts_lista>
                 | <lts_conjunto>
                 | <expresion> OP_UNION <expresion>
                 | <expresión> OP_INTERSECTION <exprecion>
                 | <expresión> OP_RELATION <exprecion>
                 | <expresion> OP_DIFFERENCE <expresion>
                 | <expresión> KW_APPEND <exprecion>
                 | <expresión> KW_INSERT <exprecion>

<lts_lista>       ::= '[' <lts_expresiones> ']'
                    | '[' ']'

<lts_conjunto>    ::= '{' <lts_expresiones> ´}´
                    | '{' '}'

<sent_while>  ::= KW_WHILE '(' <op_log> ')' <bloque>

<sent_for>    ::= KW_FOR '(' <op_log> ')' <bloque>

<sent_if>  ::= KW_IF  '(' <op_log> ')' <bloque> KW_ELSE <bloque>

<op_log>  ::= <OP_rel>
            | KW_NOT <op_log>
            | <op_log> KW_AND <op_rel>
            | <op_log> KW_OR <OP_rel>

<op_rel>  ::= <expresion> KW_IN <expresion>
            | <expresion> OP_EQUAL <expresion>
            | <expresion> OP_NOT_EQUAL <expresion> 
            | KW_EMPTY <espresion>
            | <expresión> KW_ANY <exprecion>
            | <expresion> "==" <expresion>
            | <expresion> "!=" <expresion>

<bloque>          ::= DELIM_DO <lista_sent> DELIM_END

```
