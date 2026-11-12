## 3.7.1. Semántica Estática

La semántica estática abarca aquellas validaciones y reglas de significado que podemos comprobar en el programa fuente antes de su ejecución real. A diferencia del análisis sintáctico (que solo verifica que las oraciones estén bien formadas según la BNF), la semántica estática de Aleph se encarga de dar sentido a esas estructuras, comprobando, por ejemplo, que las variables utilizadas hayan sido previamente declaradas, que las operaciones algebraicas reciban operandos compatibles y que los identificadores respeten las reglas de alcance (scope).

Dado que hemos diseñado Aleph como un intérprete puro, efectuamos gran parte de nuestro análisis estático de manera integrada durante la fase de construcción del Árbol de Sintaxis Abstracta (AST). El analizador sintáctico (Bison) guía la estructura, pero son las acciones semánticas asociadas a cada regla las que se encargan de propagar la información de tipos y construir los nodos en memoria para su posterior evaluación.

##### Gramática de Atributos

Para formalizar cómo fluye la información de contexto y de tipos a través de la sintaxis de Aleph, nos basamos en el concepto de Gramáticas de Atributos propuesto por Knuth (1968) [Louden, 2004, p. 101]. Una gramática de atributos es una extensión de las gramáticas libres de contexto, donde asociamos a cada símbolo de la gramática (terminal o no terminal) un conjunto de propiedades denominadas "atributos", y a cada regla de producción le asignamos funciones semánticas que especifican cómo calcular dichos atributos.

En nuestro diseño diferenciamos dos tipos de atributos:
*   **Atributos Sintetizados:** Son aquellos cuyo valor en el nodo padre se calcula exclusivamente a partir de los valores de los atributos de sus nodos hijos. En implementaciones de análisis ascendente (bottom-up) como la generada por Bison, estos atributos son los más naturales de implementar, ya que la información fluye desde las hojas (los lexemas) hacia la raíz del árbol.
*   **Atributos Heredados:** Son aquellos cuyo valor se calcula tomando información del nodo padre o de los nodos hermanos. Permiten que la información fluya "hacia abajo" o "lateralmente" en el árbol de derivación.

En la arquitectura de nuestro intérprete Aleph, hemos optado por utilizar fundamentalmente **atributos sintetizados**. Cada vez que Flex reconoce un literal o un identificador, emite un token y sintetiza su valor (por ejemplo, el carácter en sí, o el nombre de la variable) hacia Bison mediante la variable `yylval`. Luego, a medida que Bison reduce las reglas gramaticales de abajo hacia arriba, nuestras acciones en C recolectan esos punteros de los hijos para sintetizar un nuevo nodo (una estructura en C tipo `struct ast`) que se pasa al nivel superior mediante `$$`.

### Gramática de atributos para las expresiones de Aleph

Para ilustrar nuestro modelo, definimos a continuación la gramática de atributos sintetizados para las expresiones algebraicas básicas de Aleph. Sea `.val` el atributo que representa el puntero al nodo del AST que estamos construyendo en memoria, definimos las siguientes reglas semánticas para sintetizar el sub-árbol:
```
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
````

## 3.7.2. Semántica Dinámica

La semántica dinámica de un lenguaje de programación define el significado computacional riguroso de cada sentencia y expresión durante el tiempo de ejecución [Louden, 2004, pp. 104-105]. Mientras que la sintaxis y la semántica estática se enfocan en la validez estructural y las restricciones de tipado previo, la semántica dinámica especifica exactamente **qué sucede** cuando el programa es ejecutado por la máquina.

En el diseño de Aleph, la semántica dinámica recae enteramente en el motor de nuestro intérprete puro. Al no compilar a código de máquina nativo, el significado de cada construcción del lenguaje (como un bucle `while` o una operación `union`) está determinado por la rutina en lenguaje C (la función `eval()`) que se invoca recursivamente al visitar el nodo correspondiente del Árbol de Sintaxis Abstracta (AST) en tiempo de ejecución.

#### Semántica Operacional

Existen diversos enfoques formales para describir la semántica dinámica, tales como la Semántica Denotacional (que asocia sentencias a funciones matemáticas) o la Semántica Axiomática (basada en precondiciones lógicas y postcondiciones). Para Aleph, hemos adoptado el enfoque de la **Semántica Operacional**.

La Semántica Operacional describe el significado de un programa especificando cómo su ejecución altera el estado de una máquina matemática abstracta paso a paso [Louden, 2004, p. 106]. En este modelo, el "estado" de la máquina está representado por los valores actuales almacenados en la Tabla de Símbolos, y el significado de una sentencia se comprende evaluando cómo dicha sentencia transforma el estado previo de la memoria en un nuevo estado. 

Para lograr esta descripción sin atarnos a una arquitectura de hardware específica (como x86 o ARM), la semántica operacional requiere que diseñemos un lenguaje intermedio de muy bajo nivel (similar a un lenguaje ensamblador o *bytecode*) que actúa como el conjunto de instrucciones nativas de nuestra máquina abstracta.

### Descripción de las estructuras de control de Aleph

Para describir rigurosamente la semántica operacional de las estructuras de control de flujo en Aleph, definimos a continuación nuestro **Lenguaje Intermedio de Descripción**. Este conjunto de instrucciones primitivas representa las operaciones atómicas que nuestra máquina virtual es capaz de ejecutar:

*   `eval(expr, dir)`: Evalúa la expresión `expr` (que puede ser una operación relacional, lógica o aritmética) y almacena el resultado booleano (o valor resultante) en la dirección de memoria o registro `dir`.
*   `goto(etiqueta)`: Transfiere incondicionalmente el flujo de ejecución (el contador de programa) hacia la instrucción marcada con la `etiqueta`.
*   `if(dir) goto(etiqueta)`: Bifurcación condicional. Transfiere el flujo de control hacia la `etiqueta` únicamente si el valor booleano contenido en `dir` es verdadero.
*   `if(!dir) goto(etiqueta)`: Bifurcación condicional inversa. Transfiere el flujo de control hacia la `etiqueta` únicamente si el valor booleano contenido en `dir` es falso.

A partir de este conjunto de primitivas, procedemos a describir la semántica operacional de las sentencias estructurales de Aleph:

**1. Semántica Operacional de la Alternativa (`if - else`)**

La estructura condicional en Aleph se define sintácticamente como `if ( <op_log> ) do <bloque_1> else <bloque_2> end`. Dinámicamente, su ejecución altera el flujo de la siguiente manera:

```text
    // Estado inicial
    eval(<op_log>, T1)
    if(!T1) goto(L_ELSE)
    
    // Ejecución de <bloque_1> si T1 es verdadero
    <bloque_1>
    goto(L_FIN)
    
L_ELSE:
    // Ejecución de <bloque_2> si T1 es falso
    <bloque_2>
    
L_FIN:
    // El programa continúa su estado normal
```
2. Semántica Operacional de la Iteración (while)

La estructura repetitiva básica en Aleph se define sintácticamente como while ( <op_log> ) do <bloque> end. Su semántica operacional requiere una evaluación constante de la guardia booleana antes de cada ejecución del cuerpo, generando el siguiente rastro de estados:
```
L_INICIO_WHILE:
    // Evaluación de la condición de parada
    eval(<op_log>, T1)
    if(!T1) goto(L_FIN_WHILE)
    
    // Modificación del estado a través del bloque
    <bloque>
    
    // Retorno incondicional a la evaluación
    goto(L_INICIO_WHILE)
    
L_FIN_WHILE:
    // Fin de la iteración. El programa continúa.
````

## 3.8 Análisis Semántico

El análisis semántico constituye la fase final y decisiva en el *front-end* de nuestro intérprete Aleph. Mientras que el analizador léxico agrupa los caracteres en tokens válidos y el analizador sintáctico verifica que la secuencia de estos tokens obedezca la gramática formal (BNF), el análisis semántico se encarga de dotar de significado computacional a dichas estructuras.

En esta etapa, nuestro intérprete verifica la consistencia de tipos, asegurando que las operaciones conjuntistas y de listas operen sobre operandos válidos. Asimismo, es durante este análisis dirigido por la sintaxis donde construimos en memoria la representación jerárquica del programa fuente: el Árbol de Sintaxis Abstracta (AST), el cual será posteriormente recorrido por el motor de evaluación para su ejecución dinámica.

#### 3.8.1 Construcción del AST y evaluación de expresiones

**Estructura de datos y módulos para la construcción del AST**

Para representar los programas escritos en Aleph, hemos estructurado nuestro Árbol de Sintaxis Abstracta aprovechando las bases de nuestro Tipo Abstracto de Datos (TAD) construido previamente para manipular conjuntos y listas en memoria dinámica. El núcleo de nuestra representación recae en la estructura polimórfica `dataType` (`TNodo`).

A diferencia de un árbol binario clásico que posee explícitamente punteros "izquierdo" y "derecho", nuestra implementación modela los nodos del AST utilizando una arquitectura de listas enlazadas generalizadas, lo que nos brinda extrema flexibilidad para representar tanto sentencias de control anidadas como colecciones algebraicas arbitrarias.

```c
#define STR 1
#define SET 2
#define LIST 3
// Extendible a tipos adicionales de AST como OP_UNION, IF_NODE, WHILE_NODE, etc.

typedef struct dataType {
    int nodeType;   /* Identificador de tipo de nodo o instrucción */
    union {
        str string; /* Almacenamiento de valores terminales (Identificadores, Literales) */
        struct {
            struct dataType* data; /* Puntero al primer operando o nodo hijo (rama inferior) */
            struct dataType* next; /* Puntero al siguiente operando o sentencia (hermano) */
        };
    };
} TNodo;

typedef TNodo* Tdata; // Puntero al nodo raíz de un sub-árbol
````
Durante el proceso de reducción sintáctica (acción reduce de Bison), las reglas formales de la gramática invocan funciones constructivas en C. Por ejemplo, al evaluar una colección literal, Bison ejecuta módulos constructores como create_set() o create_list() que instancian un TNodo[cite: 17, 18]. Los elementos terminales se enlazan secuencialmente a través del puntero next, colgando como hijos desde el puntero data del nodo padre. De esta manera, el programa completo en Aleph se transforma en un único nodo raíz Tdata desde el cual se desprenden todos los bloques de código, expresiones y asignaciones.

Estructura de datos y módulos para evaluar el AST

Una vez que Bison ha finalizado la construcción del AST con éxito, el control del flujo del programa es transferido a nuestra función principal de interpretación, la cual evaluamos algorítmicamente mediante un recorrido en profundidad del árbol (Tree-Walk Evaluator).

Esta rutina, típicamente estructurada mediante una gran sentencia switch sobre el atributo nodeType de nuestro TNodo, determina la semántica dinámica (operacional) de la instrucción representada. A continuación presentamos la estructura conceptual de nuestro motor de evaluación eval():
````
Tdata eval(Tdata ast) {
    if (ast == NULL) return NULL;
    
    // Variables temporales para operandos
    Tdata izq, der, resultado;
    
    switch (ast->nodeType) {
        
        // 1. Evaluación de terminales (hojas del AST)
        case STR: 
        case SET: 
        case LIST:
            // Retorna una copia del valor del nodo en memoria
            return copy_ast(ast); 
            
        // 2. Operaciones de colecciones (Ej: Unión)
        case OP_UNION:
            // Evalúa recursivamente ambos operandos (hijos)
            izq = eval(ast->data);
            der = eval(ast->data->next);
            
            // Ejecuta la lógica matemática pura sobre nuestras estructuras
            if (izq->nodeType == SET && der->nodeType == SET) {
                 resultado = union_set(izq, der); // Función interna de lógica de conjuntos
            } else {
                 yyerror("Error semántico: Operandos incompatibles para operador union.");
                 resultado = NULL;
            }
            free_ast(izq); free_ast(der);
            return resultado;
            
        // 3. Estructura de Control Iterativa (While)
        case WHILE_NODE:
            // Evalúa la condición (guardia). Mientras sea verdadera, ejecuta el cuerpo.
            while (es_verdadero( eval(ast->data) )) {
                 // El cuerpo del while está enlazado a continuación de la condición
                 eval(ast->data->next); 
            }
            return NULL;
            
        default:
            printf("Error interno: Tipo de nodo desconocido.\n");
            return NULL;
    }
}
````
El motor eval() opera bajo el principio de recursión: para resolver una operación compleja, primero desencadena la evaluación de los nodos hijos en el AST (ast->data y ast->data->next). Una vez que los valores se materializan (retornando un puntero Tdata a la abstracción concreta de lista, string o conjunto), aplica la lógica del lenguaje Aleph y propaga el resultado modificado hacia la raíz, concretando así la ejecución del programa.
