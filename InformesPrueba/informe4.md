## 3.7 Semántica: Métodos Formales
En esta sección se especificará la semántica, el significado, de las construcciones sintácticas descriptas en la sección anterior del presente informe. Las herramientas formales para esta tarea no son tan conocidas como las herramientas formales para la sintaxis. No obstante, estas herramientas pueden ser utilizadas para complementar las definiciones de algunas construcciones que exijan una mayor precisión en su descripción semántica que aquella que puede brindarse mediante una definición coloquial de su funcionamiento.

[poner aquí Def Semántica]

La semántica formal describe de manera rigurosa y matemática el significado de los programas y cómo se alteran los estados durante su ejecución. Mientras que la sintaxis establece las reglas estrictas de formación del código y su jerarquía, la semántica determina los cómputos precisos que se realizan al evaluar dichas secuencias sobre la máquina virtual o el entorno de ejecución.

## 3.7.1. Semántica Estática
[Poner aquí Def Semántica Estática]

Gramática de Atributos
[Poner aquí Def Gramática de Atributos]

Una gramática de atributos es una extensión formal de las GLC diseñada para describir la semántica estática de un lenguaje. Este modelo asocia atributos (como tipos de datos o valores) a los símbolos terminales y no terminales de la gramática. Adicionalmente, adjunta funciones semánticas a las reglas de producción para especificar cómo se sintetizan o heredan dichos atributos a lo largo del árbol sintáctico, y utiliza funciones de predicado para verificar que se cumplan las restricciones lógicas y de tipado.

Gramática de atributos para las expresiones de Aleph
[Poner aquí las gramáticas de atributos de las operaciones de alpeh]

1. Operaciones Algebraicas de Conjuntos

Regla Sintáctica:
``math
$$<expresion>_0 ::= <expresion>_1 \text{ OP\_UNION } <termino>$$
``
(Aplica idéntico para OP_INTERSECTION y OP_DIFFERENCE)
Reglas Semánticas y Atributos:
``math
$$\begin{aligned} <expresion>_1.\text{env} &\leftarrow <expresion>_0.\text{env} \\ <termino>.\text{env} &\leftarrow <expresion>_0.\text{env} \\ <expresion>_0.\text{nodo} &\leftarrow \text{newast}(\text{OP\_UNION}, <expresion>_1.\text{nodo}, <termino>.\text{nodo}) \\ <expresion>_0.\text{tipo} &\leftarrow \begin{cases} \text{set} & \text{si } (<expresion>_1.\text{tipo} \in \{\text{set}, \text{empty}\}) \land (<termino>.\text{tipo} \in \{\text{set}, \text{empty}\}) \\  \text{error\_type} & \text{en otro caso (Error de tipos)} \end{cases} \end{aligned}$$
``

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

## 3.8 Análisis Semántico
## 3.8.1 Construcción del AST y evaluación de expresiones
[Mostrar y explicar la estructura de datos y módulos que se usan para la construcción del AST]

[Mostrar y explicar la estructura de datos y módulos utilizados para evaluar los AST en Aleph]

[Entrega de código en el repo correspondiente: Evaluación de literales listas, conjuntos (sin elementos repetidos) y elemento (con sus operaciones no excluyente)]
