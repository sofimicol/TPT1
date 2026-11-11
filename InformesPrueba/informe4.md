##### 3.7.1. Semántica Estática

La semántica estática abarca aquellas validaciones y reglas de significado que podemos comprobar en el programa fuente antes de su ejecución real. A diferencia del análisis sintáctico (que solo verifica que las oraciones estén bien formadas según la BNF), la semántica estática de Aleph se encarga de dar sentido a esas estructuras, comprobando, por ejemplo, que las variables utilizadas hayan sido previamente declaradas, que las operaciones algebraicas reciban operandos compatibles y que los identificadores respeten las reglas de alcance (scope).

Dado que hemos diseñado Aleph como un intérprete puro, efectuamos gran parte de nuestro análisis estático de manera integrada durante la fase de construcción del Árbol de Sintaxis Abstracta (AST). El analizador sintáctico (Bison) guía la estructura, pero son las acciones semánticas asociadas a cada regla las que se encargan de propagar la información de tipos y construir los nodos en memoria para su posterior evaluación.

###### Gramática de Atributos

Para formalizar cómo fluye la información de contexto y de tipos a través de la sintaxis de Aleph, nos basamos en el concepto de Gramáticas de Atributos propuesto por Knuth (1968) [Louden, 2004, p. 101]. Una gramática de atributos es una extensión de las gramáticas libres de contexto, donde asociamos a cada símbolo de la gramática (terminal o no terminal) un conjunto de propiedades denominadas "atributos", y a cada regla de producción le asignamos funciones semánticas que especifican cómo calcular dichos atributos.

En nuestro diseño diferenciamos dos tipos de atributos:
*   **Atributos Sintetizados:** Son aquellos cuyo valor en el nodo padre se calcula exclusivamente a partir de los valores de los atributos de sus nodos hijos. En implementaciones de análisis ascendente (bottom-up) como la generada por Bison, estos atributos son los más naturales de implementar, ya que la información fluye desde las hojas (los lexemas) hacia la raíz del árbol.
*   **Atributos Heredados:** Son aquellos cuyo valor se calcula tomando información del nodo padre o de los nodos hermanos. Permiten que la información fluya "hacia abajo" o "lateralmente" en el árbol de derivación.

En la arquitectura de nuestro intérprete Aleph, hemos optado por utilizar fundamentalmente **atributos sintetizados**. Cada vez que Flex reconoce un literal o un identificador, emite un token y sintetiza su valor (por ejemplo, el carácter en sí, o el nombre de la variable) hacia Bison mediante la variable `yylval`. Luego, a medida que Bison reduce las reglas gramaticales de abajo hacia arriba, nuestras acciones en C recolectan esos punteros de los hijos para sintetizar un nuevo nodo (una estructura en C tipo `struct ast`) que se pasa al nivel superior mediante `$$`.

###### Gramática de atributos para las expresiones de Aleph

Para ilustrar nuestro modelo, definimos a continuación la gramática de atributos sintetizados para las expresiones algebraicas básicas de Aleph. Sea `.val` el atributo que representa el puntero al nodo del AST que estamos construyendo en memoria, definimos las siguientes reglas semánticas para sintetizar el sub-árbol:

| Producción BNF | Función Semántica (Atributo Sintetizado) |
| :--- | :--- |
| `<expresion> ::= <expresion> OP_UNION <termino>` | `<expresion>_padre.val = crear_nodo_operacion(OP_UNION, <expresion>_hijo.val, <termino>.val)` |
| `<expresion> ::= <expresion> OP_INTERSECTION <termino>` | `<expresion>_padre.val = crear_nodo_operacion(OP_INTERSECTION, <expresion>_hijo.val, <termino>.val)` |
| `<expresion> ::= <expresion> OP_DIFFERENCE <termino>` | `<expresion>_padre.val = crear_nodo_operacion(OP_DIFFERENCE, <expresion>_hijo.val, <termino>.val)` |
| `<expresion> ::= <termino>` | `<expresion>.val = <termino>.val` |
| `<termino> ::= IDENTIFIER` | `<termino>.val = crear_nodo_referencia(IDENTIFIER.lexema)` |
| `<termino> ::= LIT_ELEMENT` | `<termino>.val = crear_nodo_literal_elemento(LIT_ELEMENT.lexema)` |
| `<termino> ::= <lts_conjunto>` | `<termino>.val = <lts_conjunto>.val` |
| `<lts_conjunto> ::= DELIM_LBRACE <lts_expresiones> DELIM_RBRACE`| `<lts_conjunto>.val = crear_nodo_conjunto(<lts_expresiones>.val)` |

En este esquema, cuando analizamos una operación de conjuntos (por ejemplo la unión), la función `crear_nodo_operacion()` encapsula la creación dinámica en memoria de la estructura. El puntero resultante se convierte en el atributo sintetizado del lado izquierdo de la regla (`<expresion>_padre.val`), permitiendo que el árbol crezca progresivamente hasta abarcar la sentencia completa, dejando la estructura lista para ser verificada y evaluada por la Semántica Dinámica.

## 3.7.2. Semántica Dinámica
[Poner aquí Def Semántica Dinámica]

La semántica dinámica define el significado computacional del código en tiempo de ejecución. Su propósito es modelar rigurosamente cómo se transforman el estado de la máquina, los valores en memoria y el flujo de control secuencial a medida que las sentencias son evaluadas dinámicamente por el intérprete de Aleph.

Semántica Operacional
[Poner aquí Def Semántica Operacional]

La semántica operacional describe el significado de un lenguaje definiendo el efecto secuencial de sus sentencias sobre el estado de una máquina abstracta o virtual. En lugar de limitarse a mapeos puramente matemáticos, detalla los pasos algorítmicos que modifican las variables y las estructuras de datos en memoria instrucción por instrucción.

Descripción las estructuras de control de Aleph
[Poner aquí definición de lenguaje intermedio para describir Aleph]

Definimos el estado de la máquina abstracta $E$ como el mapeo activo en memoria que vincula las variables (identificadores) con sus estructuras asignadas (listas, conjuntos).

[Poner aquí las descripciones de las estructuras de control de Apleh con semántica operacional]

* Bifurcación Condicional (`if-else`)
  La semántica operacional de la estructura `<sent_if>` en Aleph se evalúa en base al estado actual σ :
  1.  Se evalúa la condición booleana `<condicion>` en el estado σ.
  2.  Rama verdadera: Si la condición se evalúa como true, el intérprete ingresa al primer bloque (`do ...`), ejecutando secuencialmente su lista de sentencias. Esto produce modificaciones en la memoria y genera un nuevo estado σ. Al alcanzar el final del bloque, la instrucción concluye.
  3.  Rama falsa (con `else`): Si la condición es `false` y existe una cláusula `else`, el intérprete omite el primer bloque y ejecuta el segundo, transicionando a un nuevo estado σ''. Si no existe una cláusula else, el estado de la memoria permanece inalterado como σ y la instrucción termina.
* Iteración Condicional (`while`)
  El ciclo `<sent_while>` evalúa su permanencia dinámicamente sobre la memoria:
  1. Se evalúa la `<condicion>` sobre el estado actual σ.
  2. Si la condición resulta false, el ciclo se interrumpe y la ejecución continúa con el estado σ original.
  3. Si la condición resulta `true`, se ejecutan todas las instrucciones contenidas en el bloque, lo que transforma el estado de la memoria a σ. Una vez finalizado el bloque, el control retorna forzosamente al paso 1, reevaluando la misma instrucción `while` pero sobre el nuevo estado σ.

Semántica Denotacional
[Poner aquí Def Semántica Denotacional]

La semántica denotacional representa el significado formal del código asociando cada construcción sintáctica con objetos y funciones matemáticas rigurosas, independizándose completamente de cualquier arquitectura de máquina simulada o mapa de memoria.

Descripción las estructuras de control de Aleph
[Poner aquí las descripciones de las estructuras de control de Apleh con semántica denotacional]

A. Evaluación de Colecciones (Conjuntos y Listas)
  * Lectura de variable (ID): El intérprete busca el identificador en el estado actual de la memoria $\sigma$. Si existe, retorna la lista o conjunto asociado; si no, arroja       un error de ejecución.
  * Unión de Conjuntos (OP_UNION):
    1. Se evalúa el operando izquierdo para obtener una colección $C_1$.
    2. Se evalúa el operando derecho para obtener una colección $C_2$.
    3. El intérprete crea y retorna un nuevo conjunto que contiene todos los elementos presentes en $C_1$ y $C_2$, omitiendo cualquier elemento duplicado.
  * Diferencia de Conjuntos (OP_DIFFERENCE):
    1. Se evalúa el operando izquierdo ($C_1$) y el derecho ($C_2$).
    2. El intérprete retorna un nuevo conjunto que contiene únicamente los elementos de $C_1$ que no están presentes en $C_2$.
  * Modificación (KW_INSERT / KW_APPEND):
    1. Se evalúa la colección base obteniendo $C$.
    2. Se evalúa el término derecho obteniendo el elemento $e$.Si es KW_APPEND (listas), retorna una nueva colección con $e$ concatenado en la última posición.
    3. Si es KW_INSERT (conjuntos), retorna la colección con $e$ incluido solo si no existía previamente.2.

B. Evaluación de Expresiones Relacionales
  * Pertenencia (OP_IN):
    1. Se evalúa el término izquierdo obteniendo un elemento $e$.
    2. Se evalúa la expresión derecha obteniendo la colección $C$.
    3. El intérprete recorre $C$. Si encuentra una coincidencia exacta con $e$, la instrucción finaliza y retorna true. Si agota la colección sin coincidencias, retorna false.
  * Comprobación de Vacío (KW_EMPTY):
    1. Se evalúa la expresión obteniendo una colección $C$.
    2. Si el tamaño de $C$ es igual a 0 (no contiene elementos), retorna true; de lo contrario, retorna false.3.

C. Semántica de Comandos de Asignación
  * Asignación Simple (ID = expr):
    1. Se evalúa secuencialmente la expresión expr hasta resolverse en un valor final $v$ (una colección, elemento o booleano).
    2. El intérprete actualiza el estado de la memoria $\sigma$, vinculando el identificador ID con el nuevo valor $v$.
    3. Si ID ya existía en memoria, su referencia anterior se descarta y es sobrescrita. Si no existía, se reserva el espacio y se registra en el entorno actual.

## 3.8 Análisis Semántico
## 3.8.1 Construcción del AST y evaluación de expresiones
[Mostrar y explicar la estructura de datos y módulos que se usan para la construcción del AST]

[Mostrar y explicar la estructura de datos y módulos utilizados para evaluar los AST en Aleph]

[Entrega de código en el repo correspondiente: Evaluación de literales listas, conjuntos (sin elementos repetidos) y elemento (con sus operaciones no excluyente)]
