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


3.8.3 Ligaduras
[Defina ligadura y tiempos de ligadura.]

[Defina ligadura estática y ligadura dinámica]

[Defina variables estáticas, dinámicas de pila, explicitas de montón dinámico, implícitas de montón dinámico. Enumere las ventajas y desventajas de cada una.]

[Relacione estos conceptos con la implementación de Aleph]

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
