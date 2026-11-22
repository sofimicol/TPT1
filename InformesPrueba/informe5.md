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

3.8.4 Ámbito
[Defina: tiempo de vida, alcance, alcance estático y alcance dinámico.]

[Defina: ancestro estático y dinámico de un subprograma]

[Relacione estos conceptos con la implementación de Aleph]

3.8.5 Tipos de Datos
[Defina: Chequeo de tipos. ¿Qué es un tipo compatible?]

[Defina: error de tipo]

[Defina: fuertemente tipado y enumere las ventajas de los lenguajes de este tipo.]

[Defina: conversión implícita y conversión explícita de tipos.]

[Relacione estos conceptos con la implementación de Aleph]

3.8.6 Semántica de las asignaciones y expresiones
[Explicar y mostrar la implementación de las asignaciones y expresiones de Aleph]

[Evaluación de asignaciones]
