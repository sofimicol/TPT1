### 3.8.2 Variables y la Tabla de Símbolos

En el diseño de lenguajes de programación imperativos, una **variable** es una abstracción de una celda o colección de celdas de memoria de la computadora que el programador define y nombra explícitamente para almacenar y modificar valores durante la ejecución [Sebesta, 2012].

Formalmente, caracterizamos una variable mediante una **seis-upla de atributos**:

$$\text{Variable} = (\text{Nombre}, \text{Dirección}, \text{Valor}, \text{Tipo}, \text{Tiempo de vida}, \text{Alcance})$$

A continuación describimos cada una de sus componentes y su correspondencia en Aleph:

1. **Nombre (Identificador):** Es la cadena de caracteres con la cual el programador referencia la variable dentro del código fuente. En Aleph, los identificadores deben comenzar con una letra mayúscula seguida de letras, dígitos o guiones bajos (`[A-Z][a-zA-Z0-9_]*`), y se almacenan internamente mediante nuestra estructura de cadenas dinámicas `str` (`TAD_str`).
2. **Dirección (Localidad o *l-value*):** Es la localidad de memoria física o virtual asociada a la variable. Una misma variable puede asociarse a distintas direcciones en diferentes momentos de la ejecución, y también es posible que múltiples nombres referencien una misma dirección (*aliasing*). En Aleph, la dirección lógica de una variable corresponde a su entrada dentro de la Tabla de Símbolos junto con la referencia en memoria dinámica (*heap*) al nodo `TNodo` (`Tdata`) que almacena su contenido.
3. **Valor (*r-value*):** Es el contenido concreto almacenado en la celda o estructura de memoria asociada a la variable, interpretado de acuerdo con su tipo. En Aleph, el valor está representado por un árbol dinámico `Tdata`, el cual puede ser un elemento atómico (`STR`), un conjunto sin repetidos (`SET`) o una lista ordenada (`LIST`).
4. **Tipo:** Determina el dominio o conjunto de valores válidos que la variable puede almacenar, así como las operaciones primitivas permitidas sobre ella. En Aleph, el tipo se identifica mediante el atributo `nodeType` (`STR = 1`, `SET = 2`, `LIST = 3`) y se registra en la Tabla de Símbolos para efectuar el chequeo de compatibilidad en tiempo de ejecución.
5. **Tiempo de vida (*Lifetime*):** Es el intervalo temporal durante la ejecución del programa en el cual la variable permanece ligada a una localidad específica de memoria; comienza cuando se le asigna almacenamiento y finaliza cuando dicha memoria es liberada.
6. **Alcance (*Scope*):** Es el rango o región textual del programa en la que la variable resulta visible y puede ser referenciada mediante su identificador.

#### Tabla de símbolos

La **Tabla de Símbolos** es la estructura de datos central del procesador de lenguajes que opera como un diccionario dinámico encargado de registrar los nombres (identificadores) presentes en un programa y asociarlos con sus atributos y ligaduras activas —tales como su tipo, alcance y valor o ubicación en memoria— [Louden, 2004, pp. 128-129; Levine, 2009, p. 61].

Sus operaciones fundamentales consisten en:
* **Inserción:** Registrar un nuevo identificador junto con sus atributos cuando se define por primera vez.
* **Búsqueda (*lookup*):** Localizar un identificador existente para recuperar su tipo y su valor actual durante la evaluación de una expresión.
* **Actualización:** Modificar el valor (y, en lenguajes con tipado dinámico, el tipo) asociado a un identificador existente al ejecutarse una nueva sentencia de asignación.

**Implementación de la Tabla de Símbolos en Aleph**

En nuestro intérprete de Aleph, implementamos la Tabla de Símbolos en un módulo dedicado (`TAD_symtab.h` y `TAD_symtab.c`) integrado directamente con nuestras estructuras base `TAD_str` y `TAD_ast`. Optamos por una **lista enlazada dinámica de registros (`TSimbolo`)**, lo cual evita límites fijos de capacidad y mantiene coherencia arquitectónica con el resto de nuestros Tipos Abstractos de Datos.

Cada nodo de la tabla almacena el nombre completo del identificador (`str`), el tipo actual del dato (`int`), un puntero al valor almacenado en memoria dinámica (`Tdata`) y el enlace al siguiente registro. Al ejecutar una asignación (`ID = expresion`), el intérprete busca el nombre en la tabla: si ya existe, libera la memoria del valor previo con `free_ast`, almacena una copia profunda del nuevo resultado mediante `copy_ast` y actualiza su tipo; si no existe, instancia un nuevo nodo `TSimbolo` y lo incorpora a la tabla.

A continuación mostramos la estructura de datos y los módulos principales de la Tabla de Símbolos implementados para el intérprete de Aleph:

```c
/* Definición en TAD_symtab.h */
#include "TAD_ast.h"

typedef struct simbolo {
    str nombre;             /* Identificador de la variable (TAD_str) */
    int tipo;               /* Tipo dinámico actual: STR (1), SET (2), LIST (3) */
    Tdata valor;            /* Puntero al árbol de datos (TAD_ast) */
    struct simbolo *next;   /* Enlace al siguiente registro de la tabla */
} TSimbolo;

typedef TSimbolo* TTablaSimbolos;
```
```
/* Operaciones principales en TAD_symtab.c */
TSimbolo* buscar_simbolo(TTablaSimbolos tabla, str nombre) {
    TSimbolo *aux = tabla;
    while (aux != NULL) {
        if (compare_str(aux->nombre, nombre) == 1) {
            return aux;
        }
        aux = aux->next;
    }
    return NULL;
}

TSimbolo* insertar_simbolo(TTablaSimbolos *tabla, str nombre, Tdata valor) {
    TSimbolo *existente = buscar_simbolo(*tabla, nombre);
    if (existente != NULL) {
        /* Actualización de variable existente: liberamos el valor anterior */
        free_ast(existente->valor);
        existente->valor = copy_ast(valor);
        existente->tipo = return_type(valor);
        return existente;
    }

    /* Creación de una nueva entrada en la tabla */
    TSimbolo *nuevo = (TSimbolo*)malloc(sizeof(TSimbolo));
    nuevo->nombre = copy_str(nombre);
    nuevo->valor = copy_ast(valor);
    nuevo->tipo = return_type(valor);
    nuevo->next = *tabla;
    *tabla = nuevo;
    return nuevo;
}
```
#### Ejemplo de uso en código Aleph
Al interpretar la siguiente secuencia de sentencias en Aleph:

Q = {'a', 'b'};

Sigma = {'0', '1'};

Estados = [Q, Sigma];

Q = Q union {'c'};

La Tabla de Símbolos registra inicialmente las entradas Q (tipo SET, valor {'a', 'b'}), Sigma (tipo SET, valor {'0', '1'}) y Estados (tipo LIST, valor [{'a', 'b'}, {'0', '1'}]). Al ejecutarse la última sentencia, buscar_simbolo recupera el valor actual de Q para resolver la unión y insertar_simbolo actualiza la entrada existente de Q en memoria con el nuevo conjunto {'a', 'b', 'c'}.


### 3.8.3 Ligaduras

En la semántica de los lenguajes de programación, definimos una **ligadura** (*binding*) como la asociación entre una entidad del programa —como un identificador, un operador o un símbolo— y un atributo o propiedad concreta —como su tipo, su dirección de memoria, su valor o la operación que representa— [Louden, 2004, p. 116; Sebesta, 2012].

El instante concreto del ciclo de vida de un programa en el cual se establece dicha asociación se denomina **tiempo de ligadura** (*binding time*) [Louden, 2004, p. 116]. Clasificamos los tiempos de ligadura en seis momentos posibles [Louden, 2004, p. 117]:

1. **Tiempo de definición del lenguaje:** Se establecen las propiedades inherentes al diseño del lenguaje, como las palabras reservadas, la sintaxis de las sentencias, el significado general de los operadores y el conjunto de tipos primitivos disponibles.
2. **Tiempo de implementación del lenguaje:** Quedan fijados los detalles dependientes del procesador del lenguaje o del hardware subyacente, como la representación interna en memoria de los tipos de datos o los límites de capacidad.
3. **Tiempo de traducción (tiempo de compilación):** El traductor asocia atributos a partir del código fuente escrito por el programador, como la ligadura de una variable a su tipo mediante declaraciones explícitas.
4. **Tiempo de ligado (*link time*):** Se vinculan referencias entre módulos compilados por separado y bibliotecas externas para conformar un ejecutable.
5. **Tiempo de carga (*load time*):** El cargador del sistema operativo asocia variables globales o estáticas a direcciones físicas o virtuales de memoria antes de iniciar la ejecución.
6. **Tiempo de ejecución (*run time*):** Se establecen o modifican asociaciones mientras el programa se ejecuta, como la ligadura de una variable a un valor mediante una asignación o la reserva de memoria para variables locales al invocar un subprograma.

#### Ligadura estática y ligadura dinámica

A partir de los tiempos anteriores, agrupamos toda ligadura en dos categorías fundamentales [Louden, 2004, pp. 116-117]:

* **Ligadura estática:** Es aquella que tiene lugar **antes del tiempo de ejecución** (en cualquiera de los primeros cinco tiempos de la lista) y permanece inalterada durante toda la ejecución del programa. Promueve una mayor **eficiencia** de ejecución y la detección temprana de errores, a costa de una menor flexibilidad.
* **Ligadura dinámica:** Es aquella que ocurre **durante el tiempo de ejecución** o que puede modificarse a lo largo del transcurso de la ejecución. Promueve una alta **flexibilidad** y adaptabilidad a los datos de entrada, pero introduce sobrecarga temporal y espacial debido a que el sistema de ejecución debe mantener y consultar metadatos (como etiquetas de tipo) mientras corre el programa.

#### Categorías de variables según su ligadura de almacenamiento

En función de cuándo y cómo se liga una variable a su dirección de memoria (y cuándo se libera dicha memoria), distinguimos cuatro categorías de variables [Louden, 2004, pp. 146-151; Sebesta, 2012]:

**1. Variables estáticas**
Son aquellas que se ligan a celdas de memoria antes de que comience la ejecución del programa y permanecen ligadas a esas mismas direcciones hasta que la ejecución finaliza (por ejemplo, las variables globales o declaradas con `static` en C).
* **Ventajas:** Máxima eficiencia de acceso (direccionamiento directo sin costo de reserva ni liberación en tiempo de ejecución) y soporte de **sensibilidad histórica** (conservan su valor entre sucesivas invocaciones de un subprograma).
* **Desventajas:** Reducida flexibilidad —no permiten implementar subprogramas recursivos con variables locales independientes por llamada— y menor aprovechamiento de la memoria, ya que su espacio no puede compartirse ni reutilizarse entre distintos bloques o subprogramas.

**2. Variables dinámicas de pila (*stack-dynamic*)**
Su ligadura de almacenamiento se establece dinámicamente en la pila de ejecución (*stack*) cuando el control alcanza su declaración (al entrar a un bloque o subprograma) y se libera automáticamente al finalizar dicho bloque (por ejemplo, las variables locales ordinarias en C o Java). Aunque su almacenamiento es dinámico, sus tipos suelen ligarse estáticamente.
* **Ventajas:** Permiten la **recursividad** (cada activación posee su propio espacio local en la pila) y optimizan el uso de memoria al reutilizar automáticamente el espacio de la pila entre subprogramas que no están activos simultáneamente.
* **Desventajas:** Introducen sobrecarga en tiempo de ejecución por la creación y destrucción de registros de activación, requieren direccionamiento indirecto respecto a la pila y carecen de sensibilidad histórica (su valor se pierde al salir del subprograma).

**3. Variables explícitas de montón dinámico (*explicit heap-dynamic*)**
Corresponden a bloques de memoria anónimos reservados y liberados explícitamente por el programador durante la ejecución en el área de memoria dinámica denominada montón (*heap*), mediante instrucciones dedicadas (como `malloc`/`free` en C o `new`/`delete` en C++), a los cuales se accede indirectamente mediante punteros o referencias [Louden, 2004, p. 150].
* **Ventajas:** Permiten construir estructuras de datos dinámicas cuyo tamaño y topología varían arbitrariamente en tiempo de ejecución (listas enlazadas, árboles, grafos).
* **Desventajas:** Exigen gestión manual compleja y propensa a errores críticos de confiabilidad —como **referencias pendientes** (*dangling pointers*) y **fugas de memoria** o basura (*memory leaks*) [Louden, 2004, pp. 164-165]—, presentan mayor costo temporal en la administración del montón y dificultan la depuración.

**4. Variables implícitas de montón dinámico (*implicit heap-dynamic*)**
Se ligan al almacenamiento del montón (*heap*) de forma automática exclusivamente cuando se les asigna un valor. Cada nueva asignación re-liga tanto la dirección de memoria como el tipo y el tamaño de la estructura almacenada, sin que el programador deba declarar tipos fijos ni invocar primitivas manuales de reserva o liberación (como ocurre en lenguajes interpretados como Python o JavaScript).
* **Ventajas:** Máxima flexibilidad y expresividad, permitiendo escribir código genérico y manipular estructuras de tamaño variable con una sintaxis limpia y libre de gestión manual de punteros.
* **Desventajas:** Conllevan una elevada sobrecarga en tiempo de ejecución por la gestión dinámica continua de memoria y atributos, e impiden detectar errores de tipo antes de ejecutar el programa.

#### Relación con la implementación de Aleph

Al haber diseñado Aleph como un **intérprete puro con front-end de análisis estático**, el reparto entre ligaduras estáticas y dinámicas queda organizado en dos niveles bien diferenciados:

* **Ligaduras estáticas en Aleph:**
  * *En tiempo de definición del lenguaje:* Fijamos el conjunto de palabras reservadas (`set`, `list`, `empty`), los operadores de colecciones (`union`, `inter`, `diff`, `append`, `insert`) y los tres tipos de datos nativos (`STR`, `SET`, `LIST`).
  * *En tiempo de implementación del lenguaje:* Definimos en C la representación física de los nodos del árbol (`TNodo` en `TAD_ast.h`), el encadenamiento de caracteres (`Tnodo` en `TAD_str.h`) y los registros de la Tabla de Símbolos (`TSimbolo` en `TAD_symtab.h`).
* **Ligaduras dinámicas en Aleph:**
  * *Ligadura de tipo y valor:* En el código escrito por el programador en Aleph, tanto el **valor** como el **tipo** de una variable se ligan dinámicamente en tiempo de ejecución al evaluarse una sentencia de asignación (`ID = expresion`). Si primero ejecutamos `A = {'a', 'b'};` y luego `A = ['a', 'b'];`, la entrada `A` en `TAD_symtab` actualiza dinámicamente su campo `tipo` de `SET` a `LIST` y reasigna su puntero `valor`.
  * *Categoría de variables en Aleph:* Desde la perspectiva del programador de Aleph, todas las variables del lenguaje se comportan como **variables implícitas de montón dinámico**. El usuario no declara tamaños máximos ni reserva memoria de forma manual: cada vez que evalúa una expresión o realiza una asignación, el intérprete gestiona transparentemente la creación en el montón y la liberación del valor anterior.
  * *Mecanismo interno en C:* Para materializar estas variables implícitas de montón dinámico en el nivel de implementación del intérprete en C, utilizamos internamente **variables explícitas de montón dinámico** (mediante `malloc`/`calloc` en `create_str`, `create_set`, `create_list`, `copy_ast` e `insertar_simbolo`, junto con `free`/`free_ast` para evitar fugas de memoria al reasignar). Asimismo, el puntero raíz de la Tabla de Símbolos global (`TTablaSimbolos tabla_global`) actúa como una **variable estática** dentro del intérprete en C, manteniéndose viva durante toda la ejecución de la sesión de Aleph, mientras que las variables auxiliares de las rutinas recursivas de evaluación (`eval`) son **variables dinámicas de pila** gestionadas por la pila de llamadas de C.

### 3.8.4 Ámbito

Al analizar cómo un programa gestiona y resuelve los nombres de sus variables, resulta fundamental distinguir entre la dimensión temporal de almacenamiento de una variable y la dimensión espacial o textual desde la cual puede referenciarse.

#### Tiempo de vida y alcance

* **Tiempo de vida (*Lifetime*):** Es el intervalo de tiempo durante la ejecución del programa en el cual una variable permanece ligada a una localidad específica de memoria [Pratt & Zelkowitz, p. 109]. Comienza cuando se reserva almacenamiento para la variable y finaliza cuando dicha memoria es desasignada o liberada.
* **Alcance (*Scope*):** Es el rango o conjunto de sentencias del programa dentro del cual una variable es visible y puede ser referenciada mediante su identificador [Louden, 2004, p. 122]. Una variable se denomina *local* en una unidad de programa o bloque si se declara en ella, y *no local* si es visible en dicha unidad pero fue declarada fuera de ella.

Aunque el alcance y el tiempo de vida suelen estar estrechamente vinculados, constituyen conceptos independientes que no siempre coinciden: por ejemplo, una variable local declarada como `static` en C posee un **alcance local** (solo es visible dentro de su función), pero un **tiempo de vida global** (su celda de memoria persiste durante toda la ejecución del programa) [Louden, 2004, p. 153]. Asimismo, cuando un bloque interno declara una variable con el mismo nombre que una externa, la declaración interna oculta temporalmente a la externa, generando una *apertura* o hueco de visibilidad dentro del alcance de esta última [Louden, 2004, p. 124].

#### Alcance estático y alcance dinámico

Según el criterio que utilice el lenguaje para resolver las referencias a variables no locales, distinguimos dos modelos de alcance:

* **Alcance estático (o alcance léxico):** Las reglas de alcance se determinan exclusivamente a partir de la estructura textual del código fuente, antes de la ejecución del programa [Louden, 2004, p. 122; Pratt & Zelkowitz, p. 337]. Cuando un bloque o subprograma referencia un identificador no local, el procesador busca su declaración primero en el ámbito local y, si no la encuentra, asciende sucesivamente por los bloques que lo contienen textualmente hasta llegar al ámbito global. Al depender únicamente del texto del programa, favorece la legibilidad, facilita la verificación estática y garantiza que cada referencia resuelva siempre a la misma declaración sin importar el flujo de llamadas.
* **Alcance dinámico:** El alcance de las variables se determina en tiempo de ejecución en función de la secuencia de llamadas activas a subprogramas (*cadena dinámica*) [Louden, 2004, p. 133; Pratt & Zelkowitz, p. 333]. Ante una referencia a un identificador no local, el intérprete aplica la *regla de la asociación más reciente*: busca primero en el entorno local del subprograma actual y, si no existe, retrocede por el subprograma que lo invocó, luego por el que invocó a este, y así sucesivamente hasta hallar una asociación activa [Pratt & Zelkowitz, pp. 333-334]. Su principal desventaja es que una misma referencia puede quedar ligada a variables distintas —e incluso de diferentes tipos— según la ruta de ejecución, lo que impide la verificación estática de tipos y reduce la legibilidad y confiabilidad del código [Louden, 2004, p. 133; Pratt & Zelkowitz, p. 334].

#### Ancestro estático y ancestro dinámico de un subprograma

La distinción entre ambos modelos de alcance se formaliza mediante las relaciones de parentesco entre subprogramas [Pratt & Zelkowitz, pp. 333, 340-341; Sebesta, 2012]:

* **Ancestro estático:** En lenguajes que permiten el anidamiento textual de subprogramas, si un subprograma $B$ está definido sintácticamente dentro del cuerpo de un subprograma $A$, decimos que $A$ es el *padre estático* de $B$. Por extensión, $A$ y todos los subprogramas (o el programa principal) que envuelven textualmente a $A$ conforman los **ancestros estáticos** de $B$. Durante la ejecución, el acceso a variables no locales bajo alcance estático sigue esta jerarquía de anidamiento mediante apuntadores de *cadena estática* [Pratt & Zelkowitz, pp. 340-341].
* **Ancestro dinámico:** Independientemente de cómo estén ubicados los subprogramas en el texto fuente, si durante la ejecución un subprograma $A$ invoca a un subprograma $B$ (ya sea de forma directa o a través de una secuencia intermedia de llamadas, como $A \to C \to B$), decimos que $A$ es un **ancestro dinámico** de $B$. El encadenamiento de registros de activación que registra este historial de invocaciones conforma la *cadena dinámica* en la pila de ejecución [Pratt & Zelkowitz, pp. 333, 340].

#### Relación con la implementación de Aleph

En nuestro intérprete de Aleph vinculamos estos conceptos tanto al estado actual de la implementación (evaluación de asignaciones y expresiones) como a la arquitectura general diseñada para bloques y subrutinas:

* **Alcance en la implementación actual (`TAD_symtab`):** En esta etapa del intérprete gestionamos los identificadores mediante una única Tabla de Símbolos global (`TTablaSimbolos tabla_global`). Toda variable definida a través de una sentencia de asignación (`ID = expresion`) adquiere **alcance global** desde el punto de su primera asignación en adelante (regla de definición antes de uso). Si una expresión referencia un identificador que aún no fue registrado en `tabla_global`, `buscar_simbolo` retorna `NULL` y el intérprete emite un error semántico.
* **Tiempo de vida en Aleph:** Diferenciamos dos niveles de tiempo de vida en memoria:
  1. *Entrada en la Tabla de Símbolos (`TSimbolo`):* Su tiempo de vida se inicia la primera vez que el identificador aparece a la izquierda de una asignación (`insertar_simbolo`) y finaliza al concluir la ejecución del programa, cuando se libera toda la tabla.
  2. *Valor dinámico en el montón (`Tdata`):* Cada estructura `TNodo` asociada al campo `valor` tiene un tiempo de vida acotado al lapso entre dos asignaciones sobre la misma variable. Al ejecutarse una reasignación (por ejemplo, `Q = Q union {'c'};`), `insertar_simbolo` libera el árbol previo con `free_ast` —poniendo fin a su tiempo de vida— y liga el identificador al nuevo árbol reservado con `copy_ast`.
* **Diseño de alcance y ancestros para bloques y subprogramas:** Conforme a la gramática de Aleph (estructuras de control delimitadas por `do ... end` y subrutinas como `afnd2afd`), adoptamos el **alcance estático (léxico)** para asegurar previsibilidad y legibilidad. Dado que en Aleph todas las subrutinas se definen al nivel superior del programa (sin anidamiento textual de funciones dentro de funciones), el único **ancestro estático** de cualquier subprograma es el ámbito global. En cambio, los **ancestros dinámicos** quedan determinados en tiempo de ejecución por la secuencia de llamadas activas entre subrutinas (por ejemplo, si el programa principal invoca a `afnd2afd` y esta llama a una función auxiliar). Esta arquitectura es directamente extensible sobre `TAD_symtab`: al incorporar subprogramas, basta con organizar las tablas como una pila de entornos locales vinculados a la tabla global, aislando las variables locales de cada activación respecto del ámbito externo.

### 3.8.5 Tipos de Datos

El sistema de tipos constituye uno de los pilares de confiabilidad de un lenguaje de programación, ya que establece el conjunto de valores que puede adoptar cada entidad de datos y las operaciones legítimas aplicables sobre ellos [Louden, 2004, pp. 178-179].

#### Chequeo de tipos y tipos compatibles

* **Chequeo (o verificación) de tipos (*Type checking*):** Es el proceso mediante el cual el compilador o el intérprete verifica que todas las construcciones del programa sean consistentes en términos de los tipos de sus variables, constantes y expresiones, asegurando que los operandos de cada operador sean de tipos compatibles [Louden, 2004, pp. 178, 205; Sebesta, 2012]. Según el momento en que se efectúa, se clasifica en:
  * **Verificación estática de tipos:** Se realiza durante la fase de traducción a partir de las declaraciones presentes en el código fuente, antes de ejecutar el programa [Louden, 2004, p. 205; Pratt & Zelkowitz, p. 123].
  * **Verificación dinámica de tipos:** Se efectúa durante el tiempo de ejecución, inmediatamente antes de aplicar cada operación. Se implementa almacenando una *marca de tipo* (*type tag*) en cada objeto de datos para comprobar su validez antes de operar [Louden, 2004, p. 205; Pratt & Zelkowitz, p. 122].
* **Tipo compatible:** Un tipo es **compatible** respecto de un operador si constituye un tipo legal aceptado directamente por dicho operador, o si las reglas del lenguaje permiten que sea convertido automáticamente (mediante una conversión implícita o coerción) a un tipo legal para dicha operación [Sebesta, 2012].

#### Error de tipo

Un **error de tipo** (*type error*) ocurre cuando se aplica un operador o una operación a uno o más operandos cuyo tipo resulta inapropiado o incompatible con lo que la operación requiere [Sebesta, 2012; Pratt & Zelkowitz, p. 125]. Si estos errores no son detectados por el procesador del lenguaje, el programa interpreta secuencias de bits o estructuras de memoria de manera incorrecta, produciendo corrupción silenciosa de datos o fallos críticos en ejecución [Louden, 2004, p. 179].

#### Lenguajes fuertemente tipados y sus ventajas

Un lenguaje se considera **fuertemente tipado** (*strongly typed*) si su sistema de tipos garantiza que **todos los errores de tipo son detectados siempre** —ya sea en tiempo de traducción o mediante comprobaciones estrictas en tiempo de ejecución—, impidiendo que un programa aplique operaciones ilegales o corrompa datos en memoria [Louden, 2004, p. 179; Pratt & Zelkowitz, p. 124; Sebesta, 2012]. Un factor clave del tipado fuerte es la restricción o ausencia de conversiones implícitas arbitrarias entre tipos, ya que las coerciones excesivas debilitan la capacidad de detectar errores del programador [Pratt & Zelkowitz, p. 124; Sebesta, 2012].

Las principales ventajas de los lenguajes fuertemente tipados son [Louden, 2004, p. 175; Pratt & Zelkowitz, p. 124]:

1. **Mayor confiabilidad y seguridad:** Garantizan la detección de usos erróneos de datos antes de que provoquen fallos silenciosos o corrupción de memoria.
2. **Mejora en la capacidad de escritura y depuración:** Permiten identificar rápidamente errores frecuentes de programación (como intercambiar argumentos o aplicar operaciones a estructuras equivocadas), reduciendo drásticamente el tiempo de depuración.
3. **Mayor legibilidad y documentación:** Al restringir explícitamente qué operaciones se admiten sobre cada clase de dato, el código expresa con mayor claridad el papel de cada estructura dentro del algoritmo.
4. **Eliminación de ambigüedades:** La información rigurosa de tipos permite desambiguar el significado de los operadores y detectar inconsistencias de interfaz en programas extensos.
5. **Eficiencia (cuando se combina con tipado estático):** Permite asignar memoria de forma óptima y generar código máquina eficiente sin comprobaciones repetitivas en tiempo de ejecución.

#### Conversión implícita y conversión explícita de tipos

Una **conversión de tipo** es una operación con signatura $\text{conversión\_op}: \text{tipo}_1 \to \text{tipo}_2$ que toma un valor de un tipo de dato y produce un valor equivalente de otro tipo distinto [Pratt & Zelkowitz, p. 125]. Existen dos mecanismos para realizarla:

* **Conversión implícita (o coerción):** Es efectuada de manera automática por el compilador o el intérprete cuando detecta una discordancia entre el tipo real de un operando y el tipo esperado por un operador [Pratt & Zelkowitz, p. 125]. Por lo general, los lenguajes restringen las coerciones a conversiones de **ensanchamiento** (*widening* o promoción, como convertir un entero a real), donde el tipo destino puede representar al origen sin pérdida de información, evitando las de **estrechamiento** (*narrowing*, como truncar un real a entero), que conllevan pérdida de precisión [Pratt & Zelkowitz, p. 125].
* **Conversión explícita (*casting*):** Es solicitada expresamente en el código fuente por el programador, ya sea mediante operadores de moldeado dedicados (como `(int) x` en C) o mediante funciones de conversión integradas (como `round()` en Pascal) [Pratt & Zelkowitz, p. 125]. Hace visible la intención del cambio de tipo y evita transformaciones inesperadas.

#### Relación con la implementación de Aleph

En Aleph, las abstracciones de datos provistas al programador se circunscriben estrictamente a tres tipos nativos: **Elemento (`STR = 1`)**, **Conjunto (`SET = 2`)** y **Lista (`LIST = 3`)**. Con base en esta arquitectura, nuestro intérprete adopta las siguientes decisiones de diseño:

* **Chequeo dinámico de tipos mediante marcas de tipo:** Al ser Aleph un intérprete puro con ligadura dinámica de tipos en la Tabla de Símbolos (`TAD_symtab`), efectuamos **verificación dinámica de tipos**. Tal como describe Pratt [p. 122], cada objeto en memoria (`TNodo`) porta su propia marca de tipo en el campo `nodeType` (consultada mediante `return_type(nodo)`), y cada operación del evaluador verifica las marcas de sus operandos antes de invocar las rutinas de `TAD_set` o `TAD_ast`, etiquetando correctamente el nodo resultante.
* **Reglas de compatibilidad y detección de errores de tipo:** En la siguiente tabla detallamos la compatibilidad exigida por los operadores de colecciones evaluados en Aleph:

| Operador | Tipo Operando Izquierdo | Tipo Operando Derecho | Tipo del Resultado | Condición de Error de Tipo |
| :--- | :--- | :--- | :--- | :--- |
| `union` (`OP_UNION`) | `SET` | `SET` | `SET` | Cualquier operando distinto de `SET`. |
| `inter` (`OP_INTERSECTION`) | `SET` | `SET` | `SET` | Cualquier operando distinto de `SET`. |
| `diff` (`OP_DIFFERENCE`) | `SET` | `SET` | `SET` | Cualquier operando distinto de `SET`. |
| `insert` (`KW_INSERT`) | `SET` | `STR`, `SET` o `LIST` | `SET` | Operando izquierdo distinto de `SET`. |
| `append` (`KW_APPEND`) | `LIST` | `STR`, `SET` o `LIST` | `LIST` | Operando izquierdo distinto de `LIST`. |

  Ante cualquier incumplimiento de estas reglas —por ejemplo, intentar evaluar `['a', 'b'] union {'c'}` o `'a' insert 'b'`—, el evaluador no ejecuta la operación de bajo nivel sobre los punteros en C, sino que informa explícitamente un **error semántico de tipo** y aborta la evaluación de esa expresión de forma segura.

* **Grado de tipado en Aleph:** Aleph combina **tipado dinámico con verificación estricta (tipado fuerte en tiempo de ejecución)**, de forma análoga al comportamiento de lenguajes interpretados como Scheme [Louden, 2004, p. 205]. Aunque las variables no declaran un tipo estático fijo y pueden re-ligarse a otro tipo en una nueva asignación, ningún operador puede aplicarse sobre operandos incompatibles sin disparar un error de tipo.
* **Política frente a conversiones implícitas y explícitas:** En Aleph **no permitimos conversiones implícitas (coerciones)** entre `STR`, `SET` y `LIST`. Convertir automáticamente una lista en un conjunto para resolver una unión provocaría pérdida silenciosa del orden y de elementos duplicados, mientras que transformar automáticamente un elemento `'a'` en `{'a'}` ocultaría errores lógicos del programador. Asimismo, no incorporamos operadores de conversión explícita (*casting*), ya que la sintaxis literal del lenguaje permite construir directamente la estructura deseada cuando hace falta envolver un valor (por ejemplo, escribiendo `Q union {'c'}` en lugar de `Q union 'c'`).

3.8.6 Semántica de las asignaciones y expresiones
[Explicar y mostrar la implementación de las asignaciones y expresiones de Aleph]

[Evaluación de asignaciones]
